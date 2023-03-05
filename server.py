import json
import os
import subprocess
import tempfile

from flask import Flask, render_template, request

app = Flask(__name__)

# route to index.html file
@app.route("/")
def home():
    print("index")
    return render_template("index.html")


@app.route("/run-lox", methods=["POST"])
def run_python():
    # code is in the request data field as a binary string of a json object
    # parse the json object and get the code
    # run the code and return the output
    output = ""
    code = request.data.decode("utf-8")
    code_dict = json.loads(code)
    code = code_dict["code"]
    try:
        # the code is lox code and we need to run it using the lox interpreter
        # we will use the cpplox binary to run the code and return the output
        # the output will be a string
        # we will use the subprocess module to run the binary
        # first we need to write the code to a temporary file
        # then we will run the binary on the file

        with tempfile.NamedTemporaryFile(mode="w", delete=False) as f:
            f.write(code)
            f.close()
            # run the binary on the file
            # the output will be a byte string
            # we will decode it to a string
            output = subprocess.check_output(["cpplox", f.name])
            output = output.decode("utf-8")
            # delete the file
            os.remove(f.name)
    except Exception as e:
        output = "Error: " + str(e)
    return str(output)


if __name__ == "__main__":
    # use the port from the environment variable
    # if it is not set, use port 5000
    port = int(os.environ.get("PORT", 5000))
    app.run(port=port)
