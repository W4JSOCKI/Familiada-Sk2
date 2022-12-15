import PySimpleGUI as sg


def main():
    nick = ""

    info = {
        "playersNumber": -1,
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
        "correct3": "cor3"
    }

    def getNicknames():
        return [info['nick1'], info['nick2'], info['nick3'], info['nick4'], info['nick5'], info['nick6']]

    def reload(newInfo):
        lines = newInfo.split("\n")
        info["playersNumber"] = lines[0]
        info["nick1"] = lines[1]
        info["nick2"] = lines[2]
        info["nick3"] = lines[3]
        info["nick4"] = lines[4]
        info["nick5"] = lines[5]
        info["nick6"] = lines[6]
        info["roundNumber"] = lines[7]
        info["t1Points"] = lines[8]
        info["t2Points"] = lines[9]
        info["t1Errors"] = lines[10]
        info["t2Errors"] = lines[11]
        info["answering"] = lines[12]
        info["timeLeft"] = lines[13]
        info["answersCount"] = lines[14]
        info["question"] = lines[15]
        info["correct1"] = lines[16]
        info["correct2"] = lines[17]
        info["correct3"] = lines[18]

    def refresh():
        window["nick1"].update(info["nick1"])
        window["nick2"].update(info["nick2"])
        window["nick3"].update(info["nick3"])
        window["nick4"].update(info["nick4"])
        window["nick5"].update(info["nick5"])
        window["nick6"].update(info["nick6"])

        window["t1Points"].update(info["t1Points"])
        window["t2Points"].update(info["t2Points"])
        window["t1Errors"].update(info["t1Errors"])
        window["t2Errors"].update(info["t2Errors"])

        window["question"].update(info["question"])
        window["correct1"].update(info["correct1"])
        window["correct2"].update(info["correct2"])
        window["correct3"].update(info["correct3"])
        return

    def askIfValidName():
        return
        # spytaj serwer

    # wczytaj dane

    getNicknameLayout = [[sg.Text("Enter nickname:")],
                         [sg.InputText()],
                         [sg.Button("OK"), sg.Button("Exit")]]

    loginWindow = sg.Window("Login", getNicknameLayout)

    while nick == "":
        event, values = loginWindow.read()
        if event == sg.WIN_CLOSED or event == 'Exit':
            break
        nick = values[0]

    loginWindow.close()

    blueTeamLayout = [[sg.Text(info["nick1"], text_color=("cyan" if nick == info["nick1"] else "blue"), key="nick1"),
                       sg.Text(info["nick2"], text_color=("cyan" if nick ==
                               info["nick2"] else "blue"), key="nick2"),
                       sg.Text(info["nick3"], text_color=("cyan" if nick == info["nick3"] else "blue"), key="nick3")],
                       [sg.Text("Points: "), sg.Text(info["t1Points"], key="t1Points")],
                       [sg.Text("Errors: "), sg.Text(info["t1Errors"], key="t1Errors")]]

    redTeamLayout = [[sg.Text(info["nick4"], text_color=("orange" if nick == info["nick4"] else "yellow"), key="nick4"),
                      sg.Text(info["nick5"], text_color=("orange" if nick ==
                              info["nick5"] else "yellow"), key="nick5"),
                      sg.Text(info["nick6"], text_color=("orange" if nick == info["nick6"] else "yellow"), key="nick6")],
                       [sg.Text("Points: "), sg.Text(info["t2Points"], key="t2Points")],
                       [sg.Text("Errors: "), sg.Text(info["t2Errors"], key="t2Errors")]]

    questionsLayout = [[sg.Text(info.get("question"), key="question")],
                       [sg.Text(info.get("correct1"), key="correct1")],
                       [sg.Text(info.get("correct2"), key="correct2")],
                       [sg.Text(info.get("correct3"), key="correct3")]]

    answerLayout = [[sg.Text("Time left:"), sg.Text(info["timeLeft"], key="timeLeft")],
                    [sg.Text("Answer:"), sg.InputText()]]

    mainLayout = [[sg.Frame("Blue team", blueTeamLayout, expand_x=True, key="blueTeamLayout")],
                  [sg.Frame("Questions", questionsLayout, expand_x=True, key="questionsLayout")],
                  [sg.Frame("Red team", redTeamLayout, expand_x=True, key="redTeamLayout")],
                  [sg.Frame("Answer", answerLayout, expand_x=True, visible=False, key="answerLayout")],
                  [sg.Text("                                                                                                  ")],
                  [sg.Button('Ok'), sg.Button('Exit')]]

    window = sg.Window('Familiada', mainLayout)

    while True:
        event, values = window.read()
        if event == sg.WIN_CLOSED or event == 'Exit':
            break
        if event == 'Ok':
            print('You entered ', values[0])

    window.close()


if (__name__ == '__main__'):
    main()
