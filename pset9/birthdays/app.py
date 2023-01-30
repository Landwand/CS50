import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///birthdays.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
def index():
    BIRTHDAYS = db.execute("SELECT * FROM birthdays")
    app.logger.info("index running")

    if request.method == "POST":

        # if input exists, enter into DB
        name = request.form.get("name")
        month = request.form.get("month")
        day = request.form.get("day")

        if request.form.get("name") and request.form.get("day") and request.form.get("month"):
            db.execute(
                "INSERT INTO birthdays (name, day, month) VALUES (?, ?, ?)", name, day, month)
            return redirect("/success")

        # do error message
        return render_template("/error.html")

    else:

        #Display the entries in the database on index.html
        return render_template("index.html", birthdays=BIRTHDAYS)


@app.route("/error", methods=["GET", "POST"])
def error():
    return render_template("error.html")


@app.route("/success", methods=["GET", "POST"])
def success():
    return render_template("success.html")