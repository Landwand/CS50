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
            return render_template("index.html",portfolio=portfolio, user_value = user_value )

        # ELSE incorrect cash amount
        return apology("incorrect cash amount", 400)
    return render_template("cash.html", cash=cash)