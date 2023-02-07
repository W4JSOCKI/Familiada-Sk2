import PySimpleGUI as sg
import time
import subprocess
import os

answered = False
correct_before = []

info = {
    "playerID": -1,
    "nick1": "n1",
    "nick2": "n2",
    "nick3": "n3",
    "nick4": "n4",
    "nick5": "n5",
    "nick6": "n6",
    "roundNumber": 0,
    "t1Points": 0,
    "t2Points": 0,
    "t1Errors": 0,
    "t2Errors": 0,
    "answering": -1,
    "question": "que",
    "correct1": "cor1",
    "correct2": "cor2",
    "correct3": "cor3",
    "correct4": "cor4",
    "correct5": "cor5",
    "wrong1": "prev1",
    "wrong2": "prev2",
    "wrong3": "prev3",
    "wrong4": "prev4",
    "wrong5": "prev5",
    "wrong6": "prev6",
    "wrong7": "prev7"
}

layouts = {
    "getNicknameLayout": [[sg.Text("Enter nickname:")],
                          [sg.InputText(key="inputNickname"),],
                          [sg.Button("Ok"), sg.Button("Exit")]]
}


def createLayouts():
    layouts["blueTeamLayout"] = [[sg.Text(info["nick1"], text_color=getPlayerColor(1), key="nick1"),
                                  sg.Text(info["nick2"], text_color=getPlayerColor(2), key="nick2"),
                                  sg.Text(info["nick3"], text_color=getPlayerColor(3), key="nick3")],
                                 [sg.Text("Points: "), sg.Text(info["t1Points"], key="t1Points")],
                                 [sg.Text("Errors: "), sg.Text(info["t1Errors"], key="t1Errors")]]

    layouts["redTeamLayout"] = [[sg.Text(info["nick4"], text_color=getPlayerColor(4), key="nick4"),
                                 sg.Text(info["nick5"], text_color=getPlayerColor(5), key="nick5"),
                                 sg.Text(info["nick6"], text_color=getPlayerColor(6), key="nick6")],
                                [sg.Text("Points: "), sg.Text(info["t2Points"], key="t2Points")],
                                [sg.Text("Errors: "), sg.Text(info["t2Errors"], key="t2Errors")]]

    layouts["questionsLayout"] = [[sg.Text(info["question"], key="question")],
                                  [sg.Text(info["correct1"], key="correct1")],
                                  [sg.Text(info["correct2"], key="correct2")],
                                  [sg.Text(info["correct3"], key="correct3")],
                                  [sg.Text(info["correct4"], key="correct4")],
                                  [sg.Text(info["correct5"], key="correct5")]]
    
    layouts["wrongAnswersLayout"] = [[sg.Text(info["wrong1"], key="wrong1")],
                                  [sg.Text(info["wrong2"], key="wrong2")],
                                  [sg.Text(info["wrong3"], key="wrong3")],
                                  [sg.Text(info["wrong4"], key="wrong4")],
                                  [sg.Text(info["wrong5"], key="wrong5")],
                                  [sg.Text(info["wrong6"], key="wrong6")],
                                  [sg.Text(info["wrong7"], key="wrong7")]]

    layouts["mainLayout"] = [[sg.Frame("Blue team", layouts["blueTeamLayout"], expand_x=True, key="blueTeamLayout")],
                             [sg.Frame("Questions", layouts["questionsLayout"], expand_x=True, key="questionsLayout")],
                             [sg.Frame("Red team", layouts["redTeamLayout"], expand_x=True, key="redTeamLayout")],
                             [sg.Frame("wrong answers", layouts["wrongAnswersLayout"], expand_x=True, key="wrongAnswersLayout")],
                             [sg.Text("                                                                                                  ")],
                             [sg.Button('Exit')]]

def setupInTxt():
    f = open("in.txt", "w")
    f2 = open("in_template.txt", "r")
    text = f2.read()
    f.write(text)

def tryAnswering():
    global answered
    if info["playerID"] != info["answering"]:
        answered = False
    if getCorrect() != correct_before:
        answered = False

def getPlayerColor(id):
    if id == info["answering"]:
        return "purple"
    if id == info["playerID"]:
        if id > 3:
            return "orange"
        else:
            return "cyan"
    if id > 3:
        return "darkRed"
    else:
        return "blue"

def getNicknames():
    return [info['nick1'], info['nick2'], info['nick3'], info['nick4'], info['nick5'], info['nick6']]

def getCorrect():
    return [info["correct1"], info["correct2"], info["correct3"], info["correct4"], info["correct5"]]


def reload():
    f = open("in.txt", "r")
    lines = f.read().split("\n")
    i=0
    for x in info.keys():
        if x in ("playerID", "roundNumber", "t1Points", "t2Points", "t1Errors", "t2Errors", "answering"):
            info[x] = int(lines[i])
        else:
            info[x] = lines[i]
        i+=1


def refresh():
    mainWindow["nick1"].update(info["nick1"])
    mainWindow["nick2"].update(info["nick2"])
    mainWindow["nick3"].update(info["nick3"])
    mainWindow["nick4"].update(info["nick4"])
    mainWindow["nick5"].update(info["nick5"])
    mainWindow["nick6"].update(info["nick6"])

    mainWindow["nick1"].update(text_color=getPlayerColor(1))
    mainWindow["nick2"].update(text_color=getPlayerColor(2))
    mainWindow["nick3"].update(text_color=getPlayerColor(3))
    mainWindow["nick4"].update(text_color=getPlayerColor(4))
    mainWindow["nick5"].update(text_color=getPlayerColor(5))
    mainWindow["nick6"].update(text_color=getPlayerColor(6))

    mainWindow["t1Points"].update(info["t1Points"])
    mainWindow["t2Points"].update(info["t2Points"])
    mainWindow["t1Errors"].update(info["t1Errors"])
    mainWindow["t2Errors"].update(info["t2Errors"])

    mainWindow["question"].update(info["question"])
    mainWindow["correct1"].update(info["correct1"])
    mainWindow["correct2"].update(info["correct2"])
    mainWindow["correct3"].update(info["correct3"])
    mainWindow["correct4"].update(info["correct4"])
    mainWindow["correct5"].update(info["correct5"])

    mainWindow["wrong1"].update(info["wrong1"])
    mainWindow["wrong2"].update(info["wrong2"])
    mainWindow["wrong3"].update(info["wrong3"])
    mainWindow["wrong4"].update(info["wrong4"])
    mainWindow["wrong5"].update(info["wrong5"])
    mainWindow["wrong6"].update(info["wrong6"])
    mainWindow["wrong7"].update(info["wrong7"])

def answerWindow():
    global correct_before, answered
    answered = True
    correct_before = getCorrect()
    timeStart = time.time()
    timeLeft = 120
    currTime = timeLeft
    layouts["answerLayout"] = [[sg.Text(info["question"], key="question")],
                               [sg.Text("Time left:"), sg.Text(timeLeft, key="timeLeft")],
                               [sg.Text("Answer:"), sg.InputText(key="answer")],
                               [sg.Button('Ok')]]

    answerWindow = sg.Window("Answer", layouts["answerLayout"], modal=True, finalize=True, disable_close=True)
    answerWindow["answer"].bind("<Return>", "_Enter")

    while True:
        currTime = (int)(timeLeft-(time.time()-timeStart))
        answerWindow["timeLeft"].update(currTime)
        event, values = answerWindow.read(timeout=10)
        if event in ('Ok', "answer" + "_Enter") or currTime<=0:
            giveAnswer(values["answer"])
            break

    answerWindow.close()


def giveAnswer(answer):
    f = open("answer.txt", "w")
    f.write(str(answer) + "\n" + str(info["playerID"]))
    f.close()


def sendNickname(nickname):
    global cpp_client
    f = open("nickname.txt", "w")
    f.write(nickname)
    f.close()
    cpp_client = subprocess.Popen([os.getcwd() + "/client3.exe"])

def gameOver(blueWon: bool):
    if (info["playerID"] in (1,2,3) and blueWon) or (info["playerID"] in (4,5,6) and not blueWon):
        winner = True
    else:
        winner = False
    layouts["gameOver"] = [[sg.Text("You won!" if winner else "You lost!")],
                          [sg.Button("Exit")]]

    gameOverWindow = sg.Window("Game Over", layouts["gameOver"], finalize=True)

    while True:
        event, values = gameOverWindow.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            exit()

def main():
    global mainWindow

    setupInTxt()

    loginWindow = sg.Window("Login", layouts["getNicknameLayout"], finalize=True)
    loginWindow["inputNickname"].bind("<Return>", "_Enter")

    while True:
        event, values = loginWindow.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            exit()
        if event in ('Ok', "inputNickname" + "_Enter"):
            sendNickname(values["inputNickname"])
            break

    loginWindow.close()
    createLayouts()
    mainWindow = sg.Window('Familiada', layouts["mainLayout"], finalize=True)

    reload()
    refresh()

    while True:
        
        event, values = mainWindow.read(timeout=500)
        if event in (sg.WIN_CLOSED, 'Exit'):
            break

        reload()
        refresh()

        if answered:
            tryAnswering()
        elif info["playerID"] == info["answering"]:
            answerWindow()
        if info["answering"] == -1:
            gameOver(True)
        if info["answering"] == -2:
            gameOver(False)

    cpp_client.kill()
    mainWindow.close()


if (__name__ == '__main__'):
    main()
