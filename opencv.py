import cv2

cap = cv2.VideoCapture(0)  # 0表示摄像头编号，如果有多个摄像头可以根据需要更改
if not cap.isOpened():
    print("无法打开摄像头")
    exit()

while True:
    ret, frame = cap.read()  # ret表示是否成功捕获帧，frame表示捕获的帧
    if not ret:
        print("无法捕获帧")
        break
    cv2.imshow("拍照结果", frame)
    if cv2.waitKey(1) == ord(" "):  # 按下's'键后拍照并保存为照片文件
        cv2.imwrite("拍照结果.jpg", frame)
        break

cap.release()
cv2.destroyAllWindows()  # 关闭所有窗口