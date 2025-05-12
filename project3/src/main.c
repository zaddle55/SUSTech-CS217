#include <ctype.h>
#include <getopt.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../inc/bmp.h"

#define MAX_PATH 1024
#define MAX_CLR_MODE 100
#define ERR_INVALID_COMMAND 1
#define ERR_MISSING_ARGS 2
#define ERR_INVALID_FORMAT 3
#define ERR_FILE_NOT_FOUND 4
#define ERR_INVALID_PARAM 5
#define ERR_SIZE_MISMATCH 6
#define ERR_UNSUPPORTED_FORMAT 7
#define ERR_UNSUPPORTED_CONVERSION 8

typedef enum {
  CMD_UNKNOWN,
  CMD_OP,
  CMD_CONV,
  CMD_CVTCLR,
  CMD_FFT,
  CMD_SCALE,
  CMD_TRANS,
  CMD_FLIP,
  CMD_ROTATE,
  CMD_NOISE,
  CMD_ART,
  CMD_LINE,
  CMD_RECT,
  CMD_CIRCLE,
  CMD_CLIP,
  CMD_B64,
  CMD_INFO
} CommandType;

typedef struct {
  CommandType cmd;
  char src1[MAX_PATH];
  char src2[MAX_PATH];
  char dest[MAX_PATH];

  // Op command params
  OpRule rule;
  float w1;
  float w2;

  // conv params
  bool padding;

  // CVT color params
  char colorMode[MAX_CLR_MODE];

  // FFT params
  bool shift;

  // Scale params
  double scaleX;
  double scaleY;

  // Trans params
  int transX;
  int transY;

  // Rotate angle
  double angle;

  // Flip axis
  bool flipX;
  bool flipY;

  // Noise params
  float probability;

  // Art params
  int color;

  // Line, Rect, Circle params
  int x0, y0, x1, y1;
  int x, y, dx, dy;
  int rx, ry;
  double r;
} CommandParams;

// Function prototypes
void print_usage();
int parse_command(int argc, char *argv[], CommandParams *params);
int execute_command(CommandParams *params);
int check_files_exist(const char *files[], int count);
int parse_op_command(int argc, char *argv[], CommandParams *params);
int parse_conv_command(int argc, char *argv[], CommandParams *params);
int parse_cvtclr_command(int argc, char *argv[], CommandParams *params);
int parse_fft_command(int argc, char *argv[], CommandParams *params);
int parse_scale_command(int argc, char *argv[], CommandParams *params);
int parse_trans_command(int argc, char *argv[], CommandParams *params);
int parse_rotate_command(int argc, char *argv[], CommandParams *params);
int parse_flip_command(int argc, char *argv[], CommandParams *params);
int parse_noise_command(int argc, char *argv[], CommandParams *params);
int parse_art_command(int argc, char *argv[], CommandParams *params);
int parse_line_command(int argc, char *argv[], CommandParams *params);
int parse_rect_command(int argc, char *argv[], CommandParams *params);
int parse_circle_command(int argc, char *argv[], CommandParams *params);
int parse_clip_command(int argc, char *argv[], CommandParams *params);
int parse_b64_command(int argc, char *argv[], CommandParams *params);
int parse_info_command(int argc, char *argv[], CommandParams *params);
int parse_hex_color(const char *str);

// Image processing function prototypes (to be implemented elsewhere)
int image_merge(const char *src1, const char *src2, const char *dest,
                OpRule rule, float w1, float w2);
int image_conv(const char *src1, const char *src2, const char *dest,
                bool is_padding);
int image_convert_color(const char *src, const char *dest, const char* mode);
int image_fft(const char *src, const char *dest, bool shift);
int image_scale(const char *src, const char *dest, double scaleX, double scaleY);
int image_translate(const char *src, const char *dest, int tx, int ty);
int image_add_noise(const char *src, const char *dest, float probability);
int image_rotate(const char *src, const char *dest, double angle);
int image_flip(const char *src, const char *dest, bool flipX, bool flipY);
int image_to_ascii(const char *src,
                                                      int color);
int image_draw_line(const char *src, const char *dest, int color, int x0,
                    int y0, int x1, int y1);
int image_draw_rect(const char *src, const char *dest, int color, int x, int y,
                    int dx, int dy);
int image_draw_circle(const char *src, const char *dest, int color, double r,
                      int rx, int ry);
int image_clip(const char *src, const char *dest, int x, int y, int dx, int dy);
int image_to_base64(const char *src, const char *dest);
int image_info(const char *src);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    print_usage();
    return ERR_INVALID_COMMAND;
  }

  CommandParams params = {CMD_UNKNOWN};
  int result = parse_command(argc, argv, &params);

  if (result != 0) {
    return result;
  }

  return execute_command(&params);
}

void print_usage() {
  printf("CImageProcess - BMP Image Processing CLI\n");
  printf("Supported formats: 24-bit BMP (RGB, BGR, HSV, grayscale)\n\n");
  printf("Commands:\n");
  printf("  cimage op [src1.ext] + [src2.ext] -o [dest.ext] -r [rule] "
         "[--w1=xxx] [--w2=xxx]\n");
  printf("  cimage conv [src1.ext] x [src2.ext] -o [dest.ext] [--no-padding]\n");
  printf("  cimage cvtclr -i [src.ext] -o [dest.ext] -m [mode]\n");
  printf("  cimage fft -i [src.ext] -o [dest.ext] [--no-shift]\n");
  printf("  cimage scale -i [src.ext] -o [dest.ext] --sx=[scaleX] "
         "--sy=[scaleY]\n");
  printf("  cimage rotate -i [src.ext] -o [dest.ext] -a [angle]\n");
  printf("  cimage flip -i [src.ext] -o [dest.ext] --x --y");
  printf("  cimage trans -i [src.ext] -o [dest.ext] --tx=[transX] "
         "--ty=[transY]\n");
  printf("  cimage noise -i [src.ext] -o [dest.ext] -p [probability]\n");
  printf("  cimage art -i [src.ext] -c [color]\n");
  printf("  cimage line -i [src.ext] -o [dest.ext] -c [color] --x0=[xxx] "
         "--y0=[xxx] --x1=[xxx] --y1=[xxx]\n");
  printf("  cimage rect -i [src.ext] -o [dest.ext] -c [color] --x=[xxx] "
         "--y=[xxx] --dx=[xxx] --dy=[xxx]\n");
  printf("  cimage circle -i [src.ext] -o [dest.ext] -c [color] --r=[xxx] "
         "--rx=[xxx] --ry=[xxx]\n");
  printf("  cimage clip -i [src.ext] -o [dest.ext] --x=[xxx] --y=[xxx] "
         "--dx=[xxx] --dy=[xxx]\n");
  printf("  cimage b64 -i [src.ext] -o [dest.ext]\n");
  printf("  cimage info -i [src.ext]\n");
  printf("\nFor detailed help on each command, use: cimage [command] --help\n");
}

int parse_command(int argc, char *argv[], CommandParams *params) {
  if (argc < 2) {
    printf("Error: No command specified\n");
    print_usage();
    return ERR_INVALID_COMMAND;
  }

  if (strcmp(argv[1], "op") == 0) {
    params->cmd = CMD_OP;
    return parse_op_command(argc, argv, params);
  } else if (strcmp(argv[1], "conv") == 0) {
    params->cmd = CMD_CONV;
    return parse_conv_command(argc, argv, params);
  } else if (strcmp(argv[1], "cvtclr") == 0) {
    params->cmd = CMD_CVTCLR;
    return parse_cvtclr_command(argc, argv, params);
  } else if (strcmp(argv[1], "fft") == 0) {
    params->cmd = CMD_FFT;
    return parse_fft_command(argc, argv, params);
  } else if (strcmp(argv[1], "scale") == 0) {
    params->cmd = CMD_SCALE;
    return parse_scale_command(argc, argv, params);
  } else if (strcmp(argv[1], "trans") == 0) {
    params->cmd = CMD_TRANS;
    return parse_trans_command(argc, argv, params);
  } else if (strcmp(argv[1], "rotate") == 0) {
    params->cmd = CMD_ROTATE;
    return parse_rotate_command(argc, argv, params);
  } else if (strcmp(argv[1], "flip") == 0) {
      params->cmd = CMD_FLIP;
      return parse_flip_command(argc, argv, params);
  } else if (strcmp(argv[1], "noise") == 0) {
    params->cmd = CMD_NOISE;
    return parse_noise_command(argc, argv, params);
  } else if (strcmp(argv[1], "art") == 0) {
    params->cmd = CMD_ART;
    return parse_art_command(argc, argv, params);
  } else if (strcmp(argv[1], "line") == 0) {
    params->cmd = CMD_LINE;
    return parse_line_command(argc, argv, params);
  } else if (strcmp(argv[1], "rect") == 0) {
    params->cmd = CMD_RECT;
    return parse_rect_command(argc, argv, params);
  } else if (strcmp(argv[1], "circle") == 0) {
    params->cmd = CMD_CIRCLE;
    return parse_circle_command(argc, argv, params);
  } else if (strcmp(argv[1], "clip") == 0) {
    params->cmd = CMD_CLIP;
    return parse_clip_command(argc, argv, params);
  } else if (strcmp(argv[1], "b64") == 0) {
    params->cmd = CMD_B64;
    return parse_b64_command(argc, argv, params);
  } else if (strcmp(argv[1], "info") == 0) {
    params->cmd = CMD_INFO;
    return parse_info_command(argc, argv, params);
  } else {
    printf("Error: Unknown command '%s'\n", argv[1]);
    print_usage();
    return ERR_INVALID_COMMAND;
  }
}

int parse_op_command(int argc, char *argv[], CommandParams *params) {
  // Initialize defaults
  params->rule = OP_RULE_AVG;
  params->w1 = 0.5f;
  params->w2 = 0.5f;

  // Check for minimum required arguments
  if (argc < 6) {
    printf("Error: Insufficient arguments for 'op' command\n");
    printf("Usage: cimage op [src1.ext] + [src2.ext] -o [dest.ext] -r [rule] "
           "[--w1=xxx] [--w2=xxx]\n");
    return ERR_MISSING_ARGS;
  }

  // Parse src1 + src2 format
  if (strcmp(argv[3], "+") != 0) {
    printf("Error: Expected '+' operator between source files\n");
    return ERR_INVALID_FORMAT;
  }

  strncpy(params->src1, argv[2], MAX_PATH - 1);
  strncpy(params->src2, argv[4], MAX_PATH - 1);

  // Check if files exist
  const char *files[] = {params->src1, params->src2};
  if (check_files_exist(files, 2) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  // Parse optional arguments
  int i = 5;
  bool output_set = false;

  while (i < argc) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      strncpy(params->dest, argv[i + 1], MAX_PATH - 1);
      output_set = true;
      i += 2;
    } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
      if (strcmp(argv[i + 1], "max") == 0) {
        params->rule = OP_RULE_MAX;
      } else if (strcmp(argv[i + 1], "avg") == 0) {
        params->rule = OP_RULE_AVG;
      } else if (strcmp(argv[i + 1], "weight") == 0) {
        params->rule = OP_RULE_WEIGHT;
      } else if (strcmp(argv[i + 1], "gauss") == 0) {
        params->rule = OP_RULE_GAUSS;
      } else {
        printf("Error: Unknown rule '%s'. Valid rules are: max, avg, weight, "
               "gauss\n",
               argv[i + 1]);
        return ERR_INVALID_PARAM;
      }
      i += 2;
    } else if (strncmp(argv[i], "--w1=", 5) == 0) {
      char *value = argv[i] + 5;
      params->w1 = atof(value);
      if (params->w1 < 0.0f || params->w1 >= 1.0f) {
        printf("Error: Weight w1 must be in range [0, 1)\n");
        return ERR_INVALID_PARAM;
      }
      i++;
    } else if (strncmp(argv[i], "--w2=", 5) == 0) {
      char *value = argv[i] + 5;
      params->w2 = atof(value);
      if (params->w2 < 0.0f || params->w2 >= 1.0f) {
        printf("Error: Weight w2 must be in range [0, 1)\n");
        return ERR_INVALID_PARAM;
      }
      i++;
    } else {
      printf("Error: Unknown option '%s' for 'op' command\n", argv[i]);
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if weight is required for weight rule
  if (params->rule == OP_RULE_WEIGHT && (params->w1 + params->w2 != 1.0f)) {
    printf("Warning: Weights do not sum to 1.0. Normalizing weights.\n");
    float sum = params->w1 + params->w2;
    if (sum > 0) {
      params->w1 /= sum;
      params->w2 /= sum;
    } else {
      params->w1 = params->w2 = 0.5f;
    }
  }

  return 0;
}

int parse_conv_command(int argc, char *argv[], CommandParams *params) {
  // Initialize defaults
  params->padding = true;

  // Check for minimum required arguments
  if (argc < 6) {
    printf("Error: Insufficient arguments for 'conv' command\n");
    printf("Usage: cimage conv [src1.ext] x [src2.ext] -o [dest.ext] [--no-padding]\n");
    return ERR_MISSING_ARGS;
  }

  // Parse src1 + src2 format
  if (strcmp(argv[3], "x") != 0) {
    printf("Error: Expected 'x' operator between source files\n");
    return ERR_INVALID_FORMAT;
  }

  strncpy(params->src1, argv[2], MAX_PATH - 1);
  strncpy(params->src2, argv[4], MAX_PATH - 1);

  // Check if files exist
  const char *files[] = {params->src1, params->src2};
  if (check_files_exist(files, 2) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  // Parse optional arguments
  int i = 5;
  bool output_set = false;

  while (i < argc) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      strncpy(params->dest, argv[i + 1], MAX_PATH - 1);
      output_set = true;
      i += 2;
    } else if (strncmp(argv[i], "--no-padding", 12) == 0) {
      params->padding = false;
      i++;
    } else {
      printf("Error: Unknown option '%s' for 'op' command\n", argv[i]);
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  return 0;
}

int parse_cvtclr_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, mode_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:m:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'm':
      strncpy(params->colorMode, optarg, MAX_CLR_MODE - 1);
      mode_set = true;
      break;
    case 'h':
      printf("Usage: cimage cvtclr -i [src.ext] -o [dest.ext] -m [mode]\n");
      printf("Modes: rgb2bgr, rgb2hsv, rgb2gray, bgr2rgb, bgr2hsv, bgr2gray, "
             "hsv2rgb, hsv2bgr, hsv2gray\n");
      return 1;
    default:
      printf("Error: Unknown option for 'cvtclr' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!mode_set) {
    printf("Error: Color conversion mode (-m) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_fft_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->shift = true;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"no-shift", no_argument, 0, 'n'},
                                         {"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:nh", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'n':
      params->shift = false;
      break;
    case 'h':
      printf("Usage: cimage fft -i [src.ext] -o [dest.ext] [--no-shift]\n");
      printf("Performs FFT on the source image and saves the spectrum\n");
      printf("  --no-shift: Don't shift the zero frequency to the center\n");
      return 1;
    default:
      printf("Error: Unknown option for 'fft' command\n");  
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_scale_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->scaleX = params->scaleY = 1.0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, scale_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"sx", required_argument, 0, '1'},
                                         {"sy", required_argument, 0, '2'},
      {0, 0, 0, 0},
};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) !=
         -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->scaleX = atof(optarg);
      scale_set = true;
      break;
    case '2':
      params->scaleY = atof(optarg);
      scale_set = true;
      break;
    case 'h':
      printf("Usage: cimage scale -i [src.ext] -o [dest.ext] --sx=[scaleX] "
             "--sy=[scaleY] -m [mode]\n");
      printf("Scales the image by the given factor\n");
      printf("  -s: Scale factor (positive number)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'scale' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!scale_set) {
    printf("Error: Scale factor (--sx or --sy) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_trans_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->transX = 0;
  params->transY = 0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, trans_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"tx", required_argument, 0, '1'},
                                         {"ty", required_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->transX = atoi(optarg);
      trans_set = true;
      break;
    case '2':
      params->transY = atoi(optarg);
      trans_set = true;
      break;
    case 'h':
      printf("Usage: cimage trans -i [src.ext] -o [dest.ext] --tx=[transX] --ty="
             "[transY]\n");
      printf("Translates the image by the given offsets\n");
      return 1;
    default:
      printf("Error: Unknown option for 'trans' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!trans_set) {
    printf("Error: Neither X nor Y translation values (--tx or --ty) has been specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_rotate_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->angle = 0.0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, angle_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:a:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'a':
      params->angle = atof(optarg);
      angle_set = true;
      break;
    case 'h':
      printf(
          "Usage: cimage rotate -i [src.ext] -o [dest.ext] -a [angle]\n");
      printf("Rotate the image at the center by the given angle\n");
      return 1;
    default:
      printf("Error: Unknown option for 'rotate' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!angle_set) {
    printf("Error: The angle (-a) must be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_flip_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->flipX = false;
  params->flipY = false;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"x", no_argument, 0, '1'},
                                         {"y", no_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '1':
      params->flipX = true;
      break;
    case '2':
      params->flipY = true;
      break;
    case 'h':
      printf(
          "Usage: cimage flip -i [src.ext] -o [dest.ext] --x --y\n");
      printf("Flip the image by the given directions\n");
      return 1;
    default:
      printf("Error: Unknown option for 'flip' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_noise_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->probability = 0.05f;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false, prob_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:p:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'p':
      params->probability = atof(optarg);
      if (params->probability < 0 || params->probability > 1) {
        printf("Error: Probability must be in range [0, 1]\n");
        return ERR_INVALID_PARAM;
      }
      prob_set = true;
      break;
    case 'h':
      printf(
          "Usage: cimage noise -i [src.ext] -o [dest.ext] -p [probability]\n");
      printf("Adds salt and pepper noise to the image\n");
      printf("  -p: Noise probability (0.0 to 1.0)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'noise' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!prob_set) {
    printf("Error: Noise probability (-p) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_art_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0xFFFFFF; // White

  // Parse with getopt
  int opt;
  bool input_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case 'h':
      printf("Usage: cimage art -i [src.ext] -c [color]\n");
      printf("Converts image to ASCII art and prints to terminal\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      return 1;
    default:
      printf("Error: Unknown option for 'art' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_line_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x0_set = false, y0_set = false, x1_set = false, y1_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x0", required_argument, 0, '0'},
      {"y0", required_argument, 0, '1'}, {"x1", required_argument, 0, '2'},
      {"y1", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // x0
      params->x0 = atoi(optarg);
      x0_set = true;
      break;
    case '1': // y0
      params->y0 = atoi(optarg);
      y0_set = true;
      break;
    case '2': // x1
      params->x1 = atoi(optarg);
      x1_set = true;
      break;
    case '3': // y1
      params->y1 = atoi(optarg);
      y1_set = true;
      break;
    case 'h':
      printf("Usage: cimage line -i [src.ext] -o [dest.ext] -c [color] "
             "--x0=[xxx] --y0=[xxx] --x1=[xxx] --y1=[xxx]\n");
      printf("Draws a line on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --x0, --y0: Starting point coordinates\n");
      printf("  --x1, --y1: Ending point coordinates\n");
      return 1;
    default:
      printf("Error: Unknown option for 'line' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x0_set || !y0_set || !x1_set || !y1_set) {
    printf("Error: Line coordinates (--x0, --y0, --x1, --y1) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_rect_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x_set = false, y_set = false, dx_set = false, dy_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x", required_argument, 0, '0'},
      {"y", required_argument, 0, '1'},  {"dx", required_argument, 0, '2'},
      {"dy", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // x
      params->x = atoi(optarg);
      x_set = true;
      break;
    case '1': // y
      params->y = atoi(optarg);
      y_set = true;
      break;
    case '2': // dx
      params->dx = atoi(optarg);
      dx_set = true;
      break;
    case '3': // dy
      params->dy = atoi(optarg);
      dy_set = true;
      break;
    case 'h':
      printf("Usage: cimage rect -i [src.ext] -o [dest.ext] -c [color] "
             "--x=[xxx] --y=[xxx] --dx=[xxx] --dy=[xxx]\n");
      printf("Draws a rectangle on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --x, --y: Top-left corner coordinates\n");
      printf("  --dx, --dy: Width and height of the rectangle\n");
      return 1;
    default:
      printf("Error: Unknown option for 'rect' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x_set || !y_set || !dx_set || !dy_set) {
    printf("Error: Rectangle parameters (--x, --y, --dx, --dy) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_circle_command(int argc, char *argv[], CommandParams *params) {
  // Default parameters
  params->color = 0x000000; // Black
  params->r = 0;
  params->rx = 0;
  params->ry = 0;

  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool r_set = false, rx_set = false, ry_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {"r", required_argument, 0, '0'},
                                         {"rx", required_argument, 0, '1'},
                                         {"ry", required_argument, 0, '2'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:c:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'c':
      params->color = parse_hex_color(optarg);
      if (params->color < 0) {
        return ERR_INVALID_PARAM;
      }
      break;
    case '0': // r
      params->r = atof(optarg);
      r_set = true;
      break;
    case '1': // rx
      params->rx = atoi(optarg);
      rx_set = true;
      break;
    case '2': // ry
      params->ry = atoi(optarg);
      ry_set = true;
      break;
    case 'h':
      printf("Usage: cimage circle -i [src.ext] -o [dest.ext] -c [color] "
             "--r=[radius] --rx=[centerX] --ry=[centerY]\n");
      printf("Draws a circle on the image\n");
      printf("  -c: Color in hex format (e.g., FF0000 for red)\n");
      printf("  --r: Radius of the circle\n");
      printf("  --rx, --ry: Center coordinates\n");
      return 1;
    default:
      printf("Error: Unknown option for 'circle' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!r_set || !rx_set || !ry_set) {
    printf(
        "Error: Circle parameters (--r, --rx, --ry) must all be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (params->r <= 0) {
    printf("Error: Radius must be positive\n");
    return ERR_INVALID_PARAM;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_clip_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;
  bool x_set = false, y_set = false, dx_set = false, dy_set = false;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},     {"x", required_argument, 0, '0'},
      {"y", required_argument, 0, '1'},  {"dx", required_argument, 0, '2'},
      {"dy", required_argument, 0, '3'}, {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case '0': // x
      params->x = atoi(optarg);
      x_set = true;
      break;
    case '1': // y
      params->y = atoi(optarg);
      y_set = true;
      break;
    case '2': // dx
      params->dx = atoi(optarg);
      dx_set = true;
      break;
    case '3': // dy
      params->dy = atoi(optarg);
      dy_set = true;
      break;
    case 'h':
      printf("Usage: cimage clip -i [src.ext] -o [dest.ext] --x=[xxx] "
             "--y=[xxx] --dx=[xxx] --dy=[xxx]\n");
      printf("Clips a rectangular region from the image\n");
      printf("  --x, --y: Top-left corner coordinates\n");
      printf("  --dx, --dy: Width and height of the region\n");
      return 1;
    default:
      printf("Error: Unknown option for 'clip' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!x_set || !y_set || !dx_set || !dy_set) {
    printf("Error: Clipping parameters (--x, --y, --dx, --dy) must all be "
           "specified\n");
    return ERR_MISSING_ARGS;
  }

  if (params->dx <= 0 || params->dy <= 0) {
    printf("Error: Width and height must be positive\n");
    return ERR_INVALID_PARAM;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_b64_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false, output_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:o:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'o':
      strncpy(params->dest, optarg, MAX_PATH - 1);
      output_set = true;
      break;
    case 'h':
      printf("Usage: cimage b64 -i [src.ext] -o [dest.ext]\n");
      printf("Encodes the image to base64 and saves to the output file\n");
      return 1;
    default:
      printf("Error: Unknown option for 'b64' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  if (!output_set) {
    printf("Error: Output file (-o) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int parse_info_command(int argc, char *argv[], CommandParams *params) {
  // Parse with getopt
  int opt;
  bool input_set = false;

  static struct option long_options[] = {{"help", no_argument, 0, 'h'},
                                         {0, 0, 0, 0}};

  optind = 2; // Skip the command name

  while ((opt = getopt_long(argc, argv, "i:h", long_options, NULL)) != -1) {
    switch (opt) {
    case 'i':
      strncpy(params->src1, optarg, MAX_PATH - 1);
      input_set = true;
      break;
    case 'h':
      printf("Usage: cimage info -i [src.ext]\n");
      printf("Displays information about the image\n");
      return 1;
    default:
      printf("Error: Unknown option for 'info' command\n");
      return ERR_INVALID_PARAM;
    }
  }

  // Check required parameters
  if (!input_set) {
    printf("Error: Input file (-i) must be specified\n");
    return ERR_MISSING_ARGS;
  }

  // Check if input file exists
  const char *files[] = {params->src1};
  if (check_files_exist(files, 1) != 0) {
    return ERR_FILE_NOT_FOUND;
  }

  return 0;
}

int execute_command(CommandParams *params) {
  switch (params->cmd) {
  case CMD_OP:
    return image_merge(params->src1, params->src2, params->dest, params->rule,
                       params->w1, params->w2);
  case CMD_CONV:
    return image_conv(params->src1, params->src2, params->dest,params->padding);
  case CMD_CVTCLR:
    return image_convert_color(params->src1, params->dest, params->colorMode);
  case CMD_FFT:
    return image_fft(params->src1, params->dest, params->shift);
  case CMD_SCALE:
    return image_scale(params->src1, params->dest, params->scaleX,
                       params->scaleY);
  case CMD_TRANS:
    return image_translate(params->src1, params->dest, params->transX,
                           params->transY);
  case CMD_NOISE:
    return image_add_noise(params->src1, params->dest, params->probability);
  case CMD_ROTATE:
    return image_rotate(params->src1, params->dest, params->angle);
  case CMD_FLIP:
    return image_flip(params->src1, params->dest, params->flipX, params->flipY);
  case CMD_ART:
    return image_to_ascii(params->src1, params->color);
  case CMD_LINE:
    return image_draw_line(params->src1, params->dest, params->color,
                           params->x0, params->y0, params->x1, params->y1);
  case CMD_RECT:
    return image_draw_rect(params->src1, params->dest, params->color, params->x,
                           params->y, params->dx, params->dy);
  case CMD_CIRCLE:
    return image_draw_circle(params->src1, params->dest, params->color,
                             params->r, params->rx, params->ry);
  case CMD_CLIP:
    return image_clip(params->src1, params->dest, params->x, params->y,
                      params->dx, params->dy);
  case CMD_B64:
    return image_to_base64(params->src1, params->dest);
  case CMD_INFO:
    return image_info(params->src1);
  default:
    printf("Error: Unknown command\n");
    return ERR_INVALID_COMMAND;
  }
}

int check_files_exist(const char *files[], int count) {
  for (int i = 0; i < count; i++) {
    if (access(files[i], F_OK) != 0) {
      printf("Error: File '%s' not found\n", files[i]);
      return ERR_FILE_NOT_FOUND;
    }
  }
  return 0;
}

int parse_hex_color(const char *str) {
  if (!str) {
    printf("Error: Color string is NULL\n");
    return -1;
  }

  // Skip "0x" or "#" prefix if present
  if (strncmp(str, "0x", 2) == 0) {
    str += 2;
  } else if (str[0] == '#') {
    str++;
  }

  // Check if the remaining string is valid
  size_t len = strlen(str);
  if (len != 6) {
    printf("Error: Color must be a 6-digit hexadecimal value (e.g., FF0000 for "
           "red)\n");
    return -1;
  }

  // Check if all characters are hexadecimal
  for (size_t i = 0; i < len; i++) {
    if (!isxdigit((unsigned char)str[i])) {
      printf("Error: Invalid hexadecimal character in color value\n");
      return -1;
    }
  }

  int color = (int)strtol(str, NULL, 16);
  return color;
}

// Stub implementations for the image processing functions
int image_merge(const char *src1, const char *src2, const char *dest,
                OpRule rule, float w1, float w2) {
  NOTION("Merging images %s and %s to %s using rule %d\n", src1, src2, dest,
         rule);
  
  FILE *fp1 = fopen(src1, "rb");
  FILE *fp2 = fopen(src2, "rb");
  if (fp1 == NULL) {
      WARNING("Error opening file: %s", src1);
      return 1;
      fclose(fp2);
  }
  if (fp2 == NULL) {
    WARNING("Error opening file: %s", src2);
    return 1;
    fclose(fp1);
  }
  IStream is1 = Stream(is1);
  IStream is2 = Stream(is2);
  IStream_fromFileStream(&is1, fp1);
  IStream_fromFileStream(&is2, fp2);
  fclose(fp1);
  fclose(fp2);

  BMPImage lhs = BMPImage_0();
  BMPImage rhs = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&lhs, &is1) != 0 ||
      BMPImage_decode(&rhs, &is2) != 0) {
    WARNING("Failed to decode bmp image!");
    IStream_close(&is1);
    IStream_close(&is2);
    return 1;
  }

  Confuse_args args = {rule == OP_RULE_WEIGHT, w1, w2};
  int ret = BMPImage_confuse(&res, &lhs, &rhs, rule, args);
  if (ret != 0) {
    WARNING("Failed to confuse image!");
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  FILE *fp3 = fopen(dest, "wb");
  if (fp3 == NULL) {
    WARNING("Error opening file: %s", src2);
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  OStream os = Stream(os);
  BMPImage_encode(&res, &os);
  OStream_toFileStream(&os, fp3);
  fclose(fp3);

  BMPImage_release(&res);
  BMPImage_release(&lhs);
  BMPImage_release(&rhs);

  return 0;
}

int image_conv(const char *src1, const char *src2, const char *dest,
                bool is_padding) {
  NOTION("Convolving images %s and %s to %s", src1, src2, dest);
  
  FILE *fp1 = fopen(src1, "rb");
  FILE *fp2 = fopen(src2, "rb");
  if (fp1 == NULL) {
      WARNING("Error opening file: %s", src1);
      return 1;
      fclose(fp2);
  }
  if (fp2 == NULL) {
    WARNING("Error opening file: %s", src2);
    return 1;
    fclose(fp1);
  }
  IStream is1 = Stream(is1);
  IStream is2 = Stream(is2);
  IStream_fromFileStream(&is1, fp1);
  IStream_fromFileStream(&is2, fp2);
  fclose(fp1);
  fclose(fp2);

  BMPImage lhs = BMPImage_0();
  BMPImage rhs = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&lhs, &is1) != 0 ||
      BMPImage_decode(&rhs, &is2) != 0) {
    WARNING("Failed to decode bmp image!");
    IStream_close(&is1);
    IStream_close(&is2);
    return 1;
  }

  int ret = BMPImage_conv(&res, &lhs, &rhs, 1, 1, is_padding);
  if (ret != 0) {
    WARNING("Failed to convolving image!");
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  FILE *fp3 = fopen(dest, "wb");
  if (fp3 == NULL) {
    WARNING("Error opening file: %s", src2);
    BMPImage_release(&res);
    BMPImage_release(&lhs);
    BMPImage_release(&rhs);
    return 1;
  }

  OStream os = Stream(os);
  BMPImage_encode(&res, &os);
  OStream_toFileStream(&os, fp3);
  fclose(fp3);

  BMPImage_release(&res);
  BMPImage_release(&lhs);
  BMPImage_release(&rhs);
  return 0;
}

int image_convert_color(const char *src, const char *dest, const char* mode) {
  NOTION("Converting color mode of %s to %s using mode '%s'\n", src, dest, mode);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_cvtclr(&bimg, mode);
  if (ret != 0) {
    WARNING("Failed to convert color mode of the image!");
    return ret; 
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_fft(const char *src, const char *dest, bool shift) {
  NOTION("Performing FFT on %s to %s with shift=%d\n", src, dest, shift);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);

  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  BMPImage res = BMPImage_0();
  BMPImage_fft_transform(&res, &bimg, shift);
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    perror("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_scale(const char *src, const char *dest, double scaleX, double scaleY)    {
  NOTION("Scaling %s to %s by factor X as %.2f and factor Y as %.2f\n", src, dest, scaleX,
         scaleY);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_scale(&res, &bimg, scaleX, scaleY);
  if (ret != 0) {
    WARNING("Failed to scale the image!");
    return ret;
  }
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_translate(const char *src, const char *dest, int tx, int ty) {
  NOTION("Translating %s to %s by (%d, %d)\n", src, dest, tx, ty);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_translate(&res, &bimg, tx, ty);
  if (ret != 0) {
    WARNING("Failed to scale the image!");
    return ret;
  }
    
  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_rotate(const char *src, const char *dest, double angle) {
  NOTION("Rotating %s to %s by %lf°\n", src, dest, angle);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_rotate(&res, &bimg, angle);
  if (ret != 0) {
    WARNING("Failed to rotate the image!");
    return ret;
  }  

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_flip(const char *src, const char *dest, bool flipX, bool flipY) {
  NOTION("Flipping %s to %s\n", src, dest);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  BMPImage res = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_flip(&res, &bimg, flipX, flipY);
  if (ret != 0) {
    WARNING("Failed to flip the image!");
    return ret;
  } 

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&res, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&res);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  BMPImage_release(&res);
  fclose(fp);
  return 0;
}

int image_add_noise(const char *src, const char *dest, float probability) {
  NOTION("Adding noise to %s and saving to %s with probability %.2f\n", src,
         dest, probability);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_noise_pepper(&bimg, probability);
  if (ret != 0) {
    WARNING("Failed to noise the image!");
    return ret;
  }   

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_to_ascii(const char *src, int color) {
  NOTION("Converting %s to ASCII art with color 0x%06X\n", src,
         color);
  

  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);

  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  char *ascii_str = BMP_asciiart(&bimg);
  if (ascii_str == NULL) {
    WARNING("Failed to convert image to ASCII art!");
    return 1;
  }
  int ansi_clr = hex_to_ansi16(color);
  if (ansi_clr < 8) {
    fprintf(stderr, "\033[%dm", 30 + ansi_clr);
  } else {
    fprintf(stderr, "\033[%dm", 82 + ansi_clr);
  }
  printf("%s\n", ascii_str);
  fprintf(stderr, "\033[0m");
  free(ascii_str);
  BMPImage_release(&bimg);

  return 0;
}

int image_draw_line(const char *src, const char *dest, int color, int x0,
                    int y0, int x1, int y1) {
  NOTION(
      "Drawing line on %s to %s with color 0x%06X from (%d, %d) to (%d, %d)\n",
      src, dest, color, x0, y0, x1, y1);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_line(&bimg, (Line){x0, y0, x1, y1, 5.0}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw line on the image!");
    return ret;
  }   

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_draw_rect(const char *src, const char *dest, int color, int x, int y,
                    int dx, int dy) {
  NOTION("Drawing rectangle on %s to %s with color 0x%06X at (%d, %d) with "
         "size %dx%d\n",
         src, dest, color, x, y, dx, dy);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_rect(&bimg, (Rect){x, y, dx, dy}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw rectangle on the image!");
    return ret;
  }  

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_draw_circle(const char *src, const char *dest, int color, double r,
                      int rx, int ry) {
  NOTION("Drawing circle on %s to %s with color 0x%06X, radius %.2f at center "
         "(%d, %d)\n",
         src, dest, color, r, rx, ry);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_circle(&bimg, (Circle){r, rx, ry}, fromHex(color));
  if (ret != 0) {
    WARNING("Failed to draw circle on the image!");
    return ret;
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_clip(const char *src, const char *dest, int x, int y, int dx,
               int dy) {
  NOTION("Clipping %s to %s from (%d, %d) with size %dx%d\n", src, dest, x, y,
         dx, dy);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  int ret = BMPImage_clip(&bimg, (Rect){x, y, dx, dy});
  if (ret != 0) {
    WARNING("Failed to clip the image!");
    return ret;
  }

  OStream ostream = Stream(ostream);
  if (BMPImage_encode(&bimg, &ostream) != 0) {
    WARNING("Failed to encode bmp image");
    OStream_close(&ostream);
    BMPImage_release(&bimg);
    return 1;
  }

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", dest);
    return 1;
  }
  OStream_toFileStream(&ostream, fp);
  OStream_close(&ostream);
  BMPImage_release(&bimg);
  fclose(fp);
  return 0;
}

int image_to_base64(const char *src, const char *dest) {
  NOTION("Converting %s to base64 and saving to %s\n", src, dest);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }

  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  Stream_toBase64(&istream);

  fp = fopen(dest, "wb");
  if (fp == NULL) {
    NOTION("Error opening file %s", dest);
    return 1;
  }

  OStream_toFileStream(&istream, fp);

  fclose(fp);
  return 0;
}

int image_info(const char *src) {
  NOTION("Displaying information for image %s\n", src);
  
  FILE *fp = fopen(src, "rb");
  if (fp == NULL) {
    WARNING("Error opening file: %s", src);
    return 1;
  }
  IStream istream = Stream(istream);
  IStream_fromFileStream(&istream, fp);
  fclose(fp);
  BMPImage bimg = BMPImage_0();
  if (BMPImage_decode(&bimg, &istream) != 0) {
    WARNING("Failed to decode bmp image");
    IStream_close(&istream);
    return 1;
  }
  IStream_close(&istream);

  BMPImage_dbg(&bimg, stdout);
  BMPImage_release(&bimg);
  return 0;
}