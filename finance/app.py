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


@app.route('/')
@login_required
def index():
    """Show portfolio of stocks"""
    # Get the user_id for current session
    user_id = session["user_id"]
    # Get information from the database
    user_data = db.execute("SELECT * FROM portfolio WHERE user = ?", user_id)
    cash_value = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    list_info = []
    total = cash_value
    for rows in user_data:
        if int(rows["shares"]) != 0:
            dict_info = {}
            current_data = lookup(rows["symbol"])
            name, symbol, price = current_data["name"], current_data["symbol"], current_data["price"]
            dict_info["symbol"] = symbol
            dict_info["name"] = name
            dict_info["price"] = usd(float(price))
            dict_info["shares"] = int(rows["shares"])
            dict_info["total"] = usd(float(price) * int(rows["shares"]))
            list_info.append(dict_info)
            total += float(price) * int(rows["shares"])
    return render_template("index.html", list_info=list_info, cash=usd(cash_value), total=usd(total))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote"""
    if request.method == "POST":
        if not (symb := request.form.get("symbol")):
            return apology("Ticker field can't be blank")
        else:
            try:
                data = lookup(symb)
                name, price, symbol = data["name"], data["price"], data["symbol"]
                return render_template("quoted.html", symbol=symbol, name=name, price=usd(price))
            except TypeError:
                return apology("Ticker is not valid")
    else:
        return render_template("quote.html")


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invlid username and/or password", 403)

        # Remeber which use has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
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


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # User reached route via POST
    if request.method == "POST":
        # Ensure username was submitted
        if not (username := request.form.get("username")):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not (password := request.form.get("password")):
            return apology("must provide password", 400)

        # Ensure password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 400)

        # Ensure that two passwords are correct
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Two passwords did not match!", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(rows) != 0:
            return apology("This username is not available", 400)

        # Hash the password through generating function
        hash = generate_password_hash(password)
        db.execute("INSERT INTO users (username, hash) VALUES(?,?)", username, hash)

        # Keep the user logged in
        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]
        return redirect("/")

    # User routed through GET method
    else:
        return render_template("register.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # User reached route via POST
    if request.method == "POST":
        if not (symb := request.form.get("symbol")):
            return apology("Enter Symbol")
        elif not (shares := request.form.get("shares")):
            return apology("Enter number of shares")

        # Calculate the cost of transaction
        try:
            quote = lookup(symb)
            name, price, symbol = quote["name"], float(quote["price"]), quote["symbol"]
            shares = int(shares)
            cost = price * shares
            if shares <= 0:
                return apology("Invalid share count")
        except TypeError:
            return apology("Invalid Ticker")
        except ValueError:
            return apology("Invalid share count")

        # Get the users data from the database
        user_id = session["user_id"]
        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Check if the user has sufficient amount of cash for the transaction
        if user_data[0]["cash"] < cost:  # my user_id starts from 6 for this database
            return apology("Not enough cash for this transaction")
        else:
            t_time = datetime.now()  # transaction time
            # deduct the cash amount
            db.execute("UPDATE users SET cash = ? WHERE id = ?", user_data[0]["cash"] - cost, user_id)
            # insert the transaction into the database transaction
            db.execute("INSERT INTO transactions (userid, symbol, shares, activity, datetime) \
                        VALUES(?,?,?,?,?) ", user_id, symbol, shares, "BUY", t_time)

            # Update the portfolio for the user
            user_portfolio = db.execute("SELECT * FROM portfolio where user = ? AND symbol = ?", user_id, symbol)
            if len(user_portfolio) == 0:
                db.execute("INSERT INTO portfolio (user, symbol, shares) VALUES(?,?,?)", user_id, symbol, shares)
            else:
                db.execute("UPDATE portfolio SET shares = ? WHERE user = ? AND symbol = ?",
                           int(user_portfolio[0]["shares"]) + shares, user_id, symbol)

        return redirect("/")
    # User reached route via GET
    else:
        return render_template("buy.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST
    if request.method == "POST":
        if not (symb := request.form.get("symbol")):
            return apology("Enter Symbol")
        elif not (shares := request.form.get("shares")):
            return apology("Enter number of shares")

        # Calculate the potential credit of the transaction
        try:
            quote = lookup(symb)
            name, price, symbol = quote["name"], float(quote["price"]), quote["symbol"]
            shares = int(shares)
            cost = price * shares
            if shares <= 0:
                return apology("Invalid share count")
        except TypeError:
            return apology("Invalid ticker")
        except ValueError:
            return apology("Invalid share count")

        # Get the users data from the database
        user_id = session["user_id"]
        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        # Update the portfolio for the user
        user_portfolio = db.execute("SELECT * FROM portfolio where user = ? AND symbol = ?", user_id, symbol)
        if len(user_portfolio) == 0:
            return apology("You do not have these shares.")
        elif int(user_portfolio[0]["shares"]) < shares:
            return apology("You do not have these many shares.")
        else:
            db.execute("UPDATE portfolio SET shares = ? WHERE user = ? AND symbol = ?",
                       int(user_portfolio[0]["shares"]) - shares, user_id, symbol)

        t_time = datetime.now()  # transaction time
        # add the cash amount
        db.execute("UPDATE users SET cash = ? WHERE id = ?", user_data[0]["cash"] + cost, user_id)
        # insert the transaction into the database transaction
        db.execute("INSERT INTO transactions (userid, symbol, shares, activity, datetime) VALUES(?,?,?,?,?) ",
                   user_id, symbol, shares, "SELL", t_time)
        return redirect("/")

    # User reached route via GET
    else:
        user_id = session["user_id"]
        symbols = db.execute("SELECT symbol, shares FROM portfolio WHERE user = ?", user_id)
        symbols_own = []
        for item in symbols:
            if int(item['shares']) != 0:
                symbols_own.append(item['symbol'])
        return render_template("sell.html", symbols=symbols_own)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    data = db.execute("SELECT * FROM transactions where userid = ?", user_id)
    print(data)
    return render_template("history.html", data=data)


@app.route("/changePassword", methods=["GET", "POST"])
@login_required
def changePassword():
    """Change user password"""
    if request.method == "POST":
        # Ensure password was submitted
        if not (password := request.form.get("password")):
            return apology("must provide password", 403)

        # Ensure password was submitted
        elif not request.form.get("confirmation"):
            return apology("must provide password confirmation", 403)

        # Ensure that two passwords are same
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Two passwords did not match!", 403)

        user_id = session["user_id"]
        # Hash the password through generating function
        hash = generate_password_hash(password)
        db.execute("UPDATE users SET hash = ? WHERE id = ?", hash, user_id)
        return redirect("/login")
    else:
        return render_template("changePassword.html")
