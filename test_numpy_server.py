from flask import Flask, request
import pickle
import numpy as np
import json
from build.include.packet_pb2 import tensor, batched_tensor


app = Flask(__name__)


def parse_tensor_from_string(data):
    t = tensor()
    t.ParseFromString(data)
    array = np.array(t.data)
    array.shape = tuple(t.shape)
    return array


def serialize_numpy_to_tensor(numpy_array):
    t = tensor()
    t.shape[:] = list(numpy_array.shape)
    t.data.extend(numpy_array.flatten("A"))
    return t.SerializeToString()


def parse_tensors_from_string(data):
    t = batched_tensor()
    t.ParseFromString(data)
    batch = []
    for tensor in t.tensors:
        array = np.array(tensor.data)
        array.shape = tuple(tensor.shape)
        batch.append(array)
    return np.array(batch)


def serialize_numpy_to_tensors(numpy_array):
    bt = batched_tensor()
    for tensor in numpy_array:
        t = tensor()
        t.shape = list(tensor.shape)
        t.data.extend(numpy_array.flatten("A"))
        bt.tensors.append(t)
    return t.SerializeToString()


@app.route('/')
def home():
    return "hello, flask"


@app.route("/numpy/inference", methods=["POST"])
def processing_tensor():
    msg = request.data
    received = parse_tensor_from_string(msg)
    print(received.shape)
    print(received)
    numpy_array = np.array([i for i in range(27)])
    numpy_array.shape = (3, 1, 9)
    return serialize_numpy_to_tensor(numpy_array)

@app.route("/numpy/inference/batched", methods=["POST"])
def processing_tensors():
    arrays = parse_tensor_from_string(request.data)
    for arr in arrays:
        print(arr)

    return "done"

if __name__ == '__main__':
    app.run(debug=True)    
