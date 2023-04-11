purchase_price = int(input("Enter the purchase price (in cents): ")) 
amount_paid = int(input("Enter the amount paid (in cents): ")) 
change_due = amount_paid - purchase_price 
quarters = change_due // 25 
dimes = (change_due % 25) // 10 
nickels = ((change_due % 25) % 10) // 5 
pennies = ((change_due % 25) % 10) % 5 
print("The change due is", quarters, "quarter(s),", dimes, "dime(s),", nickels, "nickel(s), and", pennies, "pennies.")
