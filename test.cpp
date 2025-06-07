// #include <iostream>
// #include <string>
// #include <vector>
// #include <stack>
// #include <map>
// #include <unordered_set>
// #include <functional>
// #include <algorithm>
// #include <cctype>
// #include <sstream>
// #include <string_view>

// using namespace std;

// // Class expr (giữ nguyên, không thay đổi)
// class expr {
// private:
//     string expression;
// protected:
//     int precedence(char op) {
//         return (op == '+' ? 1 : (op == '*' ? 2 : 0));
//     }

//     string apply_operator(const string& a, const string& b, char op) {
//         return a + op + b;
//     }

//     void process(stack<string>& operands, stack<char>& operators, function<char(char)> op_map) {
//         char op = operators.top(); 
//         operators.pop();
//         string b = operands.top(); 
//         operands.pop();
//         string a = operands.top(); 
//         operands.pop();
//         char mapped_op = op_map(op);
//         string result = apply_operator(a, b, mapped_op);
//         this->steps.push_back(result);
//         operands.push(result);
//     }
// public:
//     vector<string> steps;

//     void evaluate_expression(bool invert = false) {
//         stack<string> operands;
//         stack<char> operators;
//         auto op_map = invert ? [](char op) { return (op == '+') ? '*' : (op == '*') ? '+' : op; } 
//                              : [](char op) { return op; };
//         for (size_t i = 0; i < this->expression.size(); ++i) {
//             char token = this->expression[i];
//             if (isalpha(token)) {
//                 operands.push(string(1, token));
//             } else if (token == '+' || token == '*') {
//                 while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
//                     process(operands, operators, op_map);
//                 }
//                 operators.push(token);
//             } else if (token == '(') {
//                 operators.push(token);
//             } else if (token == ')') {
//                 while (!operators.empty() && operators.top() != '(') {
//                     process(operands, operators, op_map);
//                 }
//                 if (!operators.empty()) operators.pop();
//             }
//         }
//         while (!operators.empty()) {
//             process(operands, operators, op_map);
//         }
//     }

//     friend istream& operator>>(istream& is, expr& l) {
//         string value;
//         if (is >> value) {
//             l.expression = value;
//         }
//         return is;
//     }

//     friend ostream& operator<<(ostream& os, const expr& l) {
//         for (const auto s : l.steps) {
//             os << "\"" << s << "\"" << endl;
//         }
//         return os;
//     }
// };

// // Class graph (cập nhật addNode và addEdge)
// class graph {
// public:
//     vector<pair<string, vector<string>>> adjList; // Lưu danh sách kề với thứ tự chèn
//     unordered_set<string> nodeSet; // Theo dõi các node đã thêm

//     // Hàm tiện ích để kiểm tra định dạng node
//     bool isValidNode(const string& node) {
//         // Kiểm tra node không rỗng
//         if (node.empty()) {
//             cout << "Error: Node is empty" << endl;
//             return false;
//         }
//         // Kiểm tra định dạng: <ký_tự>S hoặc <ký_tự>D
//         if (node.length() < 2) {
//             cout << "Error: Node " << node << " is too short" << endl;
//             return false;
//         }
//         if (node.back() != 'S' && node.back() != 'D') {
//             cout << "Error: Node " << node << " must end with 'S' or 'D'" << endl;
//             return false;
//         }
//         if (!isalpha(node[0])) {
//             cout << "Error: Node " << node << " must start with a letter" << endl;
//             return false;
//         }
//         return true;
//     }

//     void printGraph() {
//         vector<pair<string, vector<string>>> sorted_adjList = adjList;
//         sort(sorted_adjList.begin(), sorted_adjList.end(),
//             [](const auto& a, const auto& b) { return a.first < b.first; });

//         for (const auto& entry : sorted_adjList) {
//             cout << entry.first << ": ";
//             vector<string> sorted_neighbors = entry.second;
//             sort(sorted_neighbors.begin(), sorted_neighbors.end());
//             for (const auto& neighbor : sorted_neighbors) {
//                 cout << neighbor << " ";
//             }
//             cout << endl;
//         }
//     }

//     void addNode(const string& node) {
//         // Kiểm tra tính chính xác của node
//         if (!isValidNode(node)) {
//             cout << "Failed to add node: " << node << endl;
//             return;
//         }
//         // Kiểm tra node chưa tồn tại
//         if (nodeSet.find(node) == nodeSet.end()) {
//             adjList.emplace_back(node, vector<string>());
//             nodeSet.insert(node);
//             cout << "Added node: " << node << endl; // Debug
//         } else {
//             cout << "Node " << node << " already exists, skipping" << endl; // Debug
//         }
//     }
    
//     void addEdge(const string& u, const string& v) {
//         // Kiểm tra tính chính xác của u và v
//         if (!isValidNode(u) || !isValidNode(v)) {
//             cout << "Failed to add edge: Invalid nodes " << u << " or " << v << endl;
//             return;
//         }
//         // Kiểm tra u và v khác nhau
//         if (u == v) {
//             cout << "Error: Cannot add edge from " << u << " to itself" << endl;
//             return;
//         }
//         // Đảm bảo node tồn tại
//         addNode(u);
//         addNode(v);
//         // Kiểm tra cạnh chưa tồn tại
//         if (!hasEdge(u, v)) {
//             for (auto& entry : adjList) {
//                 if (entry.first == u) entry.second.push_back(v);
//                 if (entry.first == v) entry.second.push_back(u);
//             }
//             cout << "Added edge: " << u << " - " << v << endl; // Debug
//         } else {
//             cout << "Edge " << u << " - " << v << " already exists, skipping" << endl; // Debug
//         }
//     }

//     void removeEdge(const string& u, const string& v) {
//         if (hasNode(u)) {
//             for (auto& entry : adjList) {
//                 if (entry.first == u) {
//                     entry.second.erase(remove(entry.second.begin(), entry.second.end(), v), entry.second.end());
//                 }
//             }
//         }
//         if (hasNode(v)) {
//             for (auto& entry : adjList) {
//                 if (entry.first == v) {
//                     entry.second.erase(remove(entry.second.begin(), entry.second.end(), u), entry.second.end());
//                 }
//             }
//         }
//     }

//     bool hasNode(const string& node) {
//         return nodeSet.find(node) != nodeSet.end();
//     }

//     bool hasEdge(const string& u, const string& v) {
//         if (!hasNode(u)) return false;
//         for (const auto& entry : adjList) {
//             if (entry.first == u) {
//                 return find(entry.second.begin(), entry.second.end(), v) != entry.second.end();
//             }
//         }
//         return false;
//     }

//     int degree(const string& node) {
//         if (!hasNode(node)) return 0;
//         for (const auto& entry : adjList) {
//             if (entry.first == node) {
//                 return entry.second.size();
//             }
//         }
//         return 0;
//     }

//     vector<string> neighbors(const string& node) {
//         if (!hasNode(node)) return vector<string>();
//         for (const auto& entry : adjList) {
//             if (entry.first == node) {
//                 return entry.second;
//             }
//         }
//         return vector<string>();
//     }

//     void createNode(const string& node) {
//         string node1 = node + "S";
//         string node2 = node + "D";
//         addNode(node1);
//         addNode(node2);
//         addEdge(node1, node2);
//     }

//     void addEdgeParallel(int type, const string& node1 = "", const string& node2 = "", const vector<string>& inter_arr = {}, int mode = 0) {
//         if (type == 1) {
//             string n1 = node1 + "S";
//             string n2 = node1 + "D";
//             string n3 = node2 + "S";
//             string n4 = node2 + "D";
//             addEdge(n1, n3);
//             addEdge(n2, n4);
//         } else if (type == 2) {
//             string n1 = node1 + "S";
//             string n2 = node1 + "D";
//             string outside_node;
//             for (const auto& entry : adjList) {
//                 string n_first = entry.first.substr(0, 1);
//                 if (find(inter_arr.begin(), inter_arr.end(), n_first) == inter_arr.end() && n_first != node1) {
//                     outside_node = n_first;
//                     break;
//                 }
//             }
//             cout << "outside_node: " << outside_node << endl;
//             string node1_deg = lowestDegree("S", node1, node1, outside_node);
//             string node2_deg = lowestDegree("D", node1, node1, outside_node);
//             int node1_degree = degree(node1_deg);
//             int node2_degree = degree(node2_deg);
//             cout << "node1_deg: " << node1_deg << endl;
//             cout << "node2_deg: " << node2_deg << endl;
//             cout << "node1_degree: " << node1_degree << endl;
//             cout << "node2_degree: " << node2_degree << endl;
//             int s = (node1_degree < node2_degree) ? 1 : 2;
//             if (s == 1) {
//                 addEdge(n1, node1_deg);
//                 string node1_first = node1_deg.substr(0, 1);
//                 string node_connect = lowestDegree("D", node1, node1_first, outside_node);
//                 addEdge(n2, node_connect);
//             } else {
//                 addEdge(n2, node2_deg);
//                 string node2_first = node2_deg.substr(0, 1);
//                 string node_connect = lowestDegree("S", node1, node2_first, outside_node);
//                 addEdge(n1, node_connect);
//             }
//         } else if (type == 3) {
//             int i = 0, j = 0;
//             int n1_degree, n2_degree, n3_degree, n4_degree;
//             string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
//             int t1 = i;
//             string n2 = lowestDegreeArrNode("S", i, inter_arr, "", n2_degree);
//             string n3 = lowestDegreeArrNode("D", j, inter_arr, "", n3_degree);
//             string n4 = lowestDegreeArrNode("D", j, inter_arr, "", n4_degree);

//             cout << "n1 = " << n1 << endl;
//             cout << "n2 = " << n2 << endl;
//             cout << "n3 = " << n3 << endl;
//             cout << "n4 = " << n4 << endl;
//             cout << "n1_degree = " << n1_degree << endl;
//             cout << "n2_degree = " << n2_degree << endl;
//             cout << "n3_degree = " << n3_degree << endl;
//             cout << "n4_degree = " << n4_degree << endl;
//             cout << "checkingParallelConnected(\"D\", inter_arr) = " << checkingParallelConnected("D", inter_arr) << endl;
//             cout << "checkingParallelConnected(\"S\", inter_arr) = " << checkingParallelConnected("S", inter_arr) << endl;

//             int sel = 3;
//             if (n1_degree + n2_degree < n3_degree + n4_degree) {
//                 if ((checkingParallelConnected("S", inter_arr) && mode == 1) || mode == 0) {
//                     sel = 0;
//                     addEdge(n1, n2);
//                 }
//             } else {
//                 if ((checkingParallelConnected("D", inter_arr) && mode == 1) || mode == 0) {
//                     sel = 1;
//                     addEdge(n3, n4);
//                 }
//             }
//             if (sel == 1) {
//                 string except_node = n4.substr(0, 1);
//                 int temp_degree;
//                 string node_connect = lowestDegreeArrNode("S", t1, inter_arr, except_node, temp_degree);
//                 cout << "node_connect = " << node_connect << endl;
//                 addEdge(n1, node_connect);
//             } else if (sel == 2) {
//                 string except_node = n2.substr(0, 1);
//                 int temp_degree;
//                 string node_connect = lowestDegreeArrNode("D", t1, inter_arr, except_node, temp_degree);
//                 cout << "node_connect = " << node_connect << endl;
//                 addEdge(n3, node_connect);
//             }
//         }
//     }

//     void addEdgeSerial(int type, const string& node1 = "", const string& node2 = "", const vector<string>& inter_arr = {}, int mode = 0) {
//         if (type == 1) {
//             string n1 = node1 + "S";
//             string n2 = node1 + "D";
//             string n3 = node2 + "S";
//             string n4 = node2 + "D";
//             vector<string> arr = {node1, "0", node2, "0"};
//             if (mode == 0) {
//                 addEdge(n3, n2);
//             } else if (mode == 1 && (checkSerialConnected(" ", arr) == true)) {
//                 addEdge(n3, n2);
//             }
//         } else if (type == 2) {
//             string n1 = node1 + "S";
//             string n2 = node1 + "D";
//             int i = 0, j = 0;
//             int degree1, degree2;
//             string node_connect1 = lowestDegreeArrNode("D", i, inter_arr, "", degree1);
//             i = degree1;
//             string node_connect2 = lowestDegreeArrNode("S", j, inter_arr, "", degree2);
//             cout << "node_connect1 = " << node_connect1 << endl;
//             cout << "node_connect2 = " << node_connect2 << endl;
//             cout << "checkSerialConnected(node1, inter_arr) = " << checkSerialConnected(node1, inter_arr) << endl;
//             if (mode == 0) {
//                 addEdge(n1, node_connect1);
//                 for (const auto& entry : adjList) {
//                     if (entry.first[0] == node_connect1[0]) continue;
//                     if (hasEdge(node_connect1, entry.first) && entry.first[1] == 'D') {
//                         addEdge(n1, entry.first);
//                     }
//                 }
//             } else if (mode == 1 && checkSerialConnected(node1, inter_arr)) {
//                 addEdge(n2, node_connect2);
//             }
//         } else if (type == 3) {
//             int i = 0;
//             int n1_degree, n2_degree;
//             string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
//             string n2 = lowestDegreeArrNode("D", i, inter_arr, n1, n2_degree);
//             if (mode == 0) {
//                 addEdge(n1, n2);
//                 for (const auto& entry : adjList) {
//                     if (hasEdge(entry.first, n1) && entry.first[1] == 'S') {
//                         addEdge(entry.first, n2);
//                     }
//                     if (hasEdge(entry.first, n2) && entry.first[1] == 'D') {
//                         addEdge(entry.first, n1);
//                     }
//                 }
//             } else if (mode == 1 && checkSerialConnected("", inter_arr)) {
//                 addEdge(n1, n2);
//             }
//         }
//     }

//     string lowestDegreeArrNode(const string& k, int& i, const vector<string>& inter_arr, const string& except_node, int& degree) {
//         int t = i;
//         int min_degree = 100;
//         string min_node;
//         while (t < inter_arr.size() && inter_arr[t] != "0") {
//             string n1 = inter_arr[t] + k;
//             int n1_degree = this->degree(n1);
//             if (n1_degree < min_degree && n1 != except_node) {
//                 min_node = n1;
//                 min_degree = n1_degree;
//             }
//             t++;
//         }
//         i = t + 1;
//         degree = min_degree;
//         return min_node;
//     }

//     string lowestDegree(const string& k, const string& except_node1, const string& except_node2, const string& except_node3) {
//         int min_degree = 100;
//         string min_node;
//         for (auto it = adjList.rbegin(); it != adjList.rend(); ++it) {
//             const auto& entry = *it;
//             if (entry.first[entry.first.size() - 1] != k[0]) continue;
//             if (entry.first.substr(0, 1) == except_node1 || entry.first.substr(0, 1) == except_node2 || entry.first.substr(0, 1) == except_node3) continue;
//             int deg = degree(entry.first);
//             if (deg < min_degree) {
//                 min_degree = deg;
//                 min_node = entry.first;
//             }
//         }
//         return min_node;
//     }

//     bool checkSerialConnected(const string& token, const vector<string>& inter_arr) {
//         if (inter_arr.empty()) return true;
//         vector<string> arr1, arr2;
//         size_t i = 0, j = 0;
//         while (i < inter_arr.size() && inter_arr[i] != "0") {
//             arr1.push_back(inter_arr[i]);
//             i++;
//         }
//         i++;
//         while (i < inter_arr.size() && inter_arr[i] != "0") {
//             arr2.push_back(inter_arr[i]);
//             i++;
//             j++;
//         }
        
//         if (arr2.empty()) {
//             string n1_s = token + "S";
//             string n1_d = token + "D";
//             for (size_t k = 0; k < arr1.size(); k++) {
//                 if (arr1[k] == "0") break;
//                 string n2_s = arr1[k] + "S";
//                 string n2_d = arr1[k] + "D";
//                 if (hasEdge(n1_s, n2_d) || hasEdge(n1_d, n2_s)) return false;
//             }
//             return true;
//         } else {
//             for (size_t k = 0; k < arr1.size(); k++) {
//                 if (arr1[k] == "0") break;
//                 string n1_s = arr1[k] + "S";
//                 string n1_d = arr1[k] + "D";
//                 for (size_t m = 0; m < arr2.size(); m++) {
//                     if (arr2[m] == "0") break;
//                     string n2_s = arr2[m] + "S";
//                     string n2_d = arr2[m] + "D";
//                     if (hasEdge(n1_s, n2_d) || hasEdge(n1_d, n2_s)) return false;
//                 }
//             }
//             return true;
//         }
//     }

//     bool checkingParallelConnected(const string& k, const vector<string>& inter_arr) {
//         if (inter_arr.empty()) return true;
//         vector<string> arr1, arr2;
//         size_t i = 0, j = 0;
//         while (i < inter_arr.size() && inter_arr[i] != "0") {
//             arr1.push_back(inter_arr[i]);
//             i++;
//         }
//         i++;
//         while (i < inter_arr.size() && inter_arr[i] != "0") {
//             arr2.push_back(inter_arr[i]);
//             i++;
//             j++;
//         }
//         if (arr2.empty()) {
//             for (size_t m = 0; m < arr1.size() - 1; m++) {
//                 string n1 = arr1[m] + k;
//                 for (size_t n = m + 1; n < arr1.size(); n++) {
//                     string n2 = arr1[n] + k;
//                     if (hasEdge(n1, n2)) return false;
//                 }
//             }
//             return true;
//         } else {
//             for (size_t m = 0; m < arr1.size(); m++) {
//                 string n1 = arr1[m] + k;
//                 for (size_t n = 0; n < arr2.size(); n++) {
//                     string n2 = arr2[n] + k;
//                     if (hasEdge(n1, n2)) return false;
//                 }
//             }
//             return true;
//         }
//     }

//     void intersect_subtract(string_view expr1, string_view expr2, vector<string>& inter_arr, string& remain) {
//         remain.clear();
//         size_t i = 0, j = 0;
//         while (i < expr1.size()) {
//             if (j < expr2.size() && expr1[i] == expr2[j]) {
//                 if (isalpha(expr1[i])) inter_arr.emplace_back(1, expr1[i]);
//                 ++j;
//             } else {
//                 remain += expr1[i];
//             }
//             ++i;
//         }
//         if (!inter_arr.empty() && inter_arr.back() != "0")
//             inter_arr.emplace_back("0");
//     }

//     void createGraph(vector<string>& q, int i, const string& expression, int mode, string& end_node) {
//         vector<string> inter_arr;
//         string remain;
//         string expre = expression;
//         int t = i - 1;
//         while (t >= 0) {
//             string_view element = q[t];
//             intersect_subtract(expre, element, inter_arr, remain);
//             if (!remain.empty()) {
//                 expre = remain;
//             }
//             t--;
//             cout << "Intersection arr: ";
//             for (string str: inter_arr) {
//                 cout << str << " ";
//             }
//             cout << endl;
//             cout << "Remain: " << expre << endl;
//         }
//         if (expre == expression) {
//             string node1, node2;
//             char operator_char = 0;
//             for (char token : expression) {
//                 if (isalpha(token)) {
//                     if (node1.empty()) {
//                         node1 = string(1, token);
//                         createNode(node1);
//                     } else {
//                         node2 = string(1, token);
//                         createNode(node2);
//                     }
//                 } else if (token == '+' || token == '*') {
//                     operator_char = token;
//                 }
//             }
//             if (operator_char == '+') {
//                 addEdgeParallel(1, node1, node2, {}, mode);
//             } else if (operator_char == '*') {
//                 addEdgeSerial(1, node1, node2, inter_arr, mode);
//             }
//         } else {
//             string node;
//             char operator_char = 0;
//             for (char token : expre) {
//                 if (token == '+' || token == '*') {
//                     operator_char = token;
//                 } else if (isalpha(token)) {
//                     node = string(1, token);
//                     createNode(node);
//                 }
//             }
//             if (((operator_char == '+') ? 1 : 2) == 1) {
//                 if (!node.empty()) {
//                     addEdgeParallel(2, node, "", inter_arr, mode);
//                 } else {
//                     addEdgeParallel(3, "", "", inter_arr, mode);
//                 }
//             } else {
//                 if (!node.empty()) {
//                     addEdgeSerial(2, node, "", inter_arr, mode);
//                 } else {
//                     addEdgeSerial(3, "", "", inter_arr, mode);
//                 }
//             }
//             if (!remain.empty()) {
//                 expre = remain;
//                 if (isalpha(remain.back())) {
//                     end_node = string(1, remain.back());
//                 }
//             }
//         }
//     }

//     void createNmos(const string& expression, string& end_node) {
//         expr e;
//         stringstream(expression) >> e;
//         e.evaluate_expression(false);
//         vector<string> q = e.steps;
//         cout << "NMOS steps: ";
//         for (string str: q) {
//             cout << str << " ";
//         }
//         cout << endl;
//         for (size_t i = 0; i < q.size(); i++) {
//             createGraph(q, i, q[i], 0, end_node);
//         }
//     }

//     void createPmos(const string& expression, const vector<string>& euler_path) {
//         expr e;
//         stringstream(expression) >> e;
//         e.evaluate_expression(true);
//         vector<string> q = e.steps;
//         cout << "PMOS steps: ";
//         for (string str: q) {
//             cout << str << " ";
//         }
//         cout << endl;
//         cout << "PMOS euler path: ";
//         for (string str: euler_path) {
//             cout << str << " ";
//         }
//         cout << endl;
//         for (size_t i = 0; i < euler_path.size(); i++) {
//             cout << "Node to add: " << euler_path[i] << endl;
//             addNode(euler_path[i]);
//             if (i != 0) {
//                 addEdge(euler_path[i], euler_path[i - 1]);
//             }
//         }
//         cout << "PMOS graph before: " << endl;
//         printGraph();
//         for (size_t i = 0; i < q.size(); i++) {
//             string dummy;
//             createGraph(q, i, q[i], 1, dummy);
//             cout << "PMOS graph after stage " << i << ": " << endl;
//             printGraph();
//         }
//     }

//     bool isValidNextNode(const string& v, const vector<string>& path) {
//         if (find(path.begin(), path.end(), v) != path.end()) return false;
//         if (path.empty()) return true;
//         string last_node = path.back();
//         if (last_node[0] != v[0]) {
//             if (last_node[1] == 'S') {
//                 string x = last_node[0] + string("D");
//                 return find(path.begin(), path.end(), x) != path.end();
//             }
//             if (last_node[1] == 'D') {
//                 string x = last_node[0] + string("S");
//                 return find(path.begin(), path.end(), x) != path.end();
//             }
//         }
//         for (const auto& entry : adjList) {
//             if (entry.first == last_node) {
//                 return find(entry.second.begin(), entry.second.end(), v) != entry.second.end();
//             }
//         }
//         return false;
//     }

//     vector<string> hamiltonianDfsEndnode(const string& start, const string& end_node, vector<string> path = {}) {
//         path.push_back(start);
//         if (path.size() == adjList.size()) {
//             return path;
//         }
//         for (const auto& entry : adjList) {
//             if (entry.first == start) {
//                 for (const string& v : entry.second) {
//                     if (isValidNextNode(v, path)) {
//                         vector<string> new_path = hamiltonianDfsEndnode(v, end_node, path);
//                         if (!new_path.empty()) return new_path;
//                     }
//                 }
//             }
//         }
//         return {};
//     }

//     vector<string> hamiltonianDfs(const string& start, vector<string> path = {}) {
//         path.push_back(start);
//         if (path.size() == adjList.size()) {
//             return path;
//         }
//         for (const auto& entry : adjList) {
//             if (entry.first == start) {
//                 for (const string& v : entry.second) {
//                     if (isValidNextNode(v, path)) {
//                         vector<string> new_path = hamiltonianDfs(v, path);
//                         if (!new_path.empty()) return new_path;
//                     }
//                 }
//             }
//         }
//         return {};
//     }

//     vector<string> findHamiltonPath(const string& end_node) {
//         vector<string> path;
//         for (const auto& entry : adjList) {
//             if (entry.first[1] == 'S') continue;
//             if (!end_node.empty()) {
//                 path = hamiltonianDfsEndnode(entry.first, end_node);
//                 if (!path.empty()) {
//                     if (path[0][0] != end_node[0] && path.back()[0] != end_node[0]) {
//                         path = {};
//                     }
//                 }
//             } else {
//                 path = hamiltonianDfs(entry.first);
//             }
//             if (!path.empty()) break;
//         }
//         if (path.empty()) {
//             for (const auto& entry : adjList) {
//                 if (entry.first[1] == 'D') continue;
//                 if (!end_node.empty()) {
//                     path = hamiltonianDfsEndnode(entry.first, end_node);
//                     if (!path.empty()) {
//                         if (path[0][0] != end_node[0] && path.back()[0] != end_node[0]) {
//                             path = {};
//                         }
//                     }
//                 } else {
//                     path = hamiltonianDfs(entry.first);
//                 }
//                 if (!path.empty()) break;
//             }
//         }
//         return path;
//     }

//     pair<vector<string>, vector<string>> eulerPath(const string& end_node) {
//         vector<string> euler_path_nmos = findHamiltonPath(end_node);
//         if (euler_path_nmos.empty()) return {{}, {}};
//         vector<string> euler_path_pmos(euler_path_nmos.size());
//         bool s = false;
//         for (size_t i = 0; i < euler_path_nmos.size() - 1; i += 2) {
//             if (s) {
//                 euler_path_pmos[i] = euler_path_nmos[i + 1];
//                 euler_path_pmos[i + 1] = euler_path_nmos[i];
//             } else {
//                 euler_path_pmos[i] = euler_path_nmos[i];
//                 euler_path_pmos[i + 1] = euler_path_nmos[i + 1];
//             }
//             s = !s;
//         }
//         return {euler_path_nmos, euler_path_pmos};
//     }

//     void checkAndAddParallelEdges(const vector<string>& serial_arr, const vector<string>& parallel_arr, const vector<string>& euler_path) {
//         map<string, vector<string>> parallel_graph;
//         vector<string> nodes;
//         for (const auto edge : parallel_arr) {
//             string n1 = edge.substr(0, 1);
//             string n2 = edge.substr(1, 1);
//             if (find(nodes.begin(), nodes.end(), n1) == nodes.end()) nodes.push_back(n1);
//             if (find(nodes.begin(), nodes.end(), n2) == nodes.end()) nodes.push_back(n2);
//             parallel_graph[n1].push_back(n2);
//             parallel_graph[n2].push_back(n1);
//         }

//         auto hasPath = [&](const string& start, const string& end, const vector<string>& component) {
//             vector<string> visited;
//             stack<string> stk;
//             stk.push(start);
//             visited.push_back(start);
//             while (!stk.empty()) {
//                 string curr = stk.top();
//                 stk.pop();
//                 if (curr == end) return true;
//                 for (const auto neighbor : parallel_graph[curr]) {
//                     if (find(component.begin(), component.end(), neighbor) != component.end() &&
//                         find(visited.begin(), visited.end(), neighbor) == visited.end()) {
//                         stk.push(neighbor);
//                         visited.push_back(neighbor);
//                     }
//                 }
//             }
//             return false;
//         };

//         vector<vector<string>> components;
//         vector<string> visited;
//         for (const auto node : nodes) {
//             if (find(visited.begin(), visited.end(), node) == visited.end()) {
//                 vector<string> component;
//                 stack<string> stk;
//                 stk.push(node);
//                 visited.push_back(node);
//                 while (!stk.empty()) {
//                     string curr = stk.top();
//                     stk.pop();
//                     component.push_back(curr);
//                     for (const auto neighbor : parallel_graph[curr]) {
//                         if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
//                             stk.push(neighbor);
//                             visited.push_back(neighbor);
//                         }
//                     }
//                 }
//                 if (component.size() >= 3) {
//                     components.push_back(component);
//                 }
//             }
//         }

//         for (const auto component : components) {
//             bool valid = true;
//             for (size_t i = 0; i < component.size() && valid; ++i) {
//                 for (size_t j = i + 1; j < component.size(); ++j) {
//                     string pair1 = component[i] + component[j];
//                     string pair2 = component[j] + component[i];
//                     if (find(serial_arr.begin(), serial_arr.end(), pair1) != serial_arr.end() ||
//                         find(serial_arr.begin(), serial_arr.end(), pair2) != serial_arr.end()) {
//                         valid = false;
//                         break;
//                     }
//                 }
//             }
//             if (!valid) continue;

//             for (const auto edge : parallel_arr) {
//                 string n1 = edge.substr(0, 1);
//                 string n2 = edge.substr(1, 1);
//                 if (find(component.begin(), component.end(), n1) == component.end() ||
//                     find(component.begin(), component.end(), n2) == component.end()) {
//                     continue;
//                 }
//                 if (!hasEdge(n1 + "S", n2 + "S")) {
//                     addEdge(n1 + "S", n2 + "S");
//                 }
//                 if (!hasEdge(n1 + "D", n2 + "D")) {
//                     addEdge(n1 + "D", n2 + "D");
//                 }
//             }
//         }
//     }

//     void filterEdgePmos(const vector<string>& serial_arr, const vector<string>& parallel_arr, const vector<string>& euler_path) {
//         vector<string> check_serial, check_parallel;
//         vector<string> edges_to_remove;
//         for (size_t i = 0; i < euler_path.size(); i++) {
//             if (i == 0) continue;
//             string n1 = euler_path[i];
//             string n2 = euler_path[i - 1];
//             if (n1[0] != n2[0] && n1[1] != n2[1]) {
//                 check_serial.push_back(n1.substr(0, 1) + n2.substr(0, 1));
//                 check_serial.push_back(n2.substr(0, 1) + n1.substr(0, 1));
//             }
//             if (n1[0] != n2[0] && n1[1] == n2[1]) {
//                 check_parallel.push_back(n1.substr(0, 1) + n2.substr(0, 1) + n1[1]);
//                 check_parallel.push_back(n2.substr(0, 1) + n1.substr(0, 1) + n1[1]);
//             }
//         }

//         cout << "check_serial: ";
//         for (auto str: check_serial) {
//             cout << str << " "; 
//         }
//         cout << endl;
//         cout << "check_parallel: ";
//         for (auto str: check_parallel) {
//             cout << str << " "; 
//         }
//         cout << endl;

//         for (const auto& entry : adjList) {
//             for (const string& neighbor : entry.second) {
//                 if (entry.first.empty() || neighbor.empty()) {
//                     continue;
//                 }
//                 string n1 = entry.first.substr(0, 1);
//                 string n2 = neighbor.substr(0, 1);
//                 if (n1 == n2) continue;
//                 string n3 = entry.first.substr(entry.first.size() - 1);
//                 string n4 = neighbor.substr(neighbor.size() - 1);
//                 cout << "n1: " << n1 << endl;
//                 cout << "n2: " << n2 << endl;
//                 cout << "n3: " << n3 << endl;
//                 cout << "n4: " << n4 << endl;
//                 if (n3 != n4) {
//                     string node1 = n1 + n2;
//                     string node2 = n2 + n1;
//                     int s = 0;
//                     if ((find(serial_arr.begin(), serial_arr.end(), node1) != serial_arr.end() || 
//                          find(serial_arr.begin(), serial_arr.end(), node2) != serial_arr.end()) &&
//                         (find(check_serial.begin(), check_serial.end(), node1) == check_serial.end() ||
//                          find(check_serial.begin(), check_serial.end(), node2) == check_serial.end())) {
//                         if (hasEdge(n1 + "S", n2 + "D")) {
//                             removeEdge(n1 + "S", n2 + "D");
//                             cout << "Removed edges 1: " << n1 + "S" << n2 + "D" << endl;
//                             edges_to_remove.push_back(n1 + "S" + n2 + "D");
//                             s = 1;
//                         }
//                         if (hasEdge(n1 + "D", n2 + "S")) {
//                             removeEdge(n1 + "D", n2 + "S");
//                             cout << "Removed edges 2: " << n1 + "D" << n2 + "S" << endl;
//                             edges_to_remove.push_back(n1 + "D" + n2 + "S");
//                             s = 2;
//                         }
//                         if (s == 1) {
//                             s = 0;
//                             for (const auto& n : adjList) {
//                                 cout << "Node here: " << n.first << endl;
//                                 if (n.first[1] == 'S' && hasEdge(n.first, n2 + "D")) {
//                                     if (find(edges_to_remove.begin(), edges_to_remove.end(), n1 + "S" + string(1, n.first[0]) + "D") == edges_to_remove.end())
//                                     {
//                                         addEdge(n1 + "S", string(1, n.first[0]) + "D");
//                                         cout << "Added edge 1: " << (n1 + "S") << (string(1, n.first[0]) + "D") << endl;
//                                         break;
//                                     }
//                                 }
//                                 if (n.first[1] == 'S' && hasEdge(n.first, n1 + "D")) {
//                                     if (find(edges_to_remove.begin(), edges_to_remove.end(), n2 + "S" + string(1, n.first[0]) + "D") == edges_to_remove.end())
//                                     {
//                                         addEdge(n2 + "S", string(1, n.first[0]) + "D");
//                                         cout << "Added edge 2: " << (n2 + "S") << (string(1, n.first[0]) + "D") << endl;
//                                         break;
//                                     }
//                                 }
//                             }
//                         } else if (s == 2) {
//                             s = 0;
//                             for (const auto& n : adjList) {
//                                 if (n.first[1] == 'D' && hasEdge(n.first, n2 + "S")) {
//                                     if (find(edges_to_remove.begin(), edges_to_remove.end(), n1 + "D" + (string(1, n.first[0]) + "S")) == edges_to_remove.end())
//                                     {
//                                         addEdge(n1 + "D", (string(1, n.first[0]) + "S"));
//                                         cout << "Added edge 3: " << (n1 + "D") << (string(1, n.first[0]) + "S") << endl;
//                                         break;
//                                     }
//                                 }
//                                 if (n.first[1] == 'D' && hasEdge(n.first, n1 + "S")) {
//                                     if (find(edges_to_remove.begin(), edges_to_remove.end(), n2 + "D" + string(1, n.first[0]) + "S") == edges_to_remove.end())
//                                     {
//                                         addEdge(n2 + "D", string(1, n.first[0]) + "S");
//                                         cout << "Added edge 4: " << (n2 + "D") << (string(1, n.first[0]) + "S") << endl;
//                                         break;
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 } else {
//                     string node1 = n1 + n2;
//                     string node2 = n2 + n1;
//                     if (find(parallel_arr.begin(), parallel_arr.end(), node1) == parallel_arr.end() &&
//                         find(parallel_arr.begin(), parallel_arr.end(), node2) == parallel_arr.end()) {
//                         for (const string& n : check_parallel) {
//                             if (node1 != n.substr(0, 2) && node2 != n.substr(0, 2) && n3 == string(1, n[2])) {
//                                 if (hasEdge(n1 + string(1, n[2]), n2 + string(1, n[2]))) {
//                                     cout << "Removed edge 3: " << (n1 + string(1, n[2])) << (n2 + string(1, n[2])) << endl;
//                                     removeEdge(n1 + string(1, n[2]), n2 + string(1, n[2]));
//                                     edges_to_remove.push_back(n1 + string(1, n[2]) + n2 + string(1, n[2]));
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//         checkAndAddParallelEdges(serial_arr, parallel_arr, euler_path);
//     }

//     bool checkingEdge(const string& full_node, const string& char_connected) {
//         for (const auto& entry : adjList) {
//             if (entry.first[0] == full_node[0]) continue;
//             if (char_connected.empty()) {
//                 if (hasEdge(full_node, entry.first)) return false;
//             } else if (entry.first[1] == char_connected[0]) {
//                 if (hasEdge(full_node, entry.first)) return false;
//             }
//         }
//         return true;
//     }

//     pair<vector<string>, vector<string>> findNodeSourceAndOut() {
//         vector<pair<string, string>> source_nodes, out_nodes;
//         for (const auto& entry : adjList) {
//             for (const string& neighbor : entry.second) {
//                 if (entry.first >= neighbor) continue;
//                 if (entry.first[0] == neighbor[0]) continue;
//                 if (entry.first[1] == neighbor[1]) {
//                     if (entry.first[1] == 'S') {
//                         source_nodes.emplace_back(entry.first.substr(0, 1), neighbor.substr(0, 1));
//                     } else {
//                         out_nodes.emplace_back(entry.first.substr(0, 1), neighbor.substr(0, 1));
//                     }
//                 }
//             }
//         }
//         vector<string> final_source_nodes, final_out_nodes;
//         if (!source_nodes.empty()) {
//             vector<pair<string, string>> temp = source_nodes;
//             for (auto it = temp.begin(); it != temp.end();) {
//                 string n1 = it->first, n2 = it->second;
//                 string n1_s = n1 + "S", n2_s = n2 + "S";
//                 if (!checkingEdge(n1_s, "D") || !checkingEdge(n2_s, "D")) {
//                     it = temp.erase(it);
//                 } else {
//                     ++it;
//                 }
//             }
//             for (const auto pair : temp) {
//                 final_source_nodes.push_back(pair.first + "S");
//                 final_source_nodes.push_back(pair.second + "S");
//             }
//         }
//         if (!out_nodes.empty()) {
//             vector<pair<string, string>> temp = out_nodes;
//             for (auto it = temp.begin(); it != temp.end();) {
//                 string n1 = it->first, n2 = it->second;
//                 string n1_d = n1 + "D", n2_d = n2 + "D";
//                 if (!checkingEdge(n1_d, "S") || !checkingEdge(n2_d, "S")) {
//                     it = temp.erase(it);
//                 } else {
//                     ++it;
//                 }
//             }
//             for (const auto pair : temp) {
//                 final_out_nodes.push_back(pair.first + "D");
//                 final_out_nodes.push_back(pair.second + "D");
//             }
//         }
//         if (final_source_nodes.empty()) {
//             for (const auto& entry : adjList) {
//                 if (entry.first[1] == 'S' && checkingEdge(entry.first, "")) {
//                     final_source_nodes.push_back(entry.first);
//                 }
//             }
//         }
//         if (final_out_nodes.empty()) {
//             for (const auto& entry : adjList) {
//                 if (entry.first[1] == 'D' && checkingEdge(entry.first, "")) {
//                     final_out_nodes.push_back(entry.first);
//                 }
//             }
//         }
//         return {final_source_nodes, final_out_nodes};
//     }
// };

// // Main function (giữ nguyên, không thay đổi)
// struct Result {
//     graph g_nmos, g_pmos;
//     vector<string> euler_path_nmos, euler_path_pmos;
//     vector<string> source_nodes_nmos, out_nodes_nmos;
//     vector<string> source_nodes_pmos, out_nodes_pmos;
// };

// Result CreateAll(const string& expression) {
//     Result result;
//     string end_node;
//     result.g_nmos.createNmos(expression, end_node);
//     auto [source_nodes_nmos, out_nodes_nmos] = result.g_nmos.findNodeSourceAndOut();
//     vector<string> serial_array_pmos, parallel_array_pmos;
//     for (const auto& entry : result.g_nmos.adjList) {
//         for (const string& neighbor : entry.second) {
//             if (entry.first >= neighbor) continue;
//             string n1 = entry.first.substr(0, 1);
//             string n2 = neighbor.substr(0, 1);
//             if (n1 == n2) continue;
//             string n3 = string(1, entry.first[entry.first.size() - 1]);
//             string n4 = string(1, neighbor[neighbor.size() - 1]);
//             if (n3 == n4) {
//                 serial_array_pmos.push_back(n1 + n2);
//             } else {
//                 parallel_array_pmos.push_back(n1 + n2);
//             }
//         }
//     }

//     cout << "end_node = " << end_node << endl;

//     auto [euler_path_nmos, euler_path_pmos] = result.g_nmos.eulerPath(end_node);
//     cout << "NMOS euler path: ";
//     for (string str: euler_path_nmos) {
//         cout << str << " ";
//     }
//     cout << endl;
//     cout << "serial_array_pmos: ";
//     for (string str: serial_array_pmos) {
//         cout << str << " ";
//     }
//     cout << endl;
//     cout << "parallel_array_pmos: ";
//     for (string str: parallel_array_pmos) {
//         cout << str << " ";
//     }
//     cout << endl;
//     result.g_pmos.createPmos(expression, euler_path_pmos);
//     result.g_pmos.filterEdgePmos(serial_array_pmos, parallel_array_pmos, euler_path_pmos);
//     cout << "\nPMOS Graph after filter:\n";
//     result.g_pmos.printGraph();
//     auto [source_nodes_pmos, out_nodes_pmos] = result.g_pmos.findNodeSourceAndOut();
//     result.euler_path_nmos = euler_path_nmos;
//     result.euler_path_pmos = euler_path_pmos;
//     result.source_nodes_nmos = source_nodes_nmos;
//     result.out_nodes_nmos = out_nodes_nmos;
//     result.source_nodes_pmos = source_nodes_pmos;
//     result.out_nodes_pmos = out_nodes_pmos;
//     return result;
// }

// string removeSpaces(const string& str) {
//     string result;
//     for (char c : str) {
//         if (!isspace(static_cast<unsigned char>(c))) {
//             result += c;
//         }
//     }
//     return result;
// }

// int main() {
//     string str;
//     string expression;
//     cout << "Enter expression: ";
//     getline(cin, str);
//     expression = removeSpaces(str);
//     cout << "Expression after removing spaces: " << expression << endl;
//     Result result = CreateAll(expression);
//     cout << endl;
//     cout << "NMOS Graph:\n";
//     result.g_nmos.printGraph();
//     cout << "\nPMOS Graph:\n";
//     result.g_pmos.printGraph();
//     cout << "\nEuler Path NMOS: ";
//     for (const auto node : result.euler_path_nmos) cout << node << " ";
//     cout << "\nEuler Path PMOS: ";
//     for (const auto node : result.euler_path_pmos) cout << node << " ";
//     cout << "\nSource Nodes NMOS: ";
//     for (const auto node : result.source_nodes_nmos) cout << node << " ";
//     cout << "\nOut Nodes NMOS: ";
//     for (const auto node : result.out_nodes_nmos) cout << node << " ";
//     cout << "\nSource Nodes PMOS: ";
//     for (const auto node : result.source_nodes_pmos) cout << node << " ";
//     cout << "\nOut Nodes PMOS: ";
//     for (const auto node : result.out_nodes_pmos) cout << node << " ";
//     cout << endl;
//     return 0;
// }

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <string_view>

using namespace std;

// Class expr
class expr {
private:
    string expression;
protected:
    int precedence(char op) {
        return (op == '+' ? 1 : (op == '*' ? 2 : 0));
    }

    string apply_operator(const string& a, const string& b, char op) {
        return a + op + b;
    }

    void process(stack<string>& operands, stack<char>& operators, function<char(char)> op_map) {
        char op = operators.top(); 
        operators.pop();
        string b = operands.top(); 
        operands.pop();
        string a = operands.top(); 
        operands.pop();
        char mapped_op = op_map(op);
        string result = apply_operator(a, b, mapped_op);
        this->steps.push_back(result);
        operands.push(result);
    }
public:
    vector<string> steps;

    void evaluate_expression(bool invert = false) {
        stack<string> operands;
        stack<char> operators;
        auto op_map = invert ? [](char op) { return (op == '+') ? '*' : (op == '*') ? '+' : op; } 
                             : [](char op) { return op; };
        for (size_t i = 0; i < this->expression.size(); ++i) {
            char token = this->expression[i];
            if (isalpha(token)) {
                operands.push(string(1, token));
            } else if (token == '+' || token == '*') {
                while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                    process(operands, operators, op_map);
                }
                operators.push(token);
            } else if (token == '(') {
                operators.push(token);
            } else if (token == ')') {
                while (!operators.empty() && operators.top() != '(') {
                    process(operands, operators, op_map);
                }
                if (!operators.empty()) operators.pop();
            }
        }
        while (!operators.empty()) {
            process(operands, operators, op_map);
        }
    }

    friend istream& operator>>(istream& is, expr& l) {
        string value;
        if (is >> value) {
            l.expression = value;
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const expr& l) {
        for (const auto s : l.steps) {
            os << "\"" << s << "\"" << endl;
        }
        return os;
    }
};

// Class graph
class graph {
public:
    vector<pair<string, vector<string>>> adjList; // Lưu danh sách kề với thứ tự chèn
    unordered_set<string> nodeSet; // Theo dõi các node đã thêm

    void printGraph() {
        // Tạo bản sao của adjList để sắp xếp
        vector<pair<string, vector<string>>> sorted_adjList = adjList;
        // Sắp xếp theo tên node (first) theo thứ tự alphabet
        sort(sorted_adjList.begin(), sorted_adjList.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        // In danh sách kề
        for (const auto& entry : sorted_adjList) {
            cout << entry.first << ": ";
            // Tạo bản sao danh sách hàng xóm và sắp xếp
            vector<string> sorted_neighbors = entry.second;
            sort(sorted_neighbors.begin(), sorted_neighbors.end());
            for (const auto& neighbor : sorted_neighbors) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    void addNode(const string& node) {
        if (nodeSet.find(node) == nodeSet.end()) {
            adjList.emplace_back(node, vector<string>());
            nodeSet.insert(node);
        }
    }
    
    void addEdge(const string& u, const string& v) {
        addNode(u);
        addNode(v);
        if (!hasEdge(u, v)) {
            for (auto& entry : adjList) {
                if (entry.first == u) entry.second.push_back(v);
                if (entry.first == v) entry.second.push_back(u);
            }
        }
    }

    void removeEdge(const string& u, const string& v) {
        if (hasNode(u)) {
            for (auto& entry : adjList) {
                if (entry.first == u) {
                    entry.second.erase(remove(entry.second.begin(), entry.second.end(), v), entry.second.end());
                }
            }
        }
        if (hasNode(v)) {
            for (auto& entry : adjList) {
                if (entry.first == v) {
                    entry.second.erase(remove(entry.second.begin(), entry.second.end(), u), entry.second.end());
                }
            }
        }
    }

    bool hasNode(const string& node) {
        return nodeSet.find(node) != nodeSet.end();
    }

    bool hasEdge(const string& u, const string& v) {
        if (!hasNode(u)) return false;
        for (const auto& entry : adjList) {
            if (entry.first == u) {
                return find(entry.second.begin(), entry.second.end(), v) != entry.second.end();
            }
        }
        return false;
    }

    int degree(const string& node) {
        if (!hasNode(node)) return 0;
        for (const auto& entry : adjList) {
            if (entry.first == node) {
                return entry.second.size();
            }
        }
        return 0;
    }

    vector<string> neighbors(const string& node) {
        if (!hasNode(node)) return vector<string>();
        for (const auto& entry : adjList) {
            if (entry.first == node) {
                return entry.second;
            }
        }
        return vector<string>();
    }

    void createNode(const string& node) {
        string node1 = node + "S";
        string node2 = node + "D";
        addNode(node1);
        addNode(node2);
        addEdge(node1, node2);
    }

    void addEdgeParallel(int type, const string& node1 = "", const string& node2 = "", const vector<string>& inter_arr = {}, int mode = 0) {
        if (type == 1) {
            string n1 = node1 + "S";
            string n2 = node1 + "D";
            string n3 = node2 + "S";
            string n4 = node2 + "D";
            addEdge(n1, n3);
            addEdge(n2, n4);
        } else if (type == 2) {
            string n1 = node1 + "S";
            string n2 = node1 + "D";
            string outside_node;
            for (const auto& entry : adjList) {
                string n_first = entry.first.substr(0, 1);
                if (find(inter_arr.begin(), inter_arr.end(), n_first) == inter_arr.end() && n_first != node1) {
                    outside_node = n_first;
                    break;
                }
            }
            cout << "outside_node: " << outside_node << endl;
            string node1_deg = lowestDegree("S", node1, node1, outside_node);
            string node2_deg = lowestDegree("D", node1, node1, outside_node);
            int node1_degree = degree(node1_deg);
            int node2_degree = degree(node2_deg);
            cout << "node1_deg: " << node1_deg << endl;
            cout << "node2_deg: " << node2_deg << endl;
            cout << "node1_degree: " << node1_degree << endl;
            cout << "node2_degree: " << node2_degree << endl;
            // int s = (node1_degree < node2_degree) ? 1 : 2;   // NVL temp
            int s = (node1_degree <= node2_degree) ? 1 : 2;
            if (s == 1) {
                addEdge(n1, node1_deg);
                string node1_first = node1_deg.substr(0, 1);
                string node_connect = lowestDegree("D", node1, "", outside_node);
                addEdge(n2, node_connect);
                cout << "n1: " << n1 << endl;
                cout << "node1_first: " << node1_first << endl;
                cout << "node_connect: " << node_connect << endl;
                cout << "n2: " << n2 << endl;
            } else {
                addEdge(n2, node2_deg);
                string node2_first = node2_deg.substr(0, 1);
                string node_connect = lowestDegree("S", node1, "", outside_node);
                addEdge(n1, node_connect);
            }
        } else if (type == 3) {
            int i = 0, j = 0;
            int n1_degree, n2_degree, n3_degree, n4_degree;
            string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
            int t1 = i;
            string n2 = lowestDegreeArrNode("S", i, inter_arr, "", n2_degree);
            string n3 = lowestDegreeArrNode("D", j, inter_arr, "", n3_degree);
            string n4 = lowestDegreeArrNode("D", j, inter_arr, "", n4_degree);

            cout << "n1 = " << n1 << endl;
            cout << "n2 = " << n2 << endl;
            cout << "n3 = " << n3 << endl;
            cout << "n4 = " << n4 << endl;
            cout << "n1_degree = " << n1_degree << endl;
            cout << "n2_degree = " << n2_degree << endl;
            cout << "n3_degree = " << n3_degree << endl;
            cout << "n4_degree = " << n4_degree << endl;
            cout << "checkingParallelConnected(\"D\", inter_arr) = " << checkingParallelConnected("D", inter_arr) << endl;
            cout << "checkingParallelConnected(\"S\", inter_arr) = " << checkingParallelConnected("S", inter_arr) << endl;

            int sel = 3;
            if (n1_degree + n2_degree < n3_degree + n4_degree) {
                if ((checkingParallelConnected("S", inter_arr) && mode == 1) || mode == 0) {
                    sel = 0;
                    addEdge(n1, n2);
                }
            } else {
                if ((checkingParallelConnected("D", inter_arr) && mode == 1) || mode == 0) {
                    sel = 1;
                    addEdge(n3, n4);
                }
            }
            if (sel == 1) {
                string except_node = n4.substr(0, 1);
                int temp_degree;
                string node_connect = lowestDegreeArrNode("S", t1, inter_arr, except_node, temp_degree);
                cout << "node_connect = " << node_connect << endl;
                addEdge(n1, node_connect);
            } else if (sel == 2) {
                string except_node = n2.substr(0, 1);
                int temp_degree;
                string node_connect = lowestDegreeArrNode("D", t1, inter_arr, except_node, temp_degree);
                cout << "node_connect = " << node_connect << endl;
                addEdge(n3, node_connect);
            }
        }
    }

    void addEdgeSerial(int type, const string& node1 = "", const string& node2 = "", const vector<string>& inter_arr = {}, int mode = 0) {
        if (type == 1) {
            string n1 = node1 + "S";
            string n2 = node1 + "D";
            string n3 = node2 + "S";
            string n4 = node2 + "D";
            vector<string> arr = {node1, "0", node2, "0"};
            if (mode == 0) {
                addEdge(n3, n2);
            } else if (mode == 1 && (checkSerialConnected(" ", arr) == true)) {
                addEdge(n3, n2);
            }
        } else if (type == 2) {
            string n1 = node1 + "S";
            string n2 = node1 + "D";
            int i = 0, j = 0;
            int degree1, degree2;
            string node_connect1 = lowestDegreeArrNode("D", i, inter_arr, "", degree1);
            i = degree1;
            string node_connect2 = lowestDegreeArrNode("S", j, inter_arr, "", degree2);
            cout << "node_connect1 = " << node_connect1 << endl;
            cout << "node_connect2 = " << node_connect2 << endl;
            cout << "checkSerialConnected(node1, inter_arr) = " << checkSerialConnected(node1, inter_arr) << endl;
            if (mode == 0) {
                addEdge(n1, node_connect1);
                for (const auto& entry : adjList) {
                    if (entry.first[0] == node_connect1[0]) continue;
                    if (hasEdge(node_connect1, entry.first) && entry.first[1] == 'D') {
                        addEdge(n1, entry.first);
                    }
                }
            } else if (mode == 1 && checkSerialConnected(node1, inter_arr)) {
                addEdge(n2, node_connect2);
            }
        } else if (type == 3) {
            int i = 0;
            int n1_degree, n2_degree;
            string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
            string n2 = lowestDegreeArrNode("D", i, inter_arr, n1, n2_degree);
            if (mode == 0) {
                addEdge(n1, n2);
                for (const auto& entry : adjList) {
                    if (hasEdge(entry.first, n1) && entry.first[1] == 'S') {
                        addEdge(entry.first, n2);
                    }
                    if (hasEdge(entry.first, n2) && entry.first[1] == 'D') {
                        addEdge(entry.first, n1);
                    }
                }
            } else if (mode == 1 && checkSerialConnected("", inter_arr)) {
                addEdge(n1, n2);
            }
        }
    }

    string lowestDegreeArrNode(const string& k, int& i, const vector<string>& inter_arr, const string& except_node, int& degree) {
        int t = i;
        int min_degree = 100;
        string min_node;
        while (t < inter_arr.size() && inter_arr[t] != "0") {
            string n1 = inter_arr[t] + k;
            int n1_degree = this->degree(n1);
            if (n1_degree < min_degree && n1 != except_node) {
                min_node = n1;
                min_degree = n1_degree;
            }
            t++;
        }
        i = t + 1;
        degree = min_degree;
        return min_node;
    }

    string lowestDegree(const string& k, const string& except_node1, const string& except_node2, const string& except_node3) {
        int min_degree = 100;
        string min_node;
        for (auto it = adjList.rbegin(); it != adjList.rend(); ++it) {
            const auto& entry = *it;
            if (entry.first[entry.first.size() - 1] != k[0]) continue;
            if (entry.first.substr(0, 1) == except_node1 || entry.first.substr(0, 1) == except_node2 || entry.first.substr(0, 1) == except_node3) continue;
            int deg = degree(entry.first);
            if (deg < min_degree) {
                min_degree = deg;
                min_node = entry.first;
            }
        }
        return min_node;
    }

    bool checkSerialConnected(const string& token, const vector<string>& inter_arr) {
        if (inter_arr.empty()) return true;
        vector<string> arr1, arr2;
        size_t i = 0, j = 0;
        while (i < inter_arr.size() && inter_arr[i] != "0") {
            arr1.push_back(inter_arr[i]);
            i++;
        }
        i++;
        while (i < inter_arr.size() && inter_arr[i] != "0") {
            arr2.push_back(inter_arr[i]);
            i++;
            j++;
        }
        
        if (arr2.empty()) {
            string n1_s = token + "S";
            string n1_d = token + "D";
            for (size_t k = 0; k < arr1.size(); k++) {
                if (arr1[k] == "0") break;
                string n2_s = arr1[k] + "S";
                string n2_d = arr1[k] + "D";
                if (hasEdge(n1_s, n2_d) || hasEdge(n1_d, n2_s)) return false;
            }
            return true;
        } else {
            for (size_t k = 0; k < arr1.size(); k++) {
                if (arr1[k] == "0") break;
                string n1_s = arr1[k] + "S";
                string n1_d = arr1[k] + "D";
                for (size_t m = 0; m < arr2.size(); m++) {
                    if (arr2[m] == "0") break;
                    string n2_s = arr2[m] + "S";
                    string n2_d = arr2[m] + "D";
                    if (hasEdge(n1_s, n2_d) || hasEdge(n1_d, n2_s)) return false;
                }
            }
            return true;
        }
    }

    bool checkingParallelConnected(const string& k, const vector<string>& inter_arr) {
        if (inter_arr.empty()) return true;
        vector<string> arr1, arr2;
        size_t i = 0, j = 0;
        while (i < inter_arr.size() && inter_arr[i] != "0") {
            arr1.push_back(inter_arr[i]);
            i++;
        }
        i++;
        while (i < inter_arr.size() && inter_arr[i] != "0") {
            arr2.push_back(inter_arr[i]);
            i++;
            j++;
        }
        if (arr2.empty()) {
            for (size_t m = 0; m < arr1.size() - 1; m++) {
                string n1 = arr1[m] + k;
                for (size_t n = m + 1; n < arr1.size(); n++) {
                    string n2 = arr1[n] + k;
                    if (hasEdge(n1, n2)) return false;
                }
            }
            return true;
        } else {
            for (size_t m = 0; m < arr1.size(); m++) {
                string n1 = arr1[m] + k;
                for (size_t n = 0; n < arr2.size(); n++) {
                    string n2 = arr2[n] + k;
                    if (hasEdge(n1, n2)) return false;
                }
            }
            return true;
        }
    }

    void intersect_subtract(string_view expr1, string_view expr2, vector<string>& inter_arr, string& remain) {
        remain.clear();
        size_t i = 0, j = 0;
        while (i < expr1.size()) {
            if (j < expr2.size() && expr1[i] == expr2[j]) {
                if (isalpha(expr1[i])) inter_arr.emplace_back(1, expr1[i]);
                ++j;
            } else {
                remain += expr1[i];
            }
            ++i;
        }
        if (!inter_arr.empty() && inter_arr.back() != "0")
            inter_arr.emplace_back("0");
    }

    void createGraph(vector<string>& q, int i, const string& expression, int mode, string& end_node) {
        vector<string> inter_arr;
        string remain;
        string expre = expression;
        int t = i - 1;
        while (t >= 0) {
            string_view element = q[t];
            intersect_subtract(expre, element, inter_arr, remain);
            if (!remain.empty()) {
                expre = remain;
            }
            t--;
            cout << "Intersection arr: ";
            for (string str: inter_arr) {
                cout << str << " ";
            }
            cout << endl;
            cout << "Remain: " << expre << endl;
        }
        if (expre == expression) {
            string node1, node2;
            char operator_char = 0;
            for (char token : expression) {
                if (isalpha(token)) {
                    if (node1.empty()) {
                        node1 = string(1, token);
                        createNode(node1);
                    } else {
                        node2 = string(1, token);
                        createNode(node2);
                    }
                } else if (token == '+' || token == '*') {
                    operator_char = token;
                }
            }
            if (operator_char == '+') {
                addEdgeParallel(1, node1, node2, {}, mode);
            } else if (operator_char == '*') {
                addEdgeSerial(1, node1, node2, inter_arr, mode);
            }
        } else {
            string node;
            char operator_char = 0;
            for (char token : expre) {
                if (token == '+' || token == '*') {
                    operator_char = token;
                } else if (isalpha(token)) {
                    node = string(1, token);
                    createNode(node);
                }
            }
            if (((operator_char == '+') ? 1 : 2) == 1) {
                if (!node.empty()) {
                    addEdgeParallel(2, node, "", inter_arr, mode);
                } else {
                    addEdgeParallel(3, "", "", inter_arr, mode);
                }
            } else {
                if (!node.empty()) {
                    addEdgeSerial(2, node, "", inter_arr, mode);
                } else {
                    addEdgeSerial(3, "", "", inter_arr, mode);
                }
            }
            end_node = node;

            // if (!remain.empty()) {
            //     expre = remain;
            //     if (isalpha(remain.back())) {
            //         end_node = string(1, remain.back());
            //     }
            // }
        }
    }

    void createNmos(const string& expression, string& end_node) {
        expr e;
        stringstream(expression) >> e;
        e.evaluate_expression(false);
        vector<string> q = e.steps;
        cout << "NMOS steps: ";
        for (string str: q) {
            cout << str << " ";
        }
        cout << endl;
        for (size_t i = 0; i < q.size(); i++) {
            createGraph(q, i, q[i], 0, end_node);
        }
    }

    void createPmos(const string& expression, const vector<string>& euler_path) {
        expr e;
        stringstream(expression) >> e;
        e.evaluate_expression(true);
        vector<string> q = e.steps;
        cout << "PMOS steps: ";
        for (string str: q) {
            cout << str << " ";
        }
        cout << endl;
        cout << "PMOS euler path: ";
        for (string str: euler_path) {
            cout << str << " ";
        }
        cout << endl;
        for (size_t i = 0; i < euler_path.size(); i++) {
            cout << "Node to add: " << euler_path[i] << endl;
            addNode(euler_path[i]);
            if (i != 0) {
                addEdge(euler_path[i], euler_path[i - 1]);
            }
        }
        cout << "PMOS graph before: " << endl;
        printGraph();
        for (size_t i = 0; i < q.size(); i++) {
            string dummy;
            createGraph(q, i, q[i], 1, dummy);
            cout << "PMOS graph after stage " << i << ": " << endl;
            printGraph();
        }
    }

    bool isValidNextNode(const string& v, const vector<string>& path) {
        if (find(path.begin(), path.end(), v) != path.end()) return false;
        if (path.empty()) return true;
        string last_node = path.back();
        if (last_node[0] != v[0]) {
            if (last_node[1] == 'S') {
                string x = last_node[0] + string("D");
                return find(path.begin(), path.end(), x) != path.end();
            }
            if (last_node[1] == 'D') {
                string x = last_node[0] + string("S");
                return find(path.begin(), path.end(), x) != path.end();
            }
        }
        // Tìm danh sách hàng xóm của last_node
        for (const auto& entry : adjList) {
            if (entry.first == last_node) {
                return find(entry.second.begin(), entry.second.end(), v) != entry.second.end();
            }
        }
        return false;
    }

    vector<string> hamiltonianDfsEndnode(const string& start, const string& end_node, vector<string> path = {}) {
        path.push_back(start);
        if (path.size() == adjList.size()) {
            return path;
        }
        for (const auto& entry : adjList) {
            if (entry.first == start) {
                for (const string& v : entry.second) {
                    if (isValidNextNode(v, path)) {
                        vector<string> new_path = hamiltonianDfsEndnode(v, end_node, path);
                        if (!new_path.empty()) return new_path;
                    }
                }
            }
        }
        return {};
    }

    vector<string> hamiltonianDfs(const string& start, vector<string> path = {}) {
        path.push_back(start);
        if (path.size() == adjList.size()) {
            return path;
        }
        for (const auto& entry : adjList) {
            if (entry.first == start) {
                for (const string& v : entry.second) {
                    if (isValidNextNode(v, path)) {
                        vector<string> new_path = hamiltonianDfs(v, path);
                        if (!new_path.empty()) return new_path;
                    }
                }
            }
        }
        return {};
    }

    vector<string> findHamiltonPath(const string& end_node) {
        vector<string> path;
        // Thử các node bắt đầu bằng 'D' trước
        for (const auto& entry : adjList) {
            if (entry.first[1] == 'S') continue;
            if (!end_node.empty()) {
                path = hamiltonianDfsEndnode(entry.first, end_node);
                if (!path.empty()) {
                    if (path[0][0] != end_node[0] && path.back()[0] != end_node[0]) {
                        path = {};
                    }
                }
            } else {
                path = hamiltonianDfs(entry.first);
            }
            if (!path.empty()) break;
        }
        // Nếu không tìm thấy, thử các node bắt đầu bằng 'S'
        if (path.empty()) {
            for (const auto& entry : adjList) {
                if (entry.first[1] == 'D') continue;
                if (!end_node.empty()) {
                    path = hamiltonianDfsEndnode(entry.first, end_node);
                    if (!path.empty()) {
                        if (path[0][0] != end_node[0] && path.back()[0] != end_node[0]) {
                            path = {};
                        }
                    }
                } else {
                    path = hamiltonianDfs(entry.first);
                }
                if (!path.empty()) break;
            }
        }
        return path;
    }

    pair<vector<string>, vector<string>> eulerPath(const string& end_node) {
        vector<string> euler_path_nmos = findHamiltonPath(end_node);
        if (euler_path_nmos.empty()) return {{}, {}};
        vector<string> euler_path_pmos(euler_path_nmos.size());
        bool s = false;
        for (size_t i = 0; i < euler_path_nmos.size() - 1; i += 2) {
            if (s) {
                euler_path_pmos[i] = euler_path_nmos[i + 1];
                euler_path_pmos[i + 1] = euler_path_nmos[i];
            } else {
                euler_path_pmos[i] = euler_path_nmos[i];
                euler_path_pmos[i + 1] = euler_path_nmos[i + 1];
            }
            s = !s;
        }
        return {euler_path_nmos, euler_path_pmos};
    }

    void checkAndAddParallelEdges(const vector<string>& serial_arr, const vector<string>& parallel_arr, const vector<string>& euler_path) {
        // Tạo đồ thị song song từ parallel_arr
        map<string, vector<string>> parallel_graph;
        vector<string> nodes;
        for (const auto edge : parallel_arr) {
            string n1 = edge.substr(0, 1);
            string n2 = edge.substr(1, 1);
            if (find(nodes.begin(), nodes.end(), n1) == nodes.end()) nodes.push_back(n1);
            if (find(nodes.begin(), nodes.end(), n2) == nodes.end()) nodes.push_back(n2);
            parallel_graph[n1].push_back(n2);
            parallel_graph[n2].push_back(n1);
        }

        // Hàm DFS kiểm tra đường đi giữa hai node
        auto hasPath = [&](const string& start, const string& end, const vector<string>& component) {
            vector<string> visited;
            stack<string> stk;
            stk.push(start);
            visited.push_back(start);
            while (!stk.empty()) {
                string curr = stk.top();
                stk.pop();
                if (curr == end) return true;
                for (const auto neighbor : parallel_graph[curr]) {
                    if (find(component.begin(), component.end(), neighbor) != component.end() &&
                        find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                        stk.push(neighbor);
                        visited.push_back(neighbor);
                    }
                }
            }
            return false;
        };

        // DFS để tìm các tập hợp liên thông
        vector<vector<string>> components;
        vector<string> visited;
        for (const auto node : nodes) {
            if (find(visited.begin(), visited.end(), node) == visited.end()) {
                vector<string> component;
                stack<string> stk;
                stk.push(node);
                visited.push_back(node);
                while (!stk.empty()) {
                    string curr = stk.top();
                    stk.pop();
                    component.push_back(curr);
                    for (const auto neighbor : parallel_graph[curr]) {
                        if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                            stk.push(neighbor);
                            visited.push_back(neighbor);
                        }
                    }
                }
                if (component.size() >= 3) {
                    components.push_back(component);
                }
            }
        }

        // Kiểm tra từng tập hợp liên thông
        for (const auto component : components) {
            bool valid = true;
            // Kiểm tra serial_arr để đảm bảo không có cặp node nào nối tiếp
            for (size_t i = 0; i < component.size() && valid; ++i) {
                for (size_t j = i + 1; j < component.size(); ++j) {
                    string pair1 = component[i] + component[j];
                    string pair2 = component[j] + component[i];
                    if (find(serial_arr.begin(), serial_arr.end(), pair1) != serial_arr.end() ||
                        find(serial_arr.begin(), serial_arr.end(), pair2) != serial_arr.end()) {
                        valid = false;
                        break;
                    }
                }
            }
            if (!valid) continue;

            // Kiểm tra và thêm các cạnh song song thiếu dựa trên parallel_arr
            for (const auto edge : parallel_arr) {
                string n1 = edge.substr(0, 1);
                string n2 = edge.substr(1, 1);
                // Chỉ xử lý nếu cả n1 và n2 đều trong component
                if (find(component.begin(), component.end(), n1) == component.end() ||
                    find(component.begin(), component.end(), n2) == component.end()) {
                    continue;
                }
                // Kiểm tra cạnh S-S
                if (!hasEdge(n1 + "S", n2 + "S")) {
                    addEdge(n1 + "S", n2 + "S");
                }
                // Kiểm tra cạnh D-D
                if (!hasEdge(n1 + "D", n2 + "D")) {
                    addEdge(n1 + "D", n2 + "D");
                }
            }
        }
    }

    void filterEdgePmos(const vector<string>& serial_arr, const vector<string>& parallel_arr, const vector<string>& euler_path) {
        vector<string> check_serial, check_parallel;
        vector<string> edges_to_remove; //Add by Khanh
        for (size_t i = 0; i < euler_path.size(); i++) {
            if (i == 0) continue;
            string n1 = euler_path[i];
            string n2 = euler_path[i - 1];
            if (n1[0] != n2[0] && n1[1] != n2[1]) {
                check_serial.push_back(n1.substr(0, 1) + n2.substr(0, 1));
                check_serial.push_back(n2.substr(0, 1) + n1.substr(0, 1));
            }
            if (n1[0] != n2[0] && n1[1] == n2[1]) {
                check_parallel.push_back(n1.substr(0, 1) + n2.substr(0, 1) + n1[1]);
                check_parallel.push_back(n2.substr(0, 1) + n1.substr(0, 1) + n1[1]);
            }
        }

        cout << "check_serial: ";
        for (auto str: check_serial) {
            cout << str << " "; 
        }
        cout << endl;
        cout << "check_parallel: ";
        for (auto str: check_parallel) {
            cout << str << " "; 
        }
        cout << endl;

        for (const auto& entry : adjList) {
            for (const string& neighbor : entry.second) {
                if (entry.first.empty() || neighbor.empty()) {
                    continue;
                }
                string n1 = entry.first.substr(0, 1);
                string n2 = neighbor.substr(0, 1);
                if (n1 == n2) continue;
                string n3 = entry.first.substr(entry.first.size() - 1);
                string n4 = neighbor.substr(neighbor.size() - 1);
                cout << "n1: " << n1 << endl;
                cout << "n2: " << n2 << endl;
                cout << "n3: " << n3 << endl;
                cout << "n4: " << n4 << endl;
                if (n3 != n4) {
                    string node1 = n1 + n2;
                    string node2 = n2 + n1;
                    int s = 0;
                    if ((find(serial_arr.begin(), serial_arr.end(), node1) != serial_arr.end() || 
                         find(serial_arr.begin(), serial_arr.end(), node2) != serial_arr.end()) &&
                        (find(check_serial.begin(), check_serial.end(), node1) == check_serial.end() ||
                         find(check_serial.begin(), check_serial.end(), node2) == check_serial.end())) {
                        if (hasEdge(n1 + "S", n2 + "D")) {
                            removeEdge(n1 + "S", n2 + "D");
                            cout << "Removed edges 1: " << n1 + "S" << n2 + "D" << endl;
                            //Add by Khanh
                            edges_to_remove.push_back(n1 + "S" + n2 + "D");
                            s = 1;
                        }
                        if (hasEdge(n1 + "D", n2 + "S")) {
                            removeEdge(n1 + "D", n2 + "S");
                            cout << "Removed edges 2: " << n1 + "D" << n2 + "S" << endl;
                            //Add by Khanh
                            edges_to_remove.push_back(n1 + "D" + n2 + "S");
                            s = 2;
                        }
                        if (s == 1) {
                            s = 0;
                            for (const auto& n : adjList) {
                                cout << "Node here: " << n.first << endl;
                                //Add by Khanh
                                if (n.first[1] == 'S' && hasEdge(n.first, n2 + "D")) {
                                    if (find(edges_to_remove.begin(), edges_to_remove.end(), n1 + "S" + string(1, n.first[0]) + "D") == edges_to_remove.end())
                                    {
                                        addEdge(n1 + "S", string(1, n.first[0]) + "D");
                                        cout << "Added края 1: " << (n1 + "S") << (string(1, n.first[0]) + "D") << endl;
                                        break;
                                    }
                                }
                                if (n.first[1] == 'S' && hasEdge(n.first, n1 + "D")) {
                                    if (find(edges_to_remove.begin(), edges_to_remove.end(), n2 + "S" + string(1, n.first[0]) + "D") == edges_to_remove.end())
                                    {
                                        addEdge(n2 + "S", string(1, n.first[0]) + "D");
                                        cout << "Added края 2: " << (n2 + "S") << (string(1, n.first[0]) + "D") << endl;
                                        break;
                                    }
                                }
                            }
                        } else if (s == 2) {
                            s = 0;
                            for (const auto& n : adjList) {
                                //Add by Khanh
                                if (n.first[1] == 'D' && hasEdge(n.first, n2 + "S")) {
                                    if (find(edges_to_remove.begin(), edges_to_remove.end(), n1 + "D" + (string(1, n.first[0]) + "S")) == edges_to_remove.end())
                                    {
                                        addEdge(n1 + "D", (string(1, n.first[0]) + "S"));
                                        cout << "Added края 3: " << (n1 + "D") << (string(1, n.first[0]) + "S") << endl;
                                        break;
                                    }
                                    
                                }
                                if (n.first[1] == 'D' && hasEdge(n.first, n1 + "S")) {
                                    if (find(edges_to_remove.begin(), edges_to_remove.end(), n2 + "D" + string(1, n.first[0]) + "S") == edges_to_remove.end())
                                    {
                                        addEdge(n2 + "D", string(1, n.first[0]) + "S");
                                        cout << "Added края 4: " << (n2 + "D") << (string(1, n.first[0]) + "S") << endl;
                                        break;
                                    }
                                    
                                }
                            }
                        }
                    }
                } else {
                    string node1 = n1 + n2;
                    string node2 = n2 + n1;
                    if (find(parallel_arr.begin(), parallel_arr.end(), node1) == parallel_arr.end() &&
                        find(parallel_arr.begin(), parallel_arr.end(), node2) == parallel_arr.end()) {
                        for (const string& n : check_parallel) {
                            if (node1 != n.substr(0, 2) && node2 != n.substr(0, 2) && n3 == string(1, n[2])) {
                                if (hasEdge(n1 + string(1, n[2]), n2 + string(1, n[2]))) {
                                    cout << "Removed края 3: " << (n1 + string(1, n[2])) << (n2 + string(1, n[2])) << endl;
                                    removeEdge(n1 + string(1, n[2]), n2 + string(1, n[2]));
                                    //Add by Khanh
                                    edges_to_remove.push_back(n1 + string(1, n[2]) + n2 + string(1, n[2]));
                                }
                            }
                        }
                    }
                }
            }
        }
        checkAndAddParallelEdges(serial_arr, parallel_arr, euler_path);
    }

    bool checkingEdge(const string& full_node, const string& char_connected) {
        for (const auto& entry : adjList) {
            if (entry.first[0] == full_node[0]) continue;
            if (char_connected.empty()) {
                if (hasEdge(full_node, entry.first)) return false;
            } else if (entry.first[1] == char_connected[0]) {
                if (hasEdge(full_node, entry.first)) return false;
            }
        }
        return true;
    }

    pair<vector<string>, vector<string>> findNodeSourceAndOut() {
        vector<pair<string, string>> sourceNodes;
        vector<pair<string, string>> outNodes;

        // Lấy tất cả các cạnh
        for (const auto& entry : adjList) {
            const string& u = entry.first;
            for (const string& v : entry.second) {
                if (u[0] == v[0]) continue;
                if (u[1] == v[1]) {
                    if (u[1] == 'S')
                        sourceNodes.emplace_back(u.substr(0, 1), v.substr(0, 1));
                    else
                        outNodes.emplace_back(u.substr(0, 1), v.substr(0, 1));
                }
            }
        }

        // Xử lý sourceNodes
        if (!sourceNodes.empty()) {
            vector<pair<string, string>> temp1 = sourceNodes;
            for (const auto& p : temp1) {
                string n1_s = p.first + "S";
                string n2_s = p.second + "S";
                if (!checkingEdge(n1_s, "D") || !checkingEdge(n2_s, "D")) {
                    sourceNodes.erase(remove(sourceNodes.begin(), sourceNodes.end(), p), sourceNodes.end());
                }
            }

            if (!sourceNodes.empty()) {
                vector<string> flat;
                for (const auto& p : sourceNodes) {
                    flat.push_back(p.first + "S");
                    flat.push_back(p.second + "S");
                }
                sourceNodes.clear();
                for (const auto& s : flat)
                    sourceNodes.emplace_back(s, "");
            }
        }

        // Xử lý outNodes
        if (!outNodes.empty()) {
            vector<pair<string, string>> temp2 = outNodes;
            for (const auto& p : temp2) {
                string n1_d = p.first + "D";
                string n2_d = p.second + "D";
                if (!checkingEdge(n1_d, "S") || !checkingEdge(n2_d, "S")) {
                    outNodes.erase(remove(outNodes.begin(), outNodes.end(), p), outNodes.end());
                }
            }

            if (!outNodes.empty()) {
                vector<string> flat;
                for (const auto& p : outNodes) {
                    flat.push_back(p.first + "D");
                    flat.push_back(p.second + "D");
                }
                outNodes.clear();
                for (const auto& s : flat)
                    outNodes.emplace_back(s, "");
            }
        }

        // Chuyển sourceNodes -> vector<string>
        vector<string> source_flat;
        for (const auto& p : sourceNodes)
            source_flat.push_back(p.first);

        vector<string> source_check = source_flat;
        for (const string& node : source_check) {
            if (!checkingEdge(node, "")) {
                source_flat.erase(remove(source_flat.begin(), source_flat.end(), node), source_flat.end());
            }
        }

        if (source_flat.empty()) {
            for (const auto& entry : adjList) {
                const string& node = entry.first;
                if (node[1] == 'S' && checkingEdge(node, "")) {
                    source_flat.push_back(node);
                }
            }
        }

        if (source_flat.empty()) {
            source_flat = source_check;
        }

        // Chuyển outNodes -> vector<string>
        vector<string> out_flat;
        for (const auto& p : outNodes)
            out_flat.push_back(p.first);

        vector<string> out_check = out_flat;
        for (const string& node : out_check) {
            if (!checkingEdge(node, "")) {
                out_flat.erase(remove(out_flat.begin(), out_flat.end(), node), out_flat.end());
            }
        }

        if (out_flat.empty()) {
            for (const auto& entry : adjList) {
                const string& node = entry.first;
                if (node[1] == 'D' && checkingEdge(node, "")) {
                    out_flat.push_back(node);
                }
            }
        }

        if (out_flat.empty()) {
            out_flat = out_check;
        }

        return {source_flat, out_flat};
    }
};

// Main function
struct Result {
    graph g_nmos, g_pmos;
    vector<string> euler_path_nmos, euler_path_pmos;
    vector<string> source_nodes_nmos, out_nodes_nmos;
    vector<string> source_nodes_pmos, out_nodes_pmos;
};

Result CreateAll(const string& expression) {
    Result result;
    string end_node;
    result.g_nmos.createNmos(expression, end_node);
    auto [source_nodes_nmos, out_nodes_nmos] = result.g_nmos.findNodeSourceAndOut();
    vector<string> serial_array_pmos, parallel_array_pmos;
    for (const auto& entry : result.g_nmos.adjList) {
        for (const string& neighbor : entry.second) {
            if (entry.first >= neighbor) continue;
            string n1 = entry.first.substr(0, 1);
            string n2 = neighbor.substr(0, 1);
            if (n1 == n2) continue;
            string n3 = string(1, entry.first[entry.first.size() - 1]);
            string n4 = string(1, neighbor[neighbor.size() - 1]);
            if (n3 == n4) {
                serial_array_pmos.push_back(n1 + n2);
            } else {
                parallel_array_pmos.push_back(n1 + n2);
            }
        }
    }

    cout << "end_node = " << end_node << endl;

    auto [euler_path_nmos, euler_path_pmos] = result.g_nmos.eulerPath(end_node);
    cout << "NMOS euler path: ";
    for (string str: euler_path_nmos) {
        cout << str << " ";
    }
    cout << endl;
    cout << "serial_array_pmos: ";
    for (string str: serial_array_pmos) {
        cout << str << " ";
    }
    cout << endl;
    cout << "parallel_array_pmos: ";
    for (string str: parallel_array_pmos) {
        cout << str << " ";
    }
    cout << endl;
    result.g_pmos.createPmos(expression, euler_path_pmos);
    result.g_pmos.filterEdgePmos(serial_array_pmos, parallel_array_pmos, euler_path_pmos);
    cout << "\nPMOS Graph after filter:\n";
    result.g_pmos.printGraph();
    auto [source_nodes_pmos, out_nodes_pmos] = result.g_pmos.findNodeSourceAndOut();
    result.euler_path_nmos = euler_path_nmos;
    result.euler_path_pmos = euler_path_pmos;
    result.source_nodes_nmos = source_nodes_nmos;
    result.out_nodes_nmos = out_nodes_nmos;
    result.source_nodes_pmos = source_nodes_pmos;
    result.out_nodes_pmos = out_nodes_pmos;
    return result;
}
