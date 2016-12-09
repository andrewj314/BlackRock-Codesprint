#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
//Simple function to convert a given portfolio allocation to the nearest tradeable amount
double toTradeableAmount(int alloc, int theIncrement, int minimumTrade){
    if((alloc - minimumTrade)%(theIncrement) == 0) return alloc;
    else{
    	int n = (alloc - minimumTrade)/theIncrement;
        return n*theIncrement + minimumTrade;
    }
}

int main() {

//read in security variables
    int T, minTrade, increment, available_units;
    cin >> T;
    cin >> minTrade >> increment >> available_units;
                                                
    //vector of mappings of portfolio IDs to orders
    vector< pair<string, double> > inputs;
    //vector of mappings of portfolio IDs to allocations
    vector< pair<string, double> > outputs;

    //loop over STDIN to populate inputs vector
    for(int i = 0; i < T; ++i){
    	string portfolio_id; double order;
    	cin >> portfolio_id >> order;
    	pair<string, double> portfolio_map;
        portfolio_map.first = portfolio_id;
        portfolio_map.second = order;
        inputs.push_back(portfolio_map);
    }
                                                                                                                             
    //get total order size
    int orderTotal = 0;
    for(int i = 0; i < inputs.size(); ++i){
        orderTotal += inputs.at(i).second;
    }
                                                                             
    //sort inputs vector in ascending order of order size. Equal orders are sorted alphabetically by portfolio ID
    sort(inputs.begin(), inputs.end(), [](const pair<string,double> &left, const pair<string,double> &right) 
    	{
        	if(left.second == right.second){  
            		return left.first < right.first;
        	}
        	else return left.second < right.second;
    	}
    );
                                                                                                                                                                                                                                                   
    for(int i = 0; i < inputs.size(); ++i){
        //calculate proportional allocation
        double proportional_alloc = (double)inputs.at(i).second/(double)orderTotal*(double)available_units;
        double actual_alloc;
        //case where the proportional allocation is greater than the order - all allocations adjusted to tradeable amounts
        if(proportional_alloc > inputs.at(i).second){
            actual_alloc = toTradeableAmount((int)inputs.at(i).second, increment, minTrade);
        }
        //case where minTrade < proportional allocation <= order
        else if(proportional_alloc > minTrade){
            actual_alloc = toTradeableAmount((int)proportional_alloc, increment, minTrade);
        }
        //case where minTrade/2 < proportional allocation
        else if(proportional_alloc > (double)minTrade/2.){
            actual_alloc = (double)minTrade;
        }
        else actual_alloc = 0;
        //if the portfolio is left with an untradeable amount, cancel the allocation!
        int leftover = (int)inputs.at(i).second - (int)actual_alloc;
        if(leftover != 0 && (leftover - minTrade)%increment != 0){
            actual_alloc = 0;
        }
        if(leftover < minTrade && leftover != 0) actual_alloc = 0;
        //populate the outputs vector with the determined allocation
        pair<string, int> output;
        output.first = inputs.at(i).first; output.second = actual_alloc;
        outputs.push_back(output);

        //decrement the total orders and available units appropriately
        orderTotal -= inputs.at(i).second;        
        available_units -= actual_alloc;       
    }

    //sort the outputs vector alphabetically
    sort(outputs.begin(), outputs.end(), [](const pair<string,double> &left, const pair<string,double> &right) {
    	return left.first < right.first;});
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
    //loop through outputs vector and print to STDOUT
    for(int i = 0; i < outputs.size(); ++i){
        cout << outputs.at(i).first << " " << outputs.at(i).second << "\n";
    }

    return 0;
}

