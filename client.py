import PySimpleGUI as sg

nick = ""
nickIndex = -1

info = {
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

# wczytaj dane

getNickname = [[sg.Text("Enter nickname:")],
               [sg.InputText()],
               [sg.Button("OK"), sg.Button("Exit")]]

login = sg.Window("Login", getNickname)

while nick=="":
    event, values = login.read()
    if event == sg.WIN_CLOSED or event == 'Exit':
        break
    nick = values[0]

login.close()

blueTeam = [[sg.Text(info["nick1"]+" ", text_color=("cyan" if nick==info["nick1"] else "blue")),
             sg.Text(info["nick2"]+" ", text_color=("cyan" if nick==info["nick2"] else "blue")),
             sg.Text(info["nick3"]    , text_color=("cyan" if nick==info["nick3"] else "blue"))]]

redTeam = [[sg.Text(info["nick4"]+" ", text_color=("orange" if nick==info["nick4"] else "yellow")),
            sg.Text(info["nick5"]+" ", text_color=("orange" if nick==info["nick5"] else "yellow")),
            sg.Text(info["nick6"]    , text_color=("orange" if nick==info["nick6"] else "yellow"))]]

questions = [[sg.Text(info.get("question"))],
             [sg.Text(info.get("correct1"))],
             [sg.Text(info.get("correct2"))],
             [sg.Text(info.get("correct3"))]]

answer = [[sg.Text("Time left:"), sg.Text(info["timeLeft"])],
          [sg.Text("Answer:"), sg.InputText()]]


layout = [  [sg.Frame("Blue team", blueTeam, expand_x=True)],
            [sg.Frame("Questions", questions, expand_x=True)],
            [sg.Frame("Red team", redTeam, expand_x=True)],
            [sg.Frame("Answer", answer, expand_x=True)],
            [sg.Button('Ok'), sg.Button('Exit')] ]

window = sg.Window('Familiada', layout)

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED or event == 'Exit':
        break
    print('You entered ', values[0])

window.close()