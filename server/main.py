from bottle import Bottle, template
import pyautogui

pyautogui.PAUSE = 1.5  # 每个函数执行后停顿1.5秒
pyautogui.FAILSAFE = True  # 鼠标移到左上角会触发FailSafeException，因此快速移动鼠标到左上角也可以停止

app = Bottle()


@app.route('/')
def index():
    return template("./index.html")


@app.route("/page_up")
def page_up():
    pyautogui.press('pageup')
    return {"code": 0, "msg": "上翻"}


@app.route("/page_down")
def page_down():
    pyautogui.press('pagedown')
    return {"code": 0, "msg": "下翻"}


app.run(host="0.0.0.0", port=8089)
