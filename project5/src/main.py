import cv2

IMAGE_PATH = "./img"

def main():
    img = cv2.imread(f"{IMAGE_PATH}/1c8192x8192.bmp", cv2.IMREAD_COLOR)
    if img is None:
        print("Image not found or could not be read.")
        return
    cv2.imwrite(f"{IMAGE_PATH}/1c8192x8192copy.bmp", img)

    img = cv2.imread(f"{IMAGE_PATH}/1c4096x4096.bmp", cv2.IMREAD_COLOR)
    if img is None:
        print("Image not found or could not be read.")
        return
    cv2.imwrite(f"{IMAGE_PATH}/1c4096x4096copy.bmp", img)
    
    img = cv2.imread(f"{IMAGE_PATH}/3c4096x4096.bmp", cv2.IMREAD_COLOR)
    if img is None:
        print("Image not found or could not be read.")
        return
    cv2.imwrite(f"{IMAGE_PATH}/3c4096x4096copy.bmp", img)

    img = cv2.imread(f"{IMAGE_PATH}/4c4096x4096.bmp", cv2.IMREAD_COLOR)
    if img is None:
        print("Image not found or could not be read.")
        return
    cv2.imwrite(f"{IMAGE_PATH}/4c4096x4096copy.bmp", img)

if __name__ == "__main__":
    main()
