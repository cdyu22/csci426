
# -------------------------------------------------------------------------
#  * This program simulates a simple (s,S) inventory system using demand read  
#  * from a text file.  Backlogging is permitted and there is no delivery lag.
#  * The output statistics are the average demand and order per time interval
#  * (they should be equal), the relative frequency of setup and the time
#  * averaged held (+) and short (-) inventory levels.
#  *
#  * NOTE: use 0 <= MINIMUM < MAXIMUM, i.e., 0 <= s < S.
#  *
#  * Name              : sis1.c  (Simple Inventory System, version 1)
#  * Authors           : Steve Park & Dave Geyer 
#  * Language          : ANSI C 
#  * Latest Revision   : 8-20-97 
#  * Compile with      : gcc sis1.c
#    Translated by   : Philip Steele 
#    Language        : Python 3.3
#    Latest Revision : 3/26/14
#  * ------------------------------------------------------------------------- 
#  */                     

FILENAME = "sis1.dat"            # input data file                */
MINIMUM = 20                    # 's' inventory policy parameter */
MAXIMUM = 80                    # 'S' inventory policy parameter */
import sys
import xlwt
from xlwt import Workbook
wb = Workbook()
sheet1 = wb.add_sheet('Sheet 1')

# print(int(sys.argv[1]))

class sumOf:
  setup = 0.0     #setup instances
  holding = 0.0   #inventory held (+)
  shortage = 0.0  #inventory held (-)
  order = 0.0     #orders
  demand = 0.0    #demands

for i in range(int(sys.argv[1])):
  MINIMUM = i
################################# Main Program ##############################


  index     = 0                      # time interval index     */
  inventory = MAXIMUM                # current inventory level */
  demand = -1                            # amount of demand        */
  order  = -1                              # amount of order         */
  sum = sumOf()

  #read in demands from file 
  try:
    fp = open(FILENAME, "r")
  except IOError:
    print("File not found")
    exit()

  data = []
  for line in fp:
    data.append(int(line))

  fp.close()



  for demand in data:
    index += 1

    if (inventory < MINIMUM):              # place an order          */
      order         = MAXIMUM - inventory
      sum.setup    += 1.0
      sum.order    += order
    
    else:                                   # no order                 */
      order         = 0  

    inventory      += order               # there is no delivery lag */
    #demand          = GetDemand(fp)

    sum.demand     += demand
    if (inventory > demand): 
      sum.holding  += (inventory - 0.5 * demand)
    else:
      sum.holding  += (inventory*inventory) / (2.0 * demand)
      sum.shortage += ((demand - inventory)*(demand-inventory)) / (2.0 * demand) #debug
    
    inventory      -= demand
  #EndFor


  if (inventory < MAXIMUM):               # force the final inventory to */
    order           = MAXIMUM - inventory # match the initial inventory  */
    sum.setup += 1
    sum.order      += order
    inventory      += order


  # print("\nfor {0:1d} time intervals with an average demand of {1:6.2f}".format(index,(sum.demand / index)))
  # print("and policy parameters (s, S) = ({0}, {1})\n".format(MINIMUM, MAXIMUM))
  # print("   average order ............ = {0:6.2f}: ".format(sum.order / index) + str(sum.order / index))
  # print("   setup frequency .......... = {0:6.2f}: ".format(sum.setup / index)+ str(sum.setup / index))
  # print("   average holding level .... = {0:6.2f}: ".format(sum.holding / index)+ str(sum.holding / index))
  # print("   average shortage level ... = {0:6.2f}: ".format(sum.shortage / index)+ str(sum.shortage / index))
  # print("")
  sheet1.write(i,0,i)

  itemCost = sum.order / index * 8000
  setupCost = sum.setup / index * 1000
  holdCost = sum.holding / index * 25
  shortCost = sum.shortage / index * 700.0

  # print("   item cost ................. = {0:6.2f}: ".format(itemCost) + str(itemCost))
  # print("   setup cost ................ = {0:6.2f}: ".format(setupCost)+ str(setupCost))
  # print("   holding cost .............. = {0:6.2f}: ".format(holdCost) + str(holdCost))
  # print("   shortage cost ............. = {0:6.2f}: ".format(shortCost) + str(shortCost))
  # print("")
  sheet1.write(i,2,itemCost)
  sheet1.write(i,3,setupCost)
  sheet1.write(i,4,holdCost)
  sheet1.write(i,5,shortCost)
  
  dependentCost = setupCost + holdCost + shortCost
  totalCost = itemCost + setupCost + holdCost + shortCost
  # print("   dependent cost............. = {0:6.2f}".format(dependentCost))
  # print("   total cost................. = {0:6.2f}".format(totalCost))
  # print("")
  sheet1.write(i,7,dependentCost)
  sheet1.write(i,8,totalCost)

wb.save('Assignment1.xls') 
#C output:
# for 100 time intervals with an average demand of  29.29
# and policy parameters (s, S) = (20, 80)

#    average order ............ =  29.29
#    setup frequency .......... =   0.39
#    average holding level .... =  42.40
#    average shortage level ... =   0.25
