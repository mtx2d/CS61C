 4. MapReduce

MapReduce Implementation
- Input to MapReduce is given by signature of map()
- emit(key k, value v) produces: key-value pair: (k, v)
	- Used to reformat the data to make it more workable
- for var in list can iterate through Iterables. Can also use hasNext(), next() functions
- int, float, String represent the possible datatypes commonly used
- intersection(list1, list2) --> returns elements shared by list1 & list2

4.1: Given set of coins and owner via list of CoinPairs, find number of coins for each denomination per person
- Want the number of each coin
CoinPair:
	String person
	String coinType

map(CoinPair pair):
	emit(pair, 1) # Produce unique key-value pairs: keys of person & coin types

reduce(CoinPair pair, Iterable<Int> count):
	total = 0
	for num in count:
		total += num
	emit(pair, total)
- We want to aggregate all the pairings through this iterable
- Iterating over the iterable gives us the total corresponding to the pair

4.2: Given output of the above, compute amount of money for each person
	 Use the valueOfCoin(String coinType) function which returns a float
	 corresponding to the value of a coin
- Unique key will correspond to a person (no longer will it include a coin type)

map(tuple<CoinPair, int> output):
	pair, count = output # Pair provides the CoinPair instance, count = # instances of that coin found
	emit(pair.person, count * valueOfCoin(pair.cointype)) # pair.person: provides the identifier, value: total dollar value of a person's coin
- Output of map is going to be a key: name, value: dollar value

reduce(String person, Iterable<float> values):
	total = 0
	for amount in values:
		total += amount
	emit(person, total) # Allows us to aggregate across all the coins for a specific person



5. SPARK: deals with resilient distributed datasets (RDD)
- Takes in an RDD and returns an RDD using map, flatMap, reduceByKey

5.1: Input: coin set & owner --> Output: number of coins of each denomination. Then compute total money
- Have access to valueOfCoin(coinType) which returns value of a specific coin
- Starting RDD: [(1, nickel), (1, dime), (1, nickel), (2, quarter), (2, quarter), (2, quarter)]
	- End: [(1, nickel, 2), (1, dime, 1), (2, quarter, 3)]
	- Intermediate: [(1, nickel, 1), (1, dime, 1)...] and aggregate across this
out1 = coinData.map(lambda (k1, k2): ((k1, k2), 1))
	- out1 produces the intermediate RDD --> for each key (person/coin) gets assigned 1
	- Want to combine the common person/coin entries
reduceByKey(lambda v1, v2: v1 + v2)
	- As long as the keys (person/coinType) are the same, they get added together

Goal 2: Find the total value of the coins for each person
- Want the following dataset: [(1, 500), (2, 300)...]
- We no longer need the cointype, but need to use the cointype to calculate value
- Intermediate dataset: [(1, 10), (1, 10), (2, 25)...]
	- Each entry corresponds to the person & the value of the coin

out2 = out1.map(lambda (k, v): (k[0], v * valueOfCoin(k[1]))) # k[0]: name, v: # of coins, k[1]: coinType
reduceByKey(lambda v1, v2: v1 + v2)
- Aggregates across all common keys (just adds them together)

5.2: Input: student name & course taken --> Output: name + GPA
- Starting dataset: [('Al', ('History', 3.0)), ('Al', ('Science', 4.0)), ('Bob', ('History', 3.7)),
					('Al', ('Math', 3.5))]
	- To get from the first to 2nd dataset, need number of courses taken per student
- Ending dataset: [('Al', 3.5), ('Bob', 3.7)]

studentData = sc.parallelize(students)
out = studentData.map(lambda (k, v): (k, (v.studentGrade, 1)))
--> [('Al', (3.0, 1)), ('Al', (4.0, 1)), ('Bob', (3.7, 1)), ('Al', (3.5, 1))]
reduceByKey(lambda v1, v2: v1[0] + v2[0], v1[1] + v2[1])
- This effectively aggregates by the values stored in the tuples corresponding to the keys
--> [('Al', (13.5, 3)), ('Bob', 3.7, 1)]
map(lambda (k, v): (k, val[0]/val[1]))
- Returns the desired key-value pair


6. Warehouse Scale Computing (WSC)
- Stats: Google has 1 million servers, each draws 200W (0.2kW), PUE = 1.5, $0.06 per KW-hr

Calculate power bill annually
- 1.5 * 10^6 servers * 0.2kW/server * $0.06/(KW*hr) * 8760 hr/year to yield the total
- 15 represents the power usage efficiency (equals 1 if all power not used for cooling)

New Stats: 50k machine datacenter, PUE reduced from 1.5 to 1.25. Calculate annual savings
- (1.5-1.25) * 50000 servers * 0.2kW/server * $0.06/(KW*hr) * 8760 hr/year 

