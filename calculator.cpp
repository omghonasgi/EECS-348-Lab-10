#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <cctype>

using namespace std;


bool is_valid(string val){

    int n = val.size();

    // Values to check the length of the input and if a decimal is seen
    int check = 0;
    int seen_decimal = 0;

    int i = 0;
    // Check for optional plus and minus signs
    if (val[i] == '+' || val[i] == '-'){
        if (!isdigit(val[i+1])){
            return false;
        }
        ++i;
        ++check;
        // cant only be a plus or minus
        if (i == n){
            return false;
        }
    }

    // check rest of the line
    for (; i < n; ++i){
        if (i == n-1 && val[i] == '.'){
            return false;
        }
        else if (i != 0 && val[i] == '.' && isdigit(val[i+1])){
            check++;
            seen_decimal++;
        }
        else if (isdigit(val[i])){
            check++;
        }
        else continue;
    }

    // if there is more than one decimal return false
    if (seen_decimal > 1){
        return false;
    }

    // if the check variable is as long as the string, return true
    if (check == n){
        return true;
    }
    return false;
}


string add_values(string num) {
    
    // Constant values for -123.456
    string const_int = "123";
    string const_dec = "456";
    bool const_negative = true;
    
    // Parse the input number
    bool num_negative = false;
    if (num[0] == '-') {
        num_negative = true;
        num.erase(0, 1);
    } else if (num[0] == '+') {
        num.erase(0, 1);
    }
    
    // Split into integer and decimal parts
    string num_int, num_dec;
    size_t dot_pos = num.find('.');
    if (dot_pos != string::npos) {
        num_int = num.substr(0, dot_pos);
        num_dec = num.substr(dot_pos + 1);
    } else {
        num_int = num;
        num_dec = "";
    }
    
    // Remove leading zeros from integer part (keep at least one)
    while (num_int.size() > 1 && num_int[0] == '0') {
        num_int.erase(0, 1);
    }
    
    // Pad decimal part to match length
    while (num_dec.size() < const_dec.size()) {
        num_dec += '0';
    }
    const_dec.resize(num_dec.size(), '0');
    
    // Case 1: Both numbers have the same sign
    if (num_negative == const_negative) {
        // Adding absolute values, then apply sign
        string res_int, res_dec;
        int carry = 0;
        
        // Add decimal parts
        for (int i = num_dec.size() - 1; i >= 0; i--) {
            int sum = (num_dec[i] - '0') + (const_dec[i] - '0') + carry;
            carry = sum / 10;
            res_dec = char(sum % 10 + '0') + res_dec;
        }
        
        // Add integer parts
        for (int i = num_int.size() - 1, j = const_int.size() - 1; i >= 0 || j >= 0 || carry; i--, j--) {
            int digit_num = (i >= 0) ? (num_int[i] - '0') : 0;
            int digit_const = (j >= 0) ? (const_int[j] - '0') : 0;
            int sum = digit_num + digit_const + carry;
            
            carry = sum / 10;
            res_int = char(sum % 10 + '0') + res_int;
        }
        
        // Remove trailing zeros from decimal part
        while (!res_dec.empty() && res_dec.back() == '0') {
            res_dec.pop_back();
        }
        
        // Construct result with sign
        string result = (num_negative ? "-" : "") + res_int;
        if (!res_dec.empty()) {
            result += "." + res_dec;
        }
        
        return result;
    }
    // Case 2: Numbers have different signs
    else {
        // Determine which number has larger absolute value to know the result sign
        bool num_larger;
        if (num_int.size() > const_int.size()) {
            num_larger = true;
        } else if (num_int.size() < const_int.size()) {
            num_larger = false;
        } else {
            // Same integer length, compare digit by digit
            if (num_int == const_int) {
                // Check decimal parts
                for (size_t i = 0; i < num_dec.size(); i++) {
                    if (num_dec[i] > const_dec[i]) {
                        num_larger = true;
                        break;
                    } else if (num_dec[i] < const_dec[i]) {
                        num_larger = false;
                        break;
                    }
                }
                // If all digits are equal, result is 0
                if (num_dec == const_dec) {
                    return "0";
                }
            } else {
                num_larger = (num_int > const_int);
            }
        }
        
        // Ensure we subtract smaller from larger
        string larger_int, larger_dec, smaller_int, smaller_dec;
        bool result_negative;
        
        if (num_larger) {
            larger_int = num_int;
            larger_dec = num_dec;
            smaller_int = const_int;
            smaller_dec = const_dec;
            result_negative = num_negative;
        } else {
            larger_int = const_int;
            larger_dec = const_dec;
            smaller_int = num_int;
            smaller_dec = num_dec;
            result_negative = const_negative;
        }
        
        // Perform subtraction
        string res_int, res_dec;
        int borrow = 0;
        
        // Subtract decimal parts
        for (int i = larger_dec.size() - 1; i >= 0; i--) {
            int digit_larger = larger_dec[i] - '0';
            int digit_smaller = smaller_dec[i] - '0';
            int diff = digit_larger - digit_smaller - borrow;
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            res_dec = char(diff + '0') + res_dec;
        }
        
        // Subtract integer parts
        for (int i = larger_int.size() - 1, j = smaller_int.size() - 1; i >= 0; i--, j--) {
            int digit_larger = larger_int[i] - '0';
            int digit_smaller = (j >= 0) ? (smaller_int[j] - '0') : 0;
            int diff = digit_larger - digit_smaller - borrow;
            
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            
            res_int = char(diff + '0') + res_int;
        }
        
        // Remove leading zeros from integer part (but keep at least one zero)
        while (res_int.size() > 1 && res_int[0] == '0') {
            res_int.erase(0, 1);
        }
        
        // Remove trailing zeros from decimal part
        while (!res_dec.empty() && res_dec.back() == '0') {
            res_dec.pop_back();
        }
        
        // Construct result
        string result = (result_negative ? "-" : "") + res_int;
        if (!res_dec.empty()) {
            result += "." + res_dec;
        }
        
        return result;
    }
}


int main(){
   string filename;
    cout << "Enter the name of the file: ";
    cin >> filename;
    
    ifstream myfile(filename);
   
    
    string line;
    
    
    while (getline(myfile, line)) {
        if (line.empty()) {
            continue;
        }
        if (is_valid(line)) {
            string result = add_values(line);
            cout << line << " + -123.456 = " << result << endl;
        } else {
            cout << "Invalid input: " << line << endl;
        }
    }
    
    myfile.close();
    return 0;
}    