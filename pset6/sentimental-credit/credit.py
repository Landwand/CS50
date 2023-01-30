from sys import exit


def luhns(strnum):
    # application of the checksum

    even_sum = 0
    odd_sum = 0
    sum = 0
    counter = 0

    # start from the end of strnum, working forwards
    for letter in strnum[::-1]:
        number = int(letter)
        counter += 1
        
        if counter % 2 == 0:  # if we're on an even digit
            # multiply by 2, then add the product's DIGITS together
            product_sum = 0
            digits_sum = 0
            digits_sum += number * 2
            if digits_sum > 9:

                tens_digit = digits_sum // 10
                ones_digit = digits_sum % 10
                product_sum += tens_digit
                product_sum += ones_digit
            else:
                product_sum += digits_sum

            even_sum += product_sum
        
        # odd number
        else:
            odd_sum += number

    sum = even_sum + odd_sum

    if sum % 10 == 0:
        return True
    else:
        return False


valid = 0

while True:
    strnum = input('Number: ')
    strnum = strnum.strip()

    try:
        int(strnum)
    except:
        continue

    # check for num of digits: AMEX = 15, VISA 13,15 MC = 15 
    if len(strnum) != 13 and len(strnum) != 15 and len(strnum) != 16:
        #print('wrong length:', len(strnum))
        print('INVALID\n')
        exit()
    
    first2 = strnum[:2]
    first2 = int(first2)

    # MasterCard
    if (first2) >= 51 and (first2) <= 55:
        if luhns(strnum) == True:
            print('MASTERCARD\n')
        else:
            print('L52 INVALID\n')
        
    # AMEX
    elif first2 == 34 or first2 == 37:
        # print ('Amex check')
        if luhns(strnum) == True:
            print('AMEX\n')
        else:
            print('L60 INVALID\n')
    
    # VISA
    elif first2 // 10 == 4:
        # print('check for VISA')
        # print (strnum, ' is strnum')
        if luhns(strnum) == True:
            print('VISA\n')
        else:
            print('INVALID\n')

    else:
        print('INVALID\n')
    
    exit()
