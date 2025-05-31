#import "handout.typ": *

#show: handout.with(
  title: "Project 5: A comprehensive comparison of image processing",
  subtitle: "CS217 Advanced Programming Spring 2025",
  author: "zaddle"
)

#outline()

= 实验总览

在#link("https://github.com/zaddle55/SUSTech-CS217/tree/sync(project4)/project4", [project4])中，我们已经搭建了一个比较完善的小型图像处理库，利用大量C++的新特性对模块进行改进与封装。
而编写出单单按预期运行的代码并不能算是完成了所有的工作，我们希望这个处理库有着不输于现有实现框架的表现。
因此，在这个实验中，我们将跨语言地对比使用C++实现与Rust乃至Python等的流行的图像处理库的性能。

= Experiment 1: BMP图像读写对比

无论要进行哪种处理方式，对图像的文件读写都是必不可少的步骤。
目前机器学习中流行的图像数据集如ImageNet[cite here], CIFAR-10[cite here]都有着数万级别的训练图像和实例。
如果是这种数量级的输入，读写时带来的性能损耗就无法忽略了。
为了考察各语言图像编解码过程的时空占用情况，我们将开展不同大小、不同通道数的输入图像的读写基准测试。
并尝试分析该过程的跨语言性能差异。

虽然我们在#link("https://github.com/zaddle55/SUSTech-CS217/tree/sync(project4)/project4", [project4])完全支持`PNG`,`JPEG`图像的读写，然而各标准库使用的DCT变换算法可能存在差异而引入测试噪声。
因此我们仅对最直接的`BMP`文件开展测试。

== Rust图像读取/写入

在Rust的测试中，我们考虑使用最流行的图像处理库`image-rs`作为基线来比较。
我们会用到两个模块：`ImageReader`和`DynamicImage`。
使用`ImageReader`读取图像文件的用例如下：
```rust
use image::{ImageReader};

fn main() {
    let fname: String = /* Path to the image file */;
    let img = ImageReader::open(fname)
        .expect("Failed to open image")
        .decode()
        .expect("Failed to decode image");
}
```

而`DynamicImage`用来产生指定大小的供写入的BMP图像，由于像素数据值对读写的影响较小，我们可以全部使用白色(255)来填充，以下是创建并保存一个4096x4096的白色灰度阶图像的代码：
```rust
use image::{DynamicImage, Rgba};

fn main() {
    let img = DynamicImage::new_luma8(4096, 4096);
    for x in 0..4096 {
        for y in 0..4096 {
            img.put_pixel(x, y, Rgba([255, 255, 255, 255]));
        }
    }
    img.save("output.bmp")
        .expect("Failed to save image");
}
```

== Python图像读取/写入

在Python中, `OpenCV`提供了更加简洁的接口来读取和写入图像。
根据官方文档，`cv2.imread()`函数可以读取图像文件并返回一个NumPy数组, 而`cv2.imwrite()`函数可以将NumPy数组保存为图像文件。
它们在文档里的描述如下：

#figure(image("image.png", width: 400pt), caption: "cv2.imread()")
#figure(image("image-1.png", width: 400pt), caption: "cv2.imwrite()")

由于我们的图像处理库的读写函数就是参照`OpenCV`设计的，因此上手起来也非常容易。

== 实验设计

为了尽可能全面地衡量各个语言在这个过程中的性能，我们将对图像形状参数从如下三个角度进行变化：

1. 图像的宽高: 读写的图像大小会从`128x128`到`8192x8192`，每次倍增其宽度和高度。

进行这种变化的原因比较好理解, 我们从project3剖析了BMP图像的存储格式和解析方法可以知道, 解析像素值时会在列与行上进行遍历。
因此预期读写的时间复杂度是$O(N^2)$, 而常数因子会与各种语言的实现方式有关, 这也是我们对比的重点。

2. 图像的通道数: 鉴于我们在project4中实现了对`RGB`,`RGBA`, `Luma`等不同位深度图像的支持。
我们将对比 $1$, $3$, $4$ 三种不同通道数的图像读写性能.

3. 图像的宽高比: 读写的图像大小固定为`1080x1080`, 我们将对比 `240:4860`, `360:3240`, `720:1620`, `810:1440`, `1080:1080`一系列宽高比的图像读写.

== 实验方法

整个实验会在我的电脑上进行基准测试，硬件配置与环境如下：

#align(center)[
  #table(
    fill: (x, _) =>
      if calc.odd(x) { luma(240) }
      else { white },
    align: (x, y) =>
      if y == 0 { center }
      else if x == 0 { center }
      else { left },
    columns: 2,
    [*Item*], [*Details*],
  [System], [6.8.0-52-generic Ubuntu 22.04.1 LTS],
  [OS Kernel], [5.15.167.4-microsoft-standard-WSL2],
  [gcc-version], [(Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0],
  [g++-version], [(Ubuntu 9.4.0-1ubuntu1~20.04.2) 9.4.0],
  [cargo-version], [1.86.0 (adf9b6ad1 2025-02-28)],
  [rustc-version], [1.86.0 (05f9846f8 2025-03-31)],
  [conda-version], [24.11.3],
  [python-env-version], [3.13.2],
  [CPU Intel(R) Core(TM)], [i5-13600KF CPU 5.10GHz],
  [CPU-cores], [14 cores],
  [L1-cache-size], [72 KB],
  [L2-cache-size], [2048 KB],
  [L3-cache-size], [24576 KB],
  [Memory], [31.968 GB],
)]

基准测试库我们分别选择的是

- C++: `Google Benchmark`[cite here]
- Rust: `Criterion`[cite here]
- Python: `pytest-benchmark`[cite here]

其中`Google Benchmark`和`Criterion`我们在前面的project有所提及, 这次的使用也将是相似的流程.
而`pytest-benchmark`和两个基准测试库比较类似, 它使用独有的装饰器接口`@pytest.mark.parametrize`来参数化测试函数, 并在函数中使用`benchmark`对象来进行多次基准测试,
在报告中也会输出详细的统计数据.

#pagebreak()

== 实验结果

=== 图像大小探究

#grid(
  align: center,
  columns: 2,
  row-gutter: 20pt,
  [#figure(image("results/bench_io_encodeNxN.png", width: 230pt), caption: "单通道写入")],
  [#figure(image("results/bench_io_decodeNxN.png", width: 230pt), caption: "单通道读取")],
  grid.cell(
    align: center,
    colspan: 2,
    [#figure(image("results/bench_io_encode_decodeNxN.png", width: 400pt), caption: "单通道读写综合对比, 实线为写入, 虚线为读取")],
  )
)

就读/写其中一个操作而言, 我们可以看到运行时间整体趋势都是以$O(N^2)$的形式增长, 这说明三种语言的实现方式在遍历像素点时都遵循了相似的逻辑.
以语言类别纵向比较, 可以看出常数因子是按`C++ > Rust > Python`的排序的, 这说明opencv-python在三者中具有最好的表现.

而在读写综合对比中, 无论哪种实现方式，都呈现小尺寸下读取速度快于写入速度, 在较大尺寸则发生逆转的现象。
对于不同语言, 对应的运行时间交叉点的到来也是不同的:
C++在小于 $2048 times 2048$ 尺寸时读取运行时间远小于写入, 而在 $2048 times 2048$时则发生逆转;
Rust在大于 $256 times 256$ 尺寸时就发生了逆转;
而Python始终差距较小, 读取和写入的运行时间几乎相同.


// bench_io_encodeNc/decodeNc
#pagebreak()

=== 图像通道数探究

#grid(
  align: center,
  columns: 2,
  row-gutter: 20pt,
  [#figure(image("results/bench_io_encodeNc.png", width: 230pt), caption: "多通道写入")],
  [#figure(image("results/bench_io_decodeNc.png", width: 230pt), caption: "多通道读取")],
  grid.cell(
    align: center,
    colspan: 2,
    [#figure(image("results/bench_io_encode_decodeNc.png", width: 400pt), caption: "多通道读写综合对比, 实线为写入, 虚线为读取")],
  )
)

在图像通道数的对比中, 似乎并不是所有实现的运行时间都随着通道数的增加而增加。
只有Python的线性程度最好; 而C++在单通道时运行速度异常地慢, 在3, 4通道时才表现出线性增长的趋势;
而对于Rust, 其三通道写入速度最慢, 其平均运行时间甚至来到了80ms左右, 这个将会是我们后续反思的重心之一.

// bench_io_encodeNxM/decodeNxM
#pagebreak()

=== 图像宽高比探究

#grid(
  align: center,
  columns: 2,
  row-gutter: 20pt,
  [#figure(image("results/bench_io_encodeNxM.png", width: 230pt), caption: "不同宽高比写入")],
  [#figure(image("results/bench_io_decodeNxM.png", width: 230pt), caption: "不同宽高比读取")],
  grid.cell(
    align: center,
    colspan: 2,
    [#figure(image("results/bench_io_encode_decodeNxM.png", width: 400pt), caption: "不同宽高比读写综合对比, 实线为写入, 虚线为读取")],
  )
)

整体来看, 随着图像宽高比的变化, 运行时间的还是有一定的波动.
其中读取操作的波动较小, 但写入操作的波动较大,
其中值得关注的是, C++实现在$240 times 4860$时明显地偏离了水平渐进线, 这有可能是解码到行主序的BMP图像时, 数据局部性没有被很好的利用 (作为参照, L1 Dcache: 48KiB), 导致了缓存未命中率的增加.

== 内存占用

我们同样利用Vtune的Memory Consumption分析工具对各语言的内存占用情况进行了分析, 收集到的数据如下:

#figure(image("截图 2025-05-30 23-49-42.png", width: 400pt), caption: "C++程序读写BMP图像的内存占用")

#figure(image("截图 2025-05-30 23-38-03.png", width: 400pt), caption: "Rust程序读写BMP图像的内存占用")

== 实验思考

我们考察了各语言在BMP图像读写过程中的性能差异, 发现python的OpenCV库在大多数情况下的确表现出色, Rust也展示出了比较优秀的时间复杂度.

为了细致分析各语言的实现方式, 我将前两者的代码调用途径列出如下:

#figure(image("image-3.png", width: 350pt), caption: "C++的图像读写调用途径")

#figure(image("image-6.png", width: 400pt), caption: "Rust的图像读写调用途径")

三者的核心解析函数逻辑都比较相似, 都是按位图信息来选择进入不同的分支, 以处理不同的图像通道数.
而一旦确定了通道数, 就会从自建的buffer中读出字节流, 进入在列与行上遍历像素点的逻辑;
然而从此开始出现了些微不同:

opencv-python的会将行上的遍历转化为`memcpy`的直接调用.
`memcpy`作为linux下的标准库函数, 其实现是基于平台高度优化的, 基本上可以认为是最优的.
通过预先计算每行的字节数, OpenCV可以直接将每行的像素数据拷贝到目标图像中, 这使得省去了逐个像素点赋值的开销, 因而在众实现中有最优的表现.

Rust的`image-rs`库则是借助`with_rows`接口来进行流式处理, 它的定义是这样的:

```rust
fn with_rows<F>(
    buffer: &mut [u8],
    width: i32,
    height: i32,
    channels: usize,
    top_down: bool,
    mut func: F,
) -> io::Result<()>
where
    F: FnMut(&mut [u8]) -> io::Result<()>,
{
    // An overflow should already have been checked for when this is called,
    // though we check anyhow, as it somehow seems to increase performance slightly.
    let row_width = channels.checked_mul(width as usize).unwrap();
    let full_image_size = row_width.checked_mul(height as usize).unwrap();
    assert_eq!(buffer.len(), full_image_size);

    if !top_down {
        for row in buffer.chunks_mut(row_width).rev() {
            func(row)?;
        }
    } else {
        for row in buffer.chunks_mut(row_width) {
            func(row)?;
        }
    }
    Ok(())
}
```

这个函数实际上也是执行了一个$O(N)$的循环, 但它的实现方式是将每行的像素数据作为一个整体传递给回调函数, 这使得Rust的实现可以在行级别上进行优化, 但很显然, 这种方式的开销还是比不上系统底层函数的`memcpy`.

因此初步的结论是, 在图像读写的过程中, OpenCV库由于使用了高度优化的`memcpy`函数, 在性能上有着明显的优势.


== Critical Thinking 1: 图像读取/写入性能反转

在之前的实验中, 我们观察到C++和Rust的图像读取与写入性能在不同尺寸的图像上有着明显的反转现象.
在小尺寸图像上, 读取的性能明显优于写入, 而在大尺寸图像上则发生了逆转.
在子实验1中这个现象极为明显:

#figure(image("results/bench_io_encode_decodeNxN.png", width: 400pt), caption: "单通道读写综合对比, 实线为写入, 虚线为读取")

我们再来回顾一下BMP图像的存储格式, 它是以行主序的方式线性存储像素数据的,
并且通过信息头解析来提前获取图像的宽度和高度, 以及每行的字节数.
不需要像JPEG等格式那样需要进行复杂的DCT变换. 这些特点也带来了这一特别现象的出现.
为了深入分析涉及I/O操作的机制, 我们需要了解Linux的文件系统和磁盘I/O的工作原理.

=== 文件系统与磁盘I/O

我们知道, 现代Linux采用的文件系统是一个叫做`ext4`的日志文件系统, 使用命令
```sh df -T -h```可以查看当前所有挂载的文件系统的类型.

```txt
文件系统       类型      大小  已用  可用 已用% 挂载点
tmpfs          tmpfs     3.2G  2.5M  3.2G    1% /run
/dev/nvme0n1p8 ext4       28G   27G     0  100% /
tmpfs          tmpfs      16G  440M   16G    3% /dev/shm
tmpfs          tmpfs     5.0M  4.0K  5.0M    1% /run/lock
efivarfs       efivarfs  192K  152K   36K   82% /sys/firmware/efi/efivars
tmpfs          tmpfs      16G     0   16G    0% /run/qemu
/dev/nvme0n1p9 ext4       61G   47G   12G   81% /home
/dev/nvme0n1p1 vfat      300M   38M  263M   13% /boot/efi
tmpfs          tmpfs     3.2G   32M  3.1G    1% /run/user/1000
/dev/sda1      ntfs3     954G  520G  435G   55% /media/zaddle/Zaddle
```

我们的图像和代码都存储在`/home`分区上, 其文件系统类型也确实是`ext4`.
作为Linux下高性能文件管理的解决方案, `ext4`采用Index Node来串联文件的元数据和数据块, 并且以树的形式组织最小的文件块, 以提高随机访问的性能, inode组织的文件系统拓扑结构如下:

#image("image-4.png")

在这种文件系统中, 文件的读写操作实际上是对磁盘块的随机访问, 而磁盘块的大小通常为4KiB,
可以预料到如果图像的尺寸较小, 读取操作会涉及到较少的磁盘块, 因此读取速度会更快，相反, 如果图像尺寸较大, 读取操作会涉及到二级甚至三级文件间接寻址, 从而引入额外的I/O开销.

很显然这些是不利于一个优秀的文件系统的需求的, 为了提高随机访问的性能, `ext4`发展了更多技术来消除这种尺寸效应, `extents` 就是其中之一.
把extents实际上可以理解为使用B+树来将多个`ext4`文件系统的块打包成一个连续的范围, 这样可以显著减少文件的碎片化, 从树的结构来看, `extents`是这样的:

#image("image-5.png")

当然, 为了实现这样一个机制, 内核中有关`extents`的代码相当复杂, 在此我们不再展开讨论, 不过说了这么多, `extents` 和我们实验中出现的现象有什么关系呢?

可以肯定的是, 如果一个文件尺寸比较合适, 它的所有数据都可以存储在一个`extent`树中, 一次I/O操作就可以完成对整个文件的读取; 但是如果这个文件尺寸比较大, 而且不幸地它的数据布在多个`extent`中, 那么读取这个文件就需要额外的时间惩罚, 而这我们是所不希望的.
好在, linux下有`filefrag`工具可以帮助我们分析文件的碎片化情况, 通过运行`filefrag -v <file>`就可以查看指定文件的详细碎片化情况.以下为对我们实验中使用的`1c4096x4096.bmp`文件的分析结果:

```txt
Filesystem type is: ef53
File size of /home/zaddle/code/SUSTech-CS217/project5/img/1c4096x4096.bmp is 16778294 (4097 blocks of 4096 bytes)
 ext:     logical_offset:        physical_offset: length:   expected: flags:
   0:        0..    2047:    5376000..   5378047:   2048:            
   1:     2048..    4095:    5382144..   5384191:   2048:    5378048:
   2:     4096..    4096:    2256896..   2256896:      1:    5384192: last,eof
/home/zaddle/code/SUSTech-CS217/project5/img/1c4096x4096.bmp: 3 extents found
```

`filefrag`报告这个文件有3个`extent`, 其中第一个`extent`的长度为$2048 times 4"KiB" = 8"MiB"$, 第二个`extent`的长度也是$2048 times 4"KiB" = 8"MiB"$, 而第三个`extent`的长度仅为$4"KiB"$.
可以预想到, 从$4096 times 4096$大小的灰度图像($approx$ 16 MiB)开始, 一个`BMP`图像的尺寸会超过一个`extent`的大小, 因此产生了算法复杂度外的系统层面的代价, 这也就是为什么在大尺寸图像上, 读取的性能会逐渐变差的原因. 事实上, $8192 times 8192$ 图像的`extents`数量已经到了9个, 对于依赖于底层C库的C++和Rust实现来说, 带来的额外的I/O开销是非常明显且无法忽视的.

为了验证这个猜想, 我们可以编写一个脚本简单模拟文件严重碎片化和普通状况下的读写s时间差异, 通过对比`filefrag`的输出结果来验证我们的猜想.

#table(
  fill: (x, _) =>
    if calc.odd(x) { luma(240) }
    else { white },
  align: (x, y) =>
    if y == 0 { center }
    else if x == 0 { center }
    else { left },
  columns: 6,
  [*File size*], [*extents*], [*Continuous File Read (s)*], [*Continuous File Write*], [*Fragmented File Read*], [*Fragmented File Write*],
  [1MiB], [1/1], [0.004394], [0.005308], [0.004921], [0.005001],
  [4MiB], [1/5], [0.005995], [0.007913], [0.006547], [0.008341],
  [16MiB], [10/29], [0.013974], [0.019146], [0.018030], [0.014073],
  [64MiB], [70/95], [0.040423], [0.052493], [0.042200], [0.040437],
)

我们可以清晰地看到, 在正常情况下, 读取时间是普遍小于写入时间的, 这也符合我们对文件系统的预期.
然而对于高度碎片化的文件, 读取的优势就不复存在了, 甚至在16MiB的文件上, 读取时间已经大于写入时间了.
这无疑印证了我们之前的猜想, 在大尺寸图像上, 读取性能偏低的原因确实是系统层面的I/O开销导致的.
具体来说, 超过一个`extent`大小(2048 MiB)的图像文件都会一定程度上受到这种影响而偏离了$O(N^2)$的时间复杂度.
不过不仅如此, 还有其他因素也一起促成了这种结果.
在进行上述实验的过程中, 一开始的写入测量实际上是偏离预期的, 经过一些资料的查阅和操作系统课上学过的知识, 我发现了原因: 在Linux下, 写入操作会触发文件系统的`write-back`机制, 这会导致写入操作的延迟, 而读取操作则不会受到这种影响.
而我们只有显式调用`sync`命令才能及时将页缓存写入磁盘, 模拟出正确的结果.

`write-back`机制的工作原理是这样的: 一般情况下Linux的write系统调用是把数据写到page cache，随后会有专门的内核线程把
page cache里的脏数据写回到持久存储设备上。当脏数据超过一定的比例时，write系统调
用会同步等待一定的脏数据回写完成.
也就是说, 如果我们不进行主动的脏页写回, 那么实际的写入操作会被延迟到下一次的`write-back`周期, 这就导致了我们在测量写入时间时, 实际上测量的只是缓存到Physical Memory的时间, 而不是实际写入磁盘的时间.
在这种机制下, 大尺寸图像的写入也不会因为数据规模的增加而导致明显的性能下降.

当然, Linux在磁盘内容的读写上还有更多的优化, 例如`readahead`机制, 它会在读取文件时预先加载后续的数据块到内存中, 以减少磁盘I/O的延迟; 以及`multi-block allocator`也是为了提高大文件的读写性能而设计的, 它会在分配磁盘块时尽量将相邻的数据块分配在一起, 以减少磁盘寻址的开销.
不过受限于没有有效的测量方案, 我们也很难估计他们对于反转现象的贡献, 因此在此不再展开讨论.

== Critical Thinking 2: Python JIT

实际上, 在初次跑python的基准测试时, 我发现第一个子实验$128 times 128$的encoding时间反常得高(2300us), 甚至比同测试的$512 times 512$还要高的多, 在我认定只是偶然的机器浮动误差时, 又连着进行了几次测试, 结果依然是如此.
这让我感到非常困惑, 因为尺寸越小, 读取和写入的时间应该越短才对.
为了定位出实验设计的错误, 我在`pytest-benchmark`的报告中再次查看了每次测试的详细信息, 发现了Standrard Deviation的值异常地高, 这说明这个测试的结果并不稳定.

根据AI project的经验, python的numba模块在操作时也有类似的问题, 当函数体第一次被调用时, 它会进行类似Java JIT编译以提高后续的运行速度.
但这个预编译的过程会带来额外的开销, 因此在第一次调用时, 运行时间会异常地高.
所以我在所有测试前注册了一个预热函数, 以确保所有的函数都已经被JIT热编译过了.

```python
# warmup
@pytest.fixture(scope='module', autouse=True)
def warmup():
    mat = np.ones((100, 100, 3), dtype=np.uint8) * 255
    cv2.imwrite(f"{IMG_DIR}warmup.bmp", mat)
```

此时再度运行基准测试, 发现所有的测试结果都恢复到了正常的水平, 这也说明了Python JIT编译的确会对性能测试带来影响.

#table(
  fill: (x, _) =>
    if calc.odd(x) { luma(240) }
    else { white },
  align: (x, y) =>
    if y == 0 { center }
    else if x == 0 { center }
    else { left },
  columns: 3,
  [*Test*], [*Mean Time (us)*], [*StdDev*],
  [Run without warmup], [2300], [$gt 1000$],
  [Run with warmup], [100], [1.4272]
)

= Experiment 2: 滤波性能比较

滤波实际上是图像卷积的一种特例, 一般为$3 times 3$大小的卷积核以步长为1的方式在图像上滑动, 计算卷积核与图像对应区域的点积来得到新的像素值.
在时间复杂度上, 滤波的时间复杂度为$O(N^2)$, 其中$N$为图像的宽或高.

滤波的种类有很多种, 特定滤波器都是为了达到提取或抑制图像中的特定信号而设计的. 盒子滤波是一种经典的滤波方法, 又具有很好的可分离特征, 因此我们将会选择它作为本实验的对比目标.

== 实验思考

在这个图像处理任务上, 我们能看出各种语言已经出现了明显的性能差异.
首先, 即便是附加了Python-ABI的调用的额外开销, OpenCV依然保持着其优势, 我们需要深入分析其调用的细节才能得出结论.

== 盒子滤波优化

前面我们

= Step into new era: 自动调度与DSL(Domain Specific Language)

在谈论自动调度之前, 我们先将目光转向高性能计算最为重要的概念之一: Hardware Abstraction Layer (HAL).
硬件抽象层(英语: Hardware Abstraction Layer, 缩写HAL)是软件层的例行程序包, 用于模拟特定系统平台的细节使程序可以直接访问硬件的资源。将硬件方面的不同抽离操作系统的核心, 核心模式的代码就不必因为硬件的不同而需要修改。因此硬件抽象层可加大软件的移植性.
在Linux下, HAL的实现是通过`libc`来完成的, 其中包含了对各种系统调用的封装, 使得程序可以通过统一的接口来访问底层硬件资源.
迁移到高性能计算领域, HAL的概念也被广泛应用于GPU编程和分布式计算中, 例如CUDA和OpenCL等框架都提供了对底层硬件的抽象接口, 使得程序可以在不同的硬件平台上运行而无需修改代码.

#image("image-7.png")

OpenCV实际上在`filter`, `DNN` 等计算密集型模块也采用了类似的设计, 比方说在`opencv\modules\core\include\opencv2\core\hal\intrin.hpp`定义了一个`CV_HAL_NAMESPACE`命名空间, 其中包含了对各种SIMD指令集的封装,

```cpp
#if CV_SIMD512 && (!defined(CV__SIMD_FORCE_WIDTH) || CV__SIMD_FORCE_WIDTH == 512)
#define CV__SIMD_NAMESPACE simd512
namespace CV__SIMD_NAMESPACE {
    #define CV_SIMD 1
    #define CV_SIMD_64F CV_SIMD512_64F
    #define CV_SIMD_FP16 CV_SIMD512_FP16
    #define CV_SIMD_WIDTH 64
    typedef v_uint8x64    v_uint8;
    typedef v_int8x64     v_int8;
    // .. other types ...
    CV_INTRIN_DEFINE_WIDE_INTRIN_ALL_TYPES(v512)
#if CV_SIMD512_64F
    typedef v_float64x8   v_float64;
    CV_INTRIN_DEFINE_WIDE_INTRIN(double, v_float64, f64, v512, load)
#endif
        inline void vx_cleanup() { v512_cleanup(); }
} // namespace
using namespace CV__SIMD_NAMESPACE;
#elif (CV_SIMD128 || CV_SIMD128_CPP) && (!defined(CV__SIMD_FORCE_WIDTH) || CV__SIMD_FORCE_WIDTH == 128)
#if defined CV_SIMD128_CPP
// ...
```

这个OCV制定的自定义抽象指令集支持不同长度SIMD（512,256,128），统一的数据类型（v_type），64F的SIMD intrinsic独立支持。支持不同长度SIMD和统一的数据类型：哪怕是AVX512指令集，还是NEON指令集，使用的数据结构、API接口都是同一套的.

而各种硬件后端的分发也被封装为`DISPATCH_SIMD_FUN`宏, 例如在`opencv\modules\core
\src\arithm.simd.hpp`定义了这样的函数宏

```cpp
#undef DISPATCH_SIMD_FUN
#define DISPATCH_SIMD_FUN(fun, _T1, _Tvec, ...)                          \
    void fun(SCALAR_ARGS(_T1), void* scalar)                             \
    {                                                                    \
        CV_INSTRUMENT_REGION();                                          \
        CALL_HAL(fun, __CV_CAT(cv_hal_, fun),                            \
            SCALAR_ARGS_PASS, (const double*)scalar)                     \
        ARITHM_CALL_IPP(__CV_CAT(arithm_ipp_, fun),                      \
            SCALAR_ARGS_PASS, (const double*)scalar)                     \
        CV_CPU_DISPATCH(fun, (SCALAR_ARGS_PASS, (const double*)scalar),  \
            CV_CPU_DISPATCH_MODES_ALL);                                  \
    }
```

有了这样一套架构非依赖性接口, 在实际的计算代码中, 就省去了大段使用条件编译判断的繁琐, 不仅使可读性直线上升, 在编写平台无关的代码也有很明显的优势: 当想要支持一个新的平台时, 只需要在对应条件编译分支中实现相同的接口, 而基本不用修改其他代码.

HAL这一思想在计算机科学中其实并不新鲜, 当两套系统难以直接跨越逻辑边界进行交互时, 引入中间层来进行抽象与解耦
是很多系统设计的常用手段.
在这种思想的指导下, 我们很快就意识到, 算法和系统调优实际上可以解耦为两个独立的部分. 作为人类程序开发者, 我们只需要专注于算法的设计和实现, 而将系统调优的工作交给计算机来完成更加适合.

于是, Halide[Halide](https://halide-lang.org/)应运而生.
Halide是一个专门为图像处理和计算密集型任务设计的DSL, 它提供了一种声明式的编程模型, 使得程序员可以专注于算法的设计而不必关心底层的硬件细节.
它的核心组件有三个:

1. *Halide DSL*: 类似于函数声明式的语法, 提供了一种可以生成复杂循环结构的语言，可以用来描述“算法”。
2. *Schedule primitive*: 对各种硬件特性进行抽象，使用统一的接口优化算法，避免了学习具体的硬件结构.
3. *Auto Scheduler*: 自动调度器, 通过分析算法的特性和硬件的特性, 使用局部搜索或遗传算法等自动生成最优的调度策略.

== Halide DSL

Halide DSL的语法十分简介直观, 主要有以下几种类型:

```c
// declare a function (pure function)
Func f;
// define a function
f(x, y) = x + y;

// declare a variable
Var x, y;

// define a expression
Expr e = f(x, y) + 1;

// define a buffer of a 1024x768 grayscale image
Buffer<uint8_t> img(1024, 768);
```

这样一来, Halide DSL就有了几分元编程范式的味道, 比如一个3x3的盒子滤波器就可以这样定义:

```c
Func box_filter(Func input) {
    Var x, y;
    Func blurred("blurred");
    blurred(x, y, c) = (input(x-1, y-1, c) + input(x-1, y, c) + input(x-1, y+1, c) +
                                             input(x, y-1, c) + input(x, y, c) + input(x, y+1, c) +
                                             input(x+1, y-1, c) + input(x+1, y, c) + input(x+1, y+1, c)) / 9.0f;
    return blurred;
}
```

当然, 这样一个朴素的定义并不能发挥出Halide的优势, 因为它没有利用Halide的调度特性, 因此性能上表现平平:

#table(
  fill: (x, _) =>
    if calc.odd(x) { luma(240) }
    else { white },
  align: (x, y) =>
    if y == 0 { center }
    else if x == 0 { center }
    else { left },
  columns: 2,
  [*Image Shape*], [*Running Time*],
  [(300, 200, 3)], [185us],
  [(600, 400, 3)], [1.2ms],
  [(1200, 800, 3)], [4.5ms],
  [(2400, 1600, 3)], [18.2ms],
  [(4800, 3200, 3)], [72.5ms],
)

我们简单利用前面提到的滤波器横纵向分离, 便可以进一步得到复杂度降为$O(N)$的盒子滤波器:

```c
Func box_filter(Func input) {
    Var x, y;
    Func blurred("blurred");
    // horizontal pass
    blurred_x(x, y, c) = (input(x-1, y, c) + input(x, y, c) + input(x+1, y, c)) / 3.0f;
    // vertical pass
    blurred_y(x, y, c) = (blurred_x(x, y-1, c) + blurred_x(x, y, c) + blurred_x(x, y+1, c)) / 3.0f;
    return blurred_y;
}
```

当然, 只有算法上的优化怎么行, 我们还需要对代码调度进行优化, 以充分利用硬件的特性.
常用的调度策略优化比如:

- *Parallelization*: 将计算任务分配到多个线程上并行执行, 以充分利用多核CPU的计算能力.
- *Vectorization*: 利用SIMD指令集对数据进行并行处理, 以提高计算效率.
- *Loop Tiling*: 将循环分块处理, 以提高数据局部性和缓存命中率.
- *Memory Layout Optimization*: 优化数据在内存中的布局, 以提高缓存命中率和内存访问效率.
- *Pipeline Optimization*: 将计算任务分成多个阶段, 以提高数据流的效率和减少数据依赖.

都可以简单地通过Halide的调度接口来实现, 例如:
```c
Func optimized_blur_3x3(Buffer<float> input_buffer) {
  Func blur_x, blur_y, input;
  Var x, y, xi, yi;

  input(x, y) = input_buffer(x, y);

  Func padded = BoundaryConditions::repeat_edge(input_buffer);

  blur_x(x, y) = (padded(x - 1, y) + padded(x, y) + padded(x + 1, y)) / 3;
  blur_y(x, y) = (blur_x(x, y - 1) + blur_x(x, y) + blur_x(x, y + 1)) / 3;

  blur_y.tile(x, y, xi, yi, 64, 16)
      .vectorize(xi, 8)
      .parallel(y);

  blur_x.compute_at(blur_y, y).vectorize(x, 8);
  return blur_y;
}
```

类似于OpenCV的调度优化策略, 我们在这里将blur_y的执行进行分块处理, 每个块的大小为$64 times 16$, 并且在x轴上使用8宽度的SIMD指令集进行向量化处理.
#table(
  fill: (x, _) =>
    if calc.odd(x) { luma(240) }
    else { white },
  align: (x, y) =>
    if y == 0 { center }
    else if x == 0 { center }
    else { left },
  columns: 4,
  [*Image Shape*], [*Running Time*], [*Original*], [*Speedup*],
  [(300, 200, 3)], [45us], [185us], [4.1x],
  [(600, 400, 3)], [250us], [1.2ms], [4.8x],
  [(1200, 800, 3)], [1.1ms], [4.5ms], [4.1x],
  [(2400, 1600, 3)], [4.5ms], [18.2ms], [4.0x],
  [(4800, 3200, 3)], [18.2ms], [72.5ms], [4.0x],
)

可以看到, 我们仅添加了几行代码与api调用, 就使得运行时间大幅下降, 这也说明了Halide的平台优化确实名不虚传.

通过这个简单的调优实例可以看到, 即便不用掌握各平台的繁琐的SIMD指令集和细致的硬件特性, 只需要使用Halide的调度接口, 也可以轻松地实现向量化、分块化和并行化的图像处理算法.
Halide的设计思想还催发了更多的研究和实践, 例如深度学习编译器Apache TVM(https://tvm.apache.org/), 它同样提供了一个高层次的DSL来描述深度学习模型, 并且通过自动调度器来生成最优的执行计划.

= 总结


