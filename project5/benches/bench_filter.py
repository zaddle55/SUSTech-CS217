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
    # 预热，确保OpenCV加载
    mat = np.ones((100, 100, 3), dtype=np.uint8) * 255
    cv2.boxFilter(mat, -1, (3, 3), (1, 1), borderType=cv2.BORDER_CONSTANT)

# 参数化测试，对应filterNxN
@pytest.mark.parametrize('N, M', [
    (300, 200),
    (600, 400),
    (900, 600),
    (1200, 800),
    (1800, 1200),
    (2400, 1600),
    (3000, 2000),
    (3600, 2400),
    (6000, 4000)
])
def test_filterNxN(benchmark, N, M):
    fname = f"{IMG_DIR}sample_{N}x{M}.jpg"

    img = cv2.imread(fname, cv2.IMREAD_COLOR)
    if img is None:
        img = np.ones((N, M, 3), dtype=np.uint8) * 255

    def filter_image(args):
        img, fname = args
        filtered_img = cv2.boxFilter(img, -1, (3, 3), (1, 1), borderType=cv2.BORDER_CONSTANT)
        # cv2.imwrite(fname, filtered_img)
        return filtered_img
    benchmark.pedantic(lambda: filter_image((img, fname)), iterations=5, rounds=100)

@pytest.mark.parametrize('K', [3, 5, 7, 9, 11, 13, 15])
def test_filterKxK(benchmark, K):
    N, M = 600, 400
    fname = f"{IMG_DIR}sample_{N}x{M}.jpg"

    img = cv2.imread(fname, cv2.IMREAD_COLOR)
    if img is None:
        img = np.ones((N, M, 3), dtype=np.uint8) * 255

    def filter_image(args):
        img, fname = args
        filtered_img = cv2.boxFilter(img, -1, (K, K), borderType=cv2.BORDER_CONSTANT)
        # cv2.imwrite(fname, filtered_img)
        return filtered_img
    benchmark.pedantic(lambda: filter_image((img, fname)), iterations=5, rounds=100)
