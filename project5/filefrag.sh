#!/bin/bash

# 快速验证碎片化对I/O影响的脚本
# 基于你已有的碎片化环境

echo "=== 快速碎片化I/O性能验证 ==="
echo "测试时间: $(date)"

# 测试目录
TEST_DIR="/tmp/frag_quick_test"
mkdir -p $TEST_DIR
cd $TEST_DIR

# 创建测试函数
test_io_performance() {
    local file=$1
    local label=$2

    echo -e "\n--- 测试 $label ---"
    echo "文件: $file"

    # 显示碎片信息
    echo "Fragment信息:"
    filefrag -v $file | head -10
    extents=$(filefrag $file | grep -o '[0-9]* extent' | cut -d' ' -f1)
    echo "总计: $extents extents"

    # 清空缓存
    sync
    echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || true

    # 读取性能测试 (3次平均)
    echo -e "\n读取性能测试:"
    total_time=0
    for i in {1..3}; do
        echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || true
        start_time=$(date +%s.%N)
        dd if=$file of=/dev/null bs=4096 2>/dev/null
        end_time=$(date +%s.%N)
        duration=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $duration" | bc -l)
        echo "  第${i}次: ${duration}秒"
    done
    avg_read_time=$(echo "scale=6; $total_time / 3" | bc -l)
    echo "  平均读取时间: ${avg_read_time}秒"

    # 写入性能测试 (复制文件)
    echo -e "\n写入性能测试:"
    total_time=0
    for i in {1..3}; do
        echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || true
        start_time=$(date +%s.%N)
        dd if=$file of=${file}.copy_$i bs=4096 2>/dev/null
        # sync
        end_time=$(date +%s.%N)
        duration=$(echo "$end_time - $start_time" | bc -l)
        total_time=$(echo "$total_time + $duration" | bc -l)
        echo "  第${i}次: ${duration}秒"
        rm -f ${file}.copy_$i
    done
    avg_write_time=$(echo "scale=6; $total_time / 3" | bc -l)
    echo "  平均写入时间: ${avg_write_time}秒"

    # 文件大小信息
    file_size=$(stat -c%s $file)
    file_size_mb=$(echo "scale=2; $file_size / 1048576" | bc -l)

    # 计算吞吐量
    read_throughput=$(echo "scale=2; $file_size_mb / $avg_read_time" | bc -l)
    write_throughput=$(echo "scale=2; $file_size_mb / $avg_write_time" | bc -l)

    echo -e "\n性能摘要:"
    echo "  文件大小: ${file_size_mb} MB"
    echo "  Extents数量: $extents"
    echo "  读取吞吐量: ${read_throughput} MB/s"
    echo "  写入吞吐量: ${write_throughput} MB/s"
    echo "  读写比率: $(echo "scale=2; $avg_read_time / $avg_write_time" | bc -l)"
}

# 第一步: 创建连续文件进行基线测试
echo -e "\n=== 创建连续文件基线 ==="

# 创建不同大小的连续文件
dd if=/dev/zero of=baseline_1mb.bmp bs=1048576 count=1 2>/dev/null
dd if=/dev/zero of=baseline_4mb.bmp bs=1048576 count=4 2>/dev/null  
dd if=/dev/zero of=baseline_16mb.bmp bs=1048576 count=16 2>/dev/null
dd if=/dev/zero of=baseline_64mb.bmp bs=1048576 count=64 2>/dev/null

sync
echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || true

# 测试连续文件性能
test_io_performance "baseline_1mb.bmp" "连续文件 1MB"
test_io_performance "baseline_4mb.bmp" "连续文件 4MB"  
test_io_performance "baseline_16mb.bmp" "连续文件 16MB"
test_io_performance "baseline_64mb.bmp" "连续文件 64MB"

# 第二步: 创建碎片化环境
echo -e "\n=== 创建碎片化环境 ==="

# 创建大量小文件
echo "创建碎片化模式..."
for i in {1..1000}; do
    dd if=/dev/zero of=spacer_$i.tmp bs=8192 count=1 2>/dev/null
done

# 删除一半文件制造空洞
for i in {2..1000..2}; do
    rm -f spacer_$i.tmp
done

echo "碎片化环境创建完成"

# 第三步: 在碎片化环境中创建相同大小的文件
echo -e "\n=== 在碎片化环境中创建文件 ==="

dd if=/dev/zero of=fragmented_1mb.bmp bs=1048576 count=1 2>/dev/null
dd if=/dev/zero of=fragmented_4mb.bmp bs=1048576 count=4 2>/dev/null
dd if=/dev/zero of=fragmented_16mb.bmp bs=1048576 count=16 2>/dev/null  
dd if=/dev/zero of=fragmented_64mb.bmp bs=1048576 count=64 2>/dev/null

sync
echo 3 > /proc/sys/vm/drop_caches 2>/dev/null || true

# 测试碎片化文件性能
test_io_performance "fragmented_1mb.bmp" "碎片化文件 1MB"
test_io_performance "fragmented_4mb.bmp" "碎片化文件 4MB"
test_io_performance "fragmented_16mb.bmp" "碎片化文件 16MB"
test_io_performance "fragmented_64mb.bmp" "碎片化文件 64MB"

# 第四步: 对比分析
echo -e "\n=== 碎片化影响对比分析 ==="

echo "Fragment数量对比:"
echo "连续文件:"
for file in baseline_*.bmp; do
    extents=$(filefrag $file | grep -o '[0-9]* extent' | cut -d' ' -f1)
    size=$(echo $file | sed 's/.*_\([0-9]*\)mb.*/\1/')
    echo "  ${size}MB: $extents extents"
done

echo -e "\n碎片化文件:"  
for file in fragmented_*.bmp; do
    extents=$(filefrag $file | grep -o '[0-9]* extent' | cut -d' ' -f1)
    size=$(echo $file | sed 's/.*_\([0-9]*\)mb.*/\1/')
    echo "  ${size}MB: $extents extents"
done

# 第五步: 清理
echo -e "\n=== 清理测试文件 ==="
echo "测试完成，是否清理测试文件? (y/N)"
read -r response
if [[ "$response" =~ ^[Yy]$ ]]; then
    cd /
    rm -rf $TEST_DIR
    echo "测试文件已清理"
else
    echo "测试文件保留在: $TEST_DIR"
fi

echo -e "\n=== 测试总结 ==="
echo "1. 对比连续文件和碎片化文件的extent数量差异"
echo "2. 观察读取/写入性能随碎片化程度的变化"
echo "3. 分析不同文件大小下碎片化的影响程度"
echo "4. 验证小文件vs大文件在碎片化环境下的性能表现"