#include "shell.h"

char *history[MAX_HISTORY];
int history_count = 0;
int current_history = -1;

struct termios orig_termios;

// 初始化终端为原始模式
void enable_raw_mode() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    struct termios raw = orig_termios;
    // 禁用回显和规范模式，但保留其他功能
    raw.c_lflag &= ~(ECHO | ICANON);
    // 禁用 Ctrl-C 和 Ctrl-Z 信号
    raw.c_lflag &= ~(ISIG);
    // 禁用 Ctrl-S 和 Ctrl-Q 控制流
    raw.c_iflag &= ~(IXON);
    // 禁用 CR 到 NL 的转换
    raw.c_iflag &= ~(ICRNL);
    // 输出处理
    raw.c_oflag &= ~(OPOST);
    // 设置读取超时
    raw.c_cc[VMIN] = 0;  // 最小字符数
    raw.c_cc[VTIME] = 1; // 超时 (十分之一秒)
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// 恢复终端设置
void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

// 清除当前行
void clear_line(int position) {
    printf("\r");           // 回到行首
    for (int i = 0; i < position + 2; i++) {
        printf(" "); 
    }
    printf("\r");
}

// 刷新显示当前行
void refresh_line(const char *prompt, char *buffer) {
    printf("\r%s%s", prompt, buffer);
    fflush(stdout);
}

// 添加命令
void add_to_history(const char *cmd) {
    if (strlen(cmd) == 0) return;
    
    // 避免连续相同的命令
    if (history_count > 0 && strcmp(history[history_count-1], cmd) == 0) {
        return;
    }

    if (history_count >= MAX_HISTORY) {
        free(history[0]);
        for (int i = 0; i < history_count - 1; i++) {
            history[i] = history[i+1];
        }
        history_count--;
    }
    
    history[history_count] = strdup(cmd);
    history_count++;
}

// 读取一行输入
char* read_line(const char *prompt) {
    char *buffer = (char *)malloc(MAX_INPUT_SIZE);
    int position = 0;
    buffer[0] = '\0';
    current_history = history_count;
    
    printf("%s", prompt);
    fflush(stdout);
    
    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 0) {
            continue; // 超时
        }
        
        // 处理退出和换行
        if (c == 4) { // Ctrl-D
            free(buffer);
            return NULL;
        } else if (c == '\r' || c == '\n') {
            printf("\r\n");
            buffer[position] = '\0';
            return buffer;
        } 
        // 处理退格键
        else if (c == 127 || c == 8) { // Backspace
            if (position > 0) {
                position--;
                buffer[position] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
        } 
        // 处理方向键和其他特殊键序列
        else if (c == 27) {
            char seq[3];
            
            if (read(STDIN_FILENO, &seq[0], 1) == 0) continue;
            if (read(STDIN_FILENO, &seq[1], 1) == 0) continue;
            
            if (seq[0] == '[') {
                if (seq[1] == 'A') { // 上箭头
                    if (history_count > 0 && current_history > 0) {
                        current_history--;
                        clear_line(position);
                        strcpy(buffer, history[current_history]);
                        position = strlen(buffer);
                        refresh_line(prompt, buffer);
                    }
                } else if (seq[1] == 'B') { // 下箭头
                    if (current_history < history_count) {
                        current_history++;
                        clear_line(position);
                        
                        // 显示较新的历史命令或清空
                        if (current_history == history_count) {
                            buffer[0] = '\0';
                            position = 0;
                        } else {
                            strcpy(buffer, history[current_history]);
                            position = strlen(buffer);
                        }
                        refresh_line(prompt, buffer);
                    }
                }
            }
        } 
        // 处理常规输入
        else if (c >= 32 && c < 127) {
            if (position < MAX_INPUT_SIZE - 1) {
                buffer[position++] = c;
                buffer[position] = '\0';
                printf("%c", c);
                fflush(stdout);
            }
        }
    }
}

// 清理历史记录
void cleanup_history() {
    for (int i = 0; i < history_count; i++) {
        free(history[i]);
    }
}