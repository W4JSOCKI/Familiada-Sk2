import PySimpleGUI as sg
import time

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
    "answersCount": -1,
    "answering": -1,
    "timeLeft": -1,
    "question": "que",
    "correct1": "cor1",
    "correct2": "cor2",
    "correct3": "cor3",
    "correct4": "cor4",
    "correct5": "cor5",
    "previous1": "prev1",
    "previous2": "prev2",
    "previous3": "prev3"
}

layouts = {
    "getNicknameLayout": [[sg.Text("Enter nickname:")],
                          [sg.InputText(key="inputNickname"),],
                          [sg.Button("Ok"), sg.Button("Exit")]]
}


def createLayouts(nick):
    layouts["blueTeamLayout"] = [[sg.Text(info["nick1"], text_color=("cyan" if nick == info["nick1"] else "blue"), key="nick1"),
                                  sg.Text(info["nick2"], text_color=("cyan" if nick == info["nick2"] else "blue"), key="nick2"),
                                  sg.Text(info["nick3"], text_color=("cyan" if nick == info["nick3"] else "blue"), key="nick3")],
                                 [sg.Text("Points: "), sg.Text(info["t1Points"], key="t1Points")],
                                 [sg.Text("Errors: "), sg.Text(info["t1Errors"], key="t1Errors")]]

    layouts["redTeamLayout"] = [[sg.Text(info["nick4"], text_color=("orange" if nick == info["nick4"] else "yellow"), key="nick4"),
                                 sg.Text(info["nick5"], text_color=("orange" if nick == info["nick5"] else "yellow"), key="nick5"),
                                 sg.Text(info["nick6"], text_color=("orange" if nick == info["nick6"] else "yellow"), key="nick6")],
                                [sg.Text("Points: "), sg.Text(info["t2Points"], key="t2Points")],
                                [sg.Text("Errors: "), sg.Text(info["t2Errors"], key="t2Errors")]]

    layouts["questionsLayout"] = [[sg.Text(info["question"], key="question")],
                                  [sg.Text(info["correct1"], key="correct1")],
                                  [sg.Text(info["correct2"], key="correct2")],
                                  [sg.Text(info["correct3"], key="correct3")],
                                  [sg.Text(info["correct4"], key="correct4")],
                                  [sg.Text(info["correct5"], key="correct5")],]
    
    layouts["previousAnswersLayout"] = [[sg.Text(info["previous1"], key="previous1")],
                                  [sg.Text(info["previous2"], key="previous2")],
                                  [sg.Text(info["previous3"], key="previous3")],]

    layouts["mainLayout"] = [[sg.Frame("Blue team", layouts["blueTeamLayout"], expand_x=True, key="blueTeamLayout")],
                             [sg.Frame("Questions", layouts["questionsLayout"], expand_x=True, key="questionsLayout")],
                             [sg.Frame("Red team", layouts["redTeamLayout"], expand_x=True, key="redTeamLayout")],
                             [sg.Frame("Previous answers", layouts["previousAnswersLayout"], expand_x=True, key="previousAnswersLayout")],
                             [sg.Text("                                                                                                  ")],
                             [sg.Button('Ok'), sg.Button('Exit')]]


def getNicknames():
    return [info['nick1'], info['nick2'], info['nick3'], info['nick4'], info['nick5'], info['nick6']]


def reload(newInfo):
    lines = newInfo.split("\n")
    i=0
    for x in info.keys():
        info[x] = lines[i]
        i+=1


def refresh():
    mainWindow["nick1"].update(info["nick1"])
    mainWindow["nick2"].update(info["nick2"])
    mainWindow["nick3"].update(info["nick3"])
    mainWindow["nick4"].update(info["nick4"])
    mainWindow["nick5"].update(info["nick5"])
    mainWindow["nick6"].update(info["nick6"])

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

    mainWindow["previous1"].update(info["previous1"])
    mainWindow["previous2"].update(info["previous2"])
    mainWindow["previous3"].update(info["previous3"])

def answerWindow():

    timeStart = time.time()
    timeLeft = 10 #info["timeLeft"]
    currTime = timeLeft
    layouts["answerLayout"] = [[sg.Text(info["question"], key="question")],
                               [sg.Text("Time left:"), sg.Text(info["timeLeft"], key="timeLeft")],
                               [sg.Text("Answer:"), sg.InputText(key="answer")],
                               [sg.Button('Ok')]]

    answerWindow = sg.Window("Answer", layouts["answerLayout"], modal=True, finalize=True, disable_close=True)
    answerWindow["answer"].bind("<Return>", "_Enter")

    while True:
        currTime = (int)(timeLeft-(time.time()-timeStart))
        answerWindow["timeLeft"].update(currTime)
        event, values = answerWindow.read(timeout=10)
        if event in ('Ok', "answer" + "_Enter") or currTime<=0:
            giveAnswer(values["answer"], currTime)
            break

    answerWindow.close()


def giveAnswer(answer, timeleft):
    f = open("answer.txt", "w")
    f.write(answer + "\n" + timeleft)
    f.close()


def sendNickname(nickname):
    f = open("name.txt", "w")
    f.write(nickname)
    f.close()

def gameOver(winner: bool):
    layouts["gameOver"] = [[sg.Text("You won!" if winner else "You lost!")],
                          [sg.Button("Exit")]]

    gameOverWindow = sg.Window("Game Over", layouts["gameOver"], finalize=True)

    while True:
        event, values = gameOverWindow.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            exit()

def main():
    global mainWindow
    nickname = ""

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
    createLayouts(nickname)
    mainWindow = sg.Window('Familiada', layouts["mainLayout"], finalize=True)

    while True:
        event, values = mainWindow.read()
        if event in (sg.WIN_CLOSED, 'Exit'):
            break
        info["name1"] = "asdasdad"
        refresh()
        answerWindow()
        gameOver(True)

    mainWindow.close()


if (__name__ == '__main__'):
    main()
