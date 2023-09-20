import cv2
import os
import time


# 显示或隐藏光标
def show_cursor(visible):
    if visible:
        os.system("setterm -cursor on")
    else:
        os.system("setterm -cursor off")


def main():
    mode = 0
    ch = ""
    temp = None
    output = []
    begin, end = 0, 0

    # 获取当前脚本所在的文件夹路径
    script_dir = os.path.dirname(os.path.abspath(__file__))

    # 读取字符集
    char_file = os.path.join(script_dir, "CHAR.txt")
    with open(char_file, "r") as fc:
        ch = fc.readline().rstrip()

    # 获取数据
    video_name = input("请输入视频路径: ")
    video = cv2.VideoCapture(video_name)

    out_size = tuple(map(int, input("生成字符画的分辨率: ").split()))

    lag = int(input("请输入延迟: "))

    c = input("是否保存字符画?[Y/n]")
    flag = c.lower() == "y"

    if flag:
        mode = int(input("请输入生成模式: "))

        # 创建或更新文件夹
        ascii_art_dir = os.path.join(script_dir, "ASCII_Art")
        if os.path.exists(ascii_art_dir):
            os.system(f"rmdir /s /q {ascii_art_dir}")
        os.makedirs(ascii_art_dir)

    # 创建文本文档
    if mode == 2 or mode == 3:
        fp = open(os.path.join(ascii_art_dir, "0.txt"), "w")

    # 清屏
    os.system("cls")

    # 隐藏光标并开启计时器
    show_cursor(False)
    begin = time.time()

    frame_count = int(video.get(cv2.CAP_PROP_FRAME_COUNT))
    for n in range(1, frame_count + 1):
        # 获取每一帧图片并将其转GRAY颜色、缩放
        ret, temp = video.read()
        temp = cv2.cvtColor(temp, cv2.COLOR_BGR2GRAY)
        temp = cv2.resize(temp, out_size, interpolation=cv2.INTER_LINEAR)

        # 生成每一帧图片的字符画
        s = ""
        for i in range(temp.shape[0]):
            for j in range(temp.shape[1]):
                tmpc = ch[int(temp[i, j] * len(ch) / 256)]
                s += tmpc
                if mode == 3:
                    fp.write(tmpc)
            s += "\n"

        # 保存字符动画
        output.append(s)
        if mode == 1:
            text_name = os.path.join(ascii_art_dir, f"{n}.txt")
            with open(text_name, "w") as fp:
                fp.write(s)
        elif mode == 2:
            fp.write(s)
        elif mode == 3:
            fp.write("\n")

        # 输出加载信息
        if n % 17 == 0:
            print(f"加载中...\t{n}/{frame_count}\t{n / frame_count * 100:.2f}%")

    # 结束计时器并显示光标
    end = time.time()
    show_cursor(True)

    # 清屏并打印信息
    os.system("cls")
    print(f"\n共生成了 {frame_count} 张字符画")
    print(f"耗时 {end - begin:.2f}s")
    print(f"平均速度为 {frame_count / (end - begin):.2f}帧/s")

    input("\n按下任意键播放")

    # 隐藏光标并开启计时器
    show_cursor(False)
    begin = time.time()

    # 打印字符画
    for i in range(frame_count):
        if lag < 0:
            # 原速打印字符画
            i = int((time.time() - begin) / (1 / video.get(cv2.CAP_PROP_FPS)))
            os.system("cls")
            print(output[i], end="", flush=True)
        else:
            # 指定速度打印字符画
            os.system("cls")
            print(output[i], end="", flush=True)
            time.sleep(lag / 1000)

    # 结束计时器并显示光标
    end = time.time()
    show_cursor(True)

    # 打印信息
    print(f"\n共播放了 {frame_count} 张字符画")
    print(f"耗时 {end - begin:.2f}s")
    print(f"平均速度为 {frame_count / (end - begin):.2f}帧/s")

    # 优雅地退出程序
    input("\n按下任意键关闭窗口")


if __name__ == "__main__":
    main()
