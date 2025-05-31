import cv2

TARGET_IMAGE = 'sample.jpg'
SCALES = [0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0]

if __name__ == '__main__':
    # Read the image
    img = cv2.imread(TARGET_IMAGE)

    # print the original image size
    print(f"Original image size: {img.shape[1]}x{img.shape[0]}")

    for scale in SCALES:
        # Resize the image
        resized_img = cv2.resize(img, None, fx=scale, fy=scale, interpolation=cv2.INTER_LINEAR)

        resized_w, resized_h = resized_img.shape[1], resized_img.shape[0]
        # Print the resized image size
        print(f"Resized image size at scale {scale}: {resized_w}x{resized_h}")

        # Save the resized image
        cv2.imwrite(f'sample_{resized_w}x{resized_h}.jpg', resized_img)