from flask import Flask, request
import pickle
import numpy as np
import json
from packet_pb2 import tensor


app = Flask(__name__)


@app.route('/')
def home():
    return "hello, flask"


@app.route("/numpy/inference", methods=["POST"])
def get_msg():
    msg = request.data
    t = tensor()
    t.ParseFromString(request.data)
    print(t.width)
    print(t.height)
    print(t.channel)
    b = np.array(t.data)
    print(b)
    t.data[:] = [i*2 for i in range(10)]

    return t.SerializeToString()


@app.route("/numpy/example")
def numpy_example():
    arr = np.arange(0, 10, 0.5)
    packet = {
        "header" : "hello world",
        "content": arr
    }
    return pickle.dumps(packet)


if __name__ == '__main__':
    app.run(debug=True)    
