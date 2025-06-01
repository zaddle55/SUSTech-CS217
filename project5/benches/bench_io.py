import os
import numpy as np
import pytest
import cv2

IMG_DIR = "./img/"

if not os.path.exists(IMG_DIR):
    os.makedirs(IMG_DIR)

# warmup
@pytest.fixture(scope='module', autouse=True)
def warmup():
    mat = np.ones((100, 100, 3), dtype=np.uint8) * 255
    cv2.imwrite(f"{IMG_DIR}warmup.bmp", mat)

@pytest.mark.parametrize('N', [128, 256, 512, 1024, 2048, 4096, 8192])
def test_encode_1cNxN(benchmark, N):
    fname = f"{IMG_DIR}1c{N}x{N}.bmp"

    mat = np.ones((N, N, 1), dtype=np.uint8) * 255  # WHITE

    def encode(args):
        mat, fname = args
        cv2.imwrite(fname, mat)

    benchmark.pedantic(lambda: encode((mat, fname)), iterations=1, rounds=3)

@pytest.mark.parametrize('N', [128, 256, 512, 1024, 2048, 4096, 8192])
def test_decode_1cNxN(benchmark, N):
    fname = f"{IMG_DIR}1c{N}x{N}.bmp"

    if not os.path.exists(fname):
        mat = np.ones((N, N, 1), dtype=np.uint8) * 255
        cv2.imwrite(fname, mat)

    def decode():
        return cv2.imread(fname, cv2.IMREAD_GRAYSCALE)

    benchmark(decode)

@pytest.mark.parametrize('N', [1, 3, 4])
def test_encode_Nc4096x4096(benchmark, N):
    fname = f"{IMG_DIR}{N}c4096x4096.bmp"

    mat = np.ones((4096, 4096, N), dtype=np.uint8) * 255  # WHITE

    def encode(args):
        mat, fname = args
        cv2.imwrite(fname, mat)

    benchmark.pedantic(lambda: encode((mat, fname)), iterations=1, rounds=3)

@pytest.mark.parametrize('N', [1, 3, 4])
def test_decode_Nc4096x4096(benchmark, N):
    fname = f"{IMG_DIR}{N}c4096x4096.bmp"

    if not os.path.exists(fname):
        mat = np.ones((4096, 4096, N), dtype=np.uint8) * 255
        cv2.imwrite(fname, mat)

    def decode():
        return cv2.imread(fname, cv2.IMREAD_UNCHANGED)

    benchmark(decode)

@pytest.mark.parametrize('N,M', [
    (240, 4860), (360, 3240), (480, 2430),
    (720, 1620), (810, 1440), (1080, 1080),
    (1440, 810), (1620, 720), (2430, 480),
    (3240, 360), (4860, 240)
])
def test_encode_1cNxM(benchmark, N, M):
    fname = f"{IMG_DIR}1c{N}x{M}.bmp"
    
    mat = np.ones((M, N, 1), dtype=np.uint8) * 255  # WHITE
    
    def encode(args):
        mat, fname = args
        cv2.imwrite(fname, mat)
    
    benchmark.pedantic(lambda: encode((mat, fname)), iterations=1, rounds=3)

@pytest.mark.parametrize('N,M', [
    (240, 4860), (360, 3240), (480, 2430),
    (720, 1620), (810, 1440), (1080, 1080),
    (1440, 810), (1620, 720), (2430, 480),
    (3240, 360), (4860, 240)
])
def test_decode_1cNxM(benchmark, N, M):
    fname = f"{IMG_DIR}1c{N}x{M}.bmp"

    if not os.path.exists(fname):
        mat = np.ones((M, N, 1), dtype=np.uint8) * 255
        cv2.imwrite(fname, mat)
    
    def decode():
        return cv2.imread(fname, cv2.IMREAD_GRAYSCALE)
    
    benchmark(decode)