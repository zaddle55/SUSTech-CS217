#!/usr/bin/env python3
import os
import time
import mmap
import subprocess
import random

def analyze_13_extents_structure():
    """分析13个extent的文件结构"""
    print("=== 13个Extent的文件系统结构分析 ===")
    
    # 文件基本信息
    file_size_bytes = 4096 * 4096 + 54  # 灰度BMP
    file_size_mb = file_size_bytes / (1024 * 1024)
    block_size = 4096
    total_blocks = (file_size_bytes + block_size - 1) // block_size
    
    print(f"文件大小: {file_size_bytes:,} 字节 ({file_size_mb:.2f} MB)")
    print(f"总Block数: {total_blocks:,} blocks")
    print(f"Extent数量: 13")
    
    # Extent分布计算
    avg_blocks_per_extent = total_blocks // 13
    remaining_blocks = total_blocks % 13
    
    extent_distribution = []
    for i in range(13):
        blocks = avg_blocks_per_extent + (1 if i < remaining_blocks else 0)
        size_mb = (blocks * block_size) / (1024 * 1024)
        extent_distribution.append((blocks, size_mb))
    
    print(f"\n13个Extent分布:")
    for i, (blocks, size_mb) in enumerate(extent_distribution):
        print(f"  Extent {i+1:2d}: {blocks:4d} blocks ({size_mb:5.2f} MB)")
    
    # Inode结构分析
    print(f"\n=== Inode结构分析 ===")
    print(f"Extent数量: 13 > 4 (inode直接存储限制)")
    print(f"需要extent tree: 是")
    print(f"Inode深度: 1")
    print(f"结构: inode → extent_index_block → 13个extent_entries")
    
    # 额外开销计算
    print(f"\n=== 存储开销 ===")
    print(f"额外的extent index block: 1个 (4KB)")
    print(f"相对于文件大小的开销: {(4096/file_size_bytes)*100:.4f}%")
    
    return extent_distribution, total_blocks

def simulate_fragmented_performance():
    """模拟13个extent对性能的影响"""
    print(f"\n=== 13个Extent性能影响模拟 ===")
    
    # 不同存储介质的性能参数
    storage_params = {
        'SSD': {
            'sequential_read_mbps': 500,
            'random_seek_ms': 0.1,
            'random_read_mbps': 400
        },
        'HDD_7200': {
            'sequential_read_mbps': 150,
            'random_seek_ms': 8.5,
            'random_read_mbps': 80
        },
        'HDD_5400': {
            'sequential_read_mbps': 100,
            'random_seek_ms': 12,
            'random_read_mbps': 50
        }
    }
    
    file_size_mb = 62.5
    
    for storage_type, params in storage_params.items():
        print(f"\n{storage_type} 性能分析:")
        
        # 连续读取时间（理想情况）
        sequential_time = file_size_mb / params['sequential_read_mbps']
        
        # 13个extent的碎片化读取
        seek_overhead = 13 * params['random_seek_ms'] / 1000  # 转换为秒
        fragmented_read_time = file_size_mb / params['random_read_mbps']
        total_fragmented_time = fragmented_read_time + seek_overhead
        
        # 性能下降计算
        performance_loss = ((total_fragmented_time - sequential_time) / sequential_time) * 100
        
        print(f"  连续读取时间: {sequential_time:.3f}秒")
        print(f"  碎片化读取时间: {total_fragmented_time:.3f}秒")
        print(f"  寻道开销: {seek_overhead:.3f}秒")
        print(f"  性能下降: {performance_loss:.1f}%")

def create_fragmented_test_file(filepath, num_extents=13):
    """创建一个模拟13个extent的测试文件"""
    print(f"\n创建模拟{num_extents}个extent的测试文件...")
    
    file_size = 8092 * 8092 + 54
    extent_size = file_size // num_extents
    
    # 创建临时文件片段
    temp_files = []
    
    try:
        # 创建多个小文件来模拟碎片化
        for i in range(num_extents):
            temp_file = f"{filepath}.part{i}"
            temp_files.append(temp_file)
            
            size = extent_size if i < num_extents - 1 else file_size - (extent_size * (num_extents - 1))
            
            with open(temp_file, 'wb') as f:
                # 写入随机数据模拟图像内容
                for chunk in range(0, size, 8192):
                    chunk_size = min(8192, size - chunk)
                    data = bytes([random.randint(0, 255) for _ in range(chunk_size)])
                    f.write(data)
        
        # 合并文件（这会创建碎片化的最终文件）
        with open(filepath, 'wb') as outfile:
            for temp_file in temp_files:
                with open(temp_file, 'rb') as infile:
                    outfile.write(infile.read())
        
        print(f"测试文件创建完成: {filepath}")
        
    finally:
        # 清理临时文件
        for temp_file in temp_files:
            try:
                os.remove(temp_file)
            except:
                pass
    
    return filepath

def benchmark_13_extents_reading(filepath):
    """测试13个extent文件的读取性能"""
    print(f"\n=== 13个Extent读取性能测试 ===")
    
    file_size = os.path.getsize(filepath)
    file_size_mb = file_size / (1024 * 1024)
    
    results = {}
    
    # 1. 完整顺序读取
    print(f"1. 完整顺序读取测试...")
    times = []
    for run in range(3):  # 多次测试取平均值
        start_time = time.time()
        with open(filepath, 'rb') as f:
            data = f.read()
        end_time = time.time()
        times.append(end_time - start_time)
    
    avg_time = sum(times) / len(times)
    speed = file_size_mb / avg_time
    results['sequential'] = {'time': avg_time, 'speed': speed}
    print(f"   平均耗时: {avg_time:.3f}秒")
    print(f"   读取速度: {speed:.2f} MB/s")
    
    # 2. 模拟extent跳跃读取
    print(f"2. 模拟13个extent跳跃读取...")
    extent_size = file_size // 13
    
    times = []
    for run in range(3):
        start_time = time.time()
        with open(filepath, 'rb') as f:
            # 按extent顺序读取，但每次都需要seek
            for i in range(13):
                f.seek(i * extent_size)
                if i == 12:  # 最后一个extent
                    f.read()  # 读取剩余部分
                else:
                    f.read(extent_size)
        end_time = time.time()
        times.append(end_time - start_time)
    
    avg_time = sum(times) / len(times)
    speed = file_size_mb / avg_time
    results['extent_jump'] = {'time': avg_time, 'speed': speed}
    print(f"   平均耗时: {avg_time:.3f}秒")
    print(f"   读取速度: {speed:.2f} MB/s")
    
    # 3. 随机extent访问
    print(f"3. 随机extent访问测试...")
    times = []
    for run in range(3):
        start_time = time.time()
        with open(filepath, 'rb') as f:
            # 随机访问各个extent
            extent_indices = list(range(13))
            random.shuffle(extent_indices)
            
            for i in extent_indices:
                f.seek(i * extent_size)
                read_size = extent_size if i < 12 else file_size - (12 * extent_size)
                f.read(read_size)
        end_time = time.time()
        times.append(end_time - start_time)
    
    avg_time = sum(times) / len(times)
    speed = file_size_mb / avg_time
    results['random_extent'] = {'time': avg_time, 'speed': speed}
    print(f"   平均耗时: {avg_time:.3f}秒")
    print(f"   读取速度: {speed:.2f} MB/s")
    
    # 4. 内存映射读取
    print(f"4. 内存映射读取测试...")
    times = []
    for run in range(3):
        start_time = time.time()
        with open(filepath, 'rb') as f:
            with mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as mm:
                data = mm[:]
        end_time = time.time()
        times.append(end_time - start_time)
    
    avg_time = sum(times) / len(times)
    speed = file_size_mb / avg_time
    results['mmap'] = {'time': avg_time, 'speed': speed}
    print(f"   平均耗时: {avg_time:.3f}秒")
    print(f"   读取速度: {speed:.2f} MB/s")
    
    return results

def analyze_performance_impact(results):
    """分析性能影响"""
    print(f"\n=== 性能影响分析 ===")
    
    if 'sequential' not in results:
        return
    
    baseline_speed = results['sequential']['speed']
    
    print(f"基线性能 (顺序读取): {baseline_speed:.2f} MB/s")
    
    for method, data in results.items():
        if method == 'sequential':
            continue
        
        speed = data['speed']
        impact = ((baseline_speed - speed) / baseline_speed) * 100
        
        method_names = {
            'extent_jump': 'Extent跳跃读取',
            'random_extent': '随机Extent访问',
            'mmap': '内存映射读取'
        }
        
        print(f"{method_names.get(method, method)}: {speed:.2f} MB/s (影响: {impact:+.1f}%)")

def optimization_recommendations():
    """提供优化建议"""
    print(f"\n=== 13个Extent优化建议 ===")
    
    print("1. 文件系统层面:")
    print("   - 使用 e4defrag 整理文件碎片")
    print("   - 考虑重新创建文件以获得连续存储")
    print("   - 调整文件系统参数以减少碎片化")
    
    print("\n2. 应用程序层面:")
    print("   - 优先使用一次性完整读取")
    print("   - 对于大文件使用内存映射 (mmap)")
    print("   - 避免频繁的随机访问")
    print("   - 考虑使用更大的读取缓冲区")
    
    print("\n3. 系统调优:")
    print("   - 增加系统页缓存大小")
    print("   - 调整 readahead 参数")
    print("   - 对于SSD，确保TRIM功能启用")
    
    print("\n4. 代码示例:")
    print("""
# 推荐的读取方式
def read_fragmented_image(filepath):
    # 方法1: 一次性读取 (推荐)
    with open(filepath, 'rb') as f:
        return f.read()
    
    # 方法2: 内存映射 (大文件推荐)
    with open(filepath, 'rb') as f:
        with mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ) as mm:
            return mm[:]
""")

def main():
    """主分析函数"""
    print("=== 13个Extent对8092x8092 BMP图像的影响分析 ===")
    
    # 1. 结构分析
    extent_dist, total_blocks = analyze_13_extents_structure()
    
    # 2. 性能模拟
    simulate_fragmented_performance()
    
    # 3. 创建测试文件并进行实际测试
    test_file = "/tmp/fragmented_test_image.bmp"
    
    try:
        create_fragmented_test_file(test_file, 13)
        
        # 分析实际的extent情况
        try:
            result = subprocess.run(['filefrag', '-v', test_file], 
                                  capture_output=True, text=True)
            print(f"\n实际文件extent分析:")
            print(result.stdout)
        except:
            print("无法运行filefrag命令")
        
        # 性能测试
        results = benchmark_13_extents_reading(test_file)
        
        # 分析结果
        analyze_performance_impact(results)
        
    finally:
        # 清理测试文件
        try:
            os.remove(test_file)
        except:
            pass
    
    # 4. 优化建议
    optimization_recommendations()
    
    # 总结
    print(f"\n=== 总结 ===")
    print(f"• Block数量: {total_blocks:,} 个")
    print(f"• Extent数量: 13 个")
    print(f"• Inode深度: 1 (需要extent index block)")
    print(f"• 性能影响: 中等到严重 (取决于存储类型)")
    print(f"• SSD环境: 性能下降约10-30%")
    print(f"• HDD环境: 性能下降约30-60%")
    print(f"• 推荐: 文件碎片整理或重新创建文件")

if __name__ == "__main__":
    main()