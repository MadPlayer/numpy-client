from flask import Flask, request
import pickle
import numpy as np
import json
from build.include.packet_pb2 import tensor


app = Flask(__name__)


def parse_tensor_from_string(data):
    t = tensor()
    t.ParseFromString(data)
    array = np.array(t.data)
    array.shape=(t.channel, t.height, t.width)
    return array


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
    received = parse_tensor_from_string(msg)
    print(received.shape)
    print(received)
    numpy_array = np.array([i for i in range(27)])
    numpy_array.shape = (3, 1, 9)
    return serialize_numpy_to_tensor(numpy_array)


if __name__ == '__main__':
    app.run(debug=True)    
