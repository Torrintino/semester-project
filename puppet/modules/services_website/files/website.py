from flask import Flask, request, render_template
import sqlite3
database=':memory:'
conn = sqlite3.connect(database)

app = Flask(__name__)

c = conn.cursor()
c.execute("CREATE TABLE scoreboard(playername STRING, score INTEGER);")
c.execute("CREATE TABLE player(playerID STRING, pname STRING, score INTEGER, gameID INTEGER);")
c.execute("CREATE TABLE game(gameID INTEGER, playernr INTEGER, mode STRING);")
@app.route('/', methods=['POST', 'GET'])
def rules():
    return render_template("rules.html")
@app.route('/startgame' , methods=['POST','GET'])
def data():
        playernr = request.form['spielerzahl']
        gameID = request.form['spielnummer']
        playerlist=[request.form['spieler0'],
            request.form['spieler1'],
            request.form['spieler2'],
            request.form['spieler3'],
            request.form['spieler4'],
            request.form['spieler5'],
            request.form['spieler6'],
            request.form['spieler7']]
        mode = request.form['spielmodus']
        c.execute("INSERT INTO game VALUES(?,?,?);", (gameID,playernr,mode))
        for x in range(0,playernr,1):
            if playerlist[x]!=' ':
                c.execute("INSERT INTO player(playerID, pname)VALUES (?,?);", (x,playerlist[x]))
            else:
                continue


        return render_template("startgame.html",playerlist=playerlist)
@app.route('/scoreboard')
def scoreboard():
    return render_template("scoreboard.html")

if __name__ == '__main__':
    app.run()
