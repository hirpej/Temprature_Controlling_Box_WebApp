from flask import Flask , render_template, request

app = Flask(__name__)
class Data():
    value = 24
d = Data()
@app.route('/', methods=['GET', 'POST'])
def hello_world():
    if request.method == "POST":
        url = request.form['url']
        d.value = url

    return render_template('index.html' , data=d.value)