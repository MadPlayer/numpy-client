from flask import Flask, request
import pickle
import numpy as np
import json
from build.include.packet_pb2 import tensor


app = Flask(__name__)

def parse_numpy_from_string(data):
    t = tensor()
    t.ParseFromString(data)
    return np.array(t.data, shape=(t.channel, t.height, t.width))


def serialize_numpy_to_tensor(numpy_array):
    t = tensor()
    t.channel, t.height, t.width = numpy_array.shape
    t.data.extend(numpy_array.flatten("A"))
    return t.SerializeToString()

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
    numpy_array = np.array([i for i in range(27)])
    numpy_array.shape = (3, 1, 9)
    return serialize_numpy_to_tensor(numpy_array)


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
