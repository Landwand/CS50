import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET", "POST"])
@login_required
def index(portfolio=None, user_value=None):
    """Show portfolio of stocks"""

    portfolio = []
    user_value = {'holdings': 0, 'grand_total': 0, 'cash': 0}
    uid = session["user_id"]

    # calculate user_value
    rows = db.execute("SELECT cash FROM users WHERE id = ?", uid)
    cash = int(rows[0]['cash'])
    user_value['cash'] = cash

    # get list of SYMBOL from HISTORY
    rows = db.execute("SELECT DISTINCT symbol FROM history WHERE id = ? ", uid)
    if not rows:
        return render_template("index.html", portfolio=None)

    # SUM Buy and Sell for each SYMBOL
    for row in rows:

        stock_info = {}
        sym = row['symbol']
        stock_info['symbol'] = sym
        stock_info['shares'] = 0

        buy_rows = db.execute("""SELECT SUM (shares) FROM history WHERE symbol = ?
            AND id = ? AND action = ?""", sym, uid, "buy")
        sell_rows = db.execute("""SELECT SUM (shares) FROM history WHERE symbol = ? AND id = ?
            AND action = ?""", sym, uid, "sell")

        buys = buy_rows[0]['SUM (shares)']
        sells = sell_rows[0]['SUM (shares)']

        if buys is None:
            buys = 0
        if sells is None:
            sells = 0
        if buys < sells:
            return apology("buys less than sells!", 400)

        net = buys-sells

        # get stocks "name" and "price"
        lookup_res = lookup(sym)
        if not lookup_res:
            return apology("symbol lookup failed in INDEX.html line 90", 400)

        # set properties
        stock_info['name'] = lookup_res["name"]
        stock_info['price'] = lookup_res["price"]
        stock_info['shares'] = net
        stock_info['value'] = stock_info['price'] * stock_info['shares']
        # rounding to 2 digits
        stock_info['price'] = round((lookup_res["price"]), 2)
        stock_info['value'] = round((stock_info['value']), 2)
        stock_info['price'] = format(stock_info['price'], '.2f')
        stock_info['value'] = format(stock_info['value'], '.2f')

        # add stock info to Portfolio
        portfolio.append(stock_info)

        # add stock-value to user_value
        user_value['holdings'] = float(user_value['holdings']) + float(stock_info['value'])

        # end for

    user_value['grand_total'] = float(user_value['holdings']) + float(user_value['cash'])

    fcash = format(round(user_value['cash'], 2), '.2f')
    fholdings = format(round(user_value['holdings'], 2), '.2f')
    ftotal = format(round(user_value['grand_total'], 2), '.2f')
    user_value['grand_total'] = ftotal
    user_value['cash'] = fcash
    user_value['holdings'] = fholdings

    # user asks to Add Cash to balance
    if request.method == "POST":

        if request.form.get("add_cash") and int(request.form.get("add_cash")) > 0:

            app.logger.info("add cash: %s", request.form.get("add_cash"))
            add_cash = float(request.form.get("add_cash"))
            app.logger.info("user value: %s", (user_value['cash']))
            add_cash = float(add_cash) + float(user_value['cash'])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", add_cash, uid)

            # temporarily add add_cash before upcoming network call
            user_value['cash'] = float(user_value['cash']) + float(add_cash)
            return render_template("index.html", portfolio=portfolio, user_value=user_value)

        # ELSE incorrect cash amount
        return apology("incorrect cash amount", 400)

    return render_template("index.html", portfolio=portfolio, user_value=user_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy(oldquote=None, oldprice=None, cash=None):
    """Get stock quote."""

    # if logged-in, get $ Amt
    if session["username"]:
        username = session["username"]
        row = db.execute("SELECT cash FROM users WHERE username = ?", username)
        cash = row[0]['cash']
        cash = int(cash)
        str_cash = str(cash)

    if request.method == "GET":
        return render_template("buy.html", cash=str_cash)

    if request.method == "POST":

        # user QUOTING
        if request.form.get("quote"):
            symbol = request.form.get("quote")
            lookup_res = lookup(symbol)
            if lookup_res:
                return render_template("buy.html", oldquote=lookup_res["name"], oldprice=str(lookup_res["price"]), cash=cash)

            return apology("Stock symbol not valid!", 400)

        # user BUYING
        if request.form.get("symbol"):
            symbol = request.form.get("symbol")
            lookup_res = lookup(symbol)

            if lookup_res:
                cash = int(cash)
                price = lookup_res['price']
            else:
                return apology("Stock symbol not valid!", 400)
        else:
            return apology("Stock sym cannnot be BLANK!", 400)

        # checking SHARES
        if request.form.get("shares"):

            if (request.form.get("shares")).isnumeric() == True \
                    and float(request.form.get("shares")) >= 1:
                shares = request.form.get("shares")
                shares = int(shares)

                # check if User has enough $ to buy
                if price * shares > cash:
                    return apology("You're too poor!", 400)
            else:
                return apology("invalid share number", 400)
        # SHARES check done

            # calculate amount left over
            cash = cash - price * shares
            buytime = datetime.utcnow()
            buytime = str(buytime)

            # inter shit into DB
            row = db.execute("SELECT id FROM users WHERE username = ?", username)
            user_id = row[0]['id']

            db.execute(
                "INSERT INTO history (id, symbol, action, price, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)", user_id, symbol, "buy", price, shares, buytime)
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?", cash, user_id)

            # success > homepage
            return redirect("/")

        else:
            return apology("Shares need to be > 0", 400)


@app.route("/cash", methods=["GET", "POST"])
@login_required
def cash(cash=None):
    """Show portfolio of stocks"""
    uid = session["user_id"]

    # calculate user_value
    rows = db.execute("SELECT cash FROM users WHERE id = ?", uid)
    cash = int(rows[0]['cash'])

    # user asks to Add Cash to balance
    if request.method == "POST":
        if request.form.get("add_cash") and int(request.form.get("add_cash")) > 0:
            app.logger.info("add cash: %s", request.form.get("add_cash"))
            add_cash = float(request.form.get("add_cash"))
            app.logger.info("user value: %s", (user_value['cash']))
            add_cash = float(add_cash) + float(user_value['cash'])
            db.execute("UPDATE users SET cash = ? WHERE id = ?", add_cash, uid)

            # temporarily add add_cash before upcoming network call
            user_value['cash'] = float(user_value['cash']) + float(add_cash)
            return render_template("index.html", portfolio=portfolio, user_value=user_value)

        # ELSE incorrect cash amount
        return apology("incorrect cash amount", 400)
    return render_template("cash.html", cash=cash)


@app.route("/history", methods=["GET"])
@login_required
def history(username=None, history=None):
    """Show history of transactions"""
    uid = session["user_id"]
    username = session["username"]
    rows = db.execute("SELECT * FROM history WHERE id = ?", uid)

    return render_template("history.html", username=username, history=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]
        session["username"] = rows[0]["username"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote(oldquote=None, oldprice=None):
    """Get stock quote."""

    if request.method == "GET":
        return render_template("quote.html")

    if request.method == "POST":

        if request.form.get("symbol"):
            symbol = request.form.get("symbol")
            lookup_res = lookup(symbol)
            if lookup_res:
                #stock_info['price'] = format(stock_info['price'], '.2f')
                lookup_res["price"] = format((lookup_res['price']), '.2f')
                return render_template("quote.html", oldquote=lookup_res["name"], oldprice=str(lookup_res["price"]))

            return apology("Stock symbol not valid!", 400)

        if oldquote:
            return render_template("quote.html", oldquote=lookup_res["name"], oldprice=lookup_res["price"])

        return apology("please input stock symbol!", 400)


@app.route("/register", methods=["GET", "POST"])
def register(message=None):
    """Register user"""

    if request.method == "POST":

        un = request.form.get("username")
        pw = request.form.get("password")
        cpw = request.form.get("confirmation")

        # check existence of all inputs
        if not un:
            return apology("provide a Username when registering", 400)
        elif not pw or not cpw:
            return apology("provide a Password and Confirm-Password", 400)
        elif not pw == cpw:
            return apology("passwords needs to match", 400)

        # check DB for same USERNAME
        rows = db.execute("SELECT * FROM users WHERE username = ?", un)
        if len(rows) > 0:
            return apology("username already exists", 400)

        # hash the password input
        hashed_pw = generate_password_hash(pw)

        # input into DB
        db.execute("INSERT INTO users (username, hash, cash) VALUES (?, ?, ?)", un, hashed_pw, 10000)
        return render_template("register.html", message=un)

    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell(oldquote=None, oldprice=None, cash=None, symbols=None):
    """Sell shares of stock"""

    # get amount of CASH
    if session["username"]:
        username = session["username"]
        uid = session['user_id']
        row = db.execute("SELECT cash FROM users WHERE username = ?", username)
        cash = row[0]['cash']
        cash = int(cash)
        # str_cash = str(cash)

        # create list of Stock names & symbols
        stock_symbols = []
        stock_symbol_rows = db.execute("SELECT DISTINCT symbol FROM history WHERE id = ? ", uid)
        if len(stock_symbol_rows) < 1:
            stock_symbols = None
        else:
            for row in stock_symbol_rows:
                stock_symbols.append(row['symbol'])

    if request.method == "GET":
        # return render_template("sell.html", cash=str_cash)
        return render_template("sell.html", cash=cash, symbols=stock_symbols)

    if request.method == "POST":

        # user QUOTING
        if request.form.get("quote"):
            symbol = request.form.get("quote")
            lookup_res = lookup(symbol)

            if lookup_res:
                return render_template("sell.html", oldquote=lookup_res["name"], oldprice=str(lookup_res["price"]), cash=cash, symbols=stock_symbols)
            return apology("Stock symbol not valid!", 400)

        # user SELLING
        if request.form.get("symbol"):
            f_sym = request.form.get("symbol")
            lookup_res = lookup(f_sym)

            if lookup_res:
                cash = int(cash)
                price = lookup_res['price']
            else:
                return apology("Stock symbol not valid!", 400)
        else:
            return apology("Stock sym cannnot be BLANK!", 400)

        # checking SHARES
        if request.form.get("shares"):

            if int(request.form.get("shares")) > 0:
                shares = request.form.get("shares")
                shares = int(shares)

                # check against ownership needed

            else:
                return apology("selling error - check stock count", 400)
        else:
            return apology("selling error - check stock count", 400)

        # check for enough SHARES
        rows = db.execute("SELECT DISTINCT symbol FROM history WHERE id = ? ", uid)
        if not rows:
            return apology("you have no shares to sell", 400)

        # total transactions to find # of SHARES held
        for row in rows:
            buy_rows = db.execute("SELECT SUM (shares) FROM history WHERE symbol = ? AND id = ? AND action = ?", f_sym, uid, "buy")
            sell_rows = db.execute(
                "SELECT SUM (shares) FROM history WHERE symbol = ? AND id = ? AND action = ?", f_sym, uid, "sell")

            buys = buy_rows[0]['SUM (shares)']
            sells = sell_rows[0]['SUM (shares)']

            if buys is None:
                buys = 0
            if sells is None:
                sells = 0
            if buys < sells:
                return apology("buys fewer than sells!", 400)

            net = buys-sells

            if shares > net:
                return apology("insufficent shares", 400)
        # DONE: total transactions to find # of SHARES held

            # calculate CASH pool after selling
            cash = cash + price * shares
            buytime = datetime.utcnow()
            buytime = str(buytime)

            # enter Sale into Database
            row = db.execute("SELECT id FROM users WHERE username = ?", username)
            user_id = row[0]['id']

            db.execute(
                "INSERT INTO history (id, symbol, action, price, shares, datetime) VALUES (?, ?, ?, ?, ?, ?)", user_id, f_sym, "sell", price, shares, buytime)
            db.execute(
                "UPDATE users SET cash = ? WHERE id = ?", cash, user_id)

            # success > homepage
            return redirect("/")

        # else:
        return apology("Shares need to be > 0", 400)

