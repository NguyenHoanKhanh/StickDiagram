#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
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
        for (const auto& s : l.steps) {
            os << "\"" << s << "\"" << endl;
        }
        return os;
    }
};

// Class graph
class graph {
public:
    unordered_map<string, vector<string>> adjList;

    void printGraph() {
        for (const auto& node : adjList) {
            cout << node.first << ": ";
            for (const auto& neighbor : node.second) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    void addNode(const string& node) {
        if (adjList.find(node) == adjList.end()) {
            adjList[node] = vector<string>();
        }
    }
    
    void addEdge(const string& u, const string& v) {
        addNode(u);
        addNode(v);
        // Kiểm tra xem cạnh (u, v) hoặc (v, u) đã tồn tại chưa
        if (!hasEdge(u, v)) {
            adjList[u].push_back(v);
            adjList[v].push_back(u);
        }
    }
    void removeEdge(const string& u, const string& v) {
        if (hasNode(u)) {
            adjList[u].erase(remove(adjList[u].begin(), adjList[u].end(), v), adjList[u].end());
        }
        if (hasNode(v)) {
            adjList[v].erase(remove(adjList[v].begin(), adjList[v].end(), u), adjList[v].end());
        }
    }
    bool hasNode(const string& node) {
        return adjList.find(node) != adjList.end();
    }
    bool hasEdge(const string& u, const string& v) {
        if (!hasNode(u)) return false;
        for (const string& neighbor : adjList[u]) {
            if (neighbor == v) return true;
        }
        return false;
    }
    int degree(const string& node) {
        return hasNode(node) ? adjList[node].size() : 0;
    }
    vector<string> neighbors(const string& node) {
        return hasNode(node) ? adjList[node] : vector<string>();
    }
    void createNode(const string& node) {
        string node1 = node + "S";
        string node2 = node + "D";
        addNode(node1);
        addNode(node2);
        addEdge(node1, node2);
    }
    void addEdgeParallel1(const string& node1, const string& node2) {
        string n1 = node1 + "S";
        string n2 = node1 + "D";
        string n3 = node2 + "S";
        string n4 = node2 + "D";
        addEdge(n1, n3);
        addEdge(n2, n4);
    }
    void addEdgeSerial1(const string& node1, const string& node2, const vector<string>& inter_arr, int mode) {
        string n1 = node1 + "S";
        string n2 = node1 + "D";
        string n3 = node2 + "S";
        string n4 = node2 + "D";
        if (mode == 0) {
            addEdge(n3, n2);
        } else if (mode == 1 && checkSerialConnected(" ", inter_arr)) {
            addEdge(n3, n2);
        }
    }
    void addEdgeParallel2(const string& node, const vector<string>& inter_arr) {
        string n1 = node + "S";
        string n2 = node + "D";
        string outside_node;
        for (const auto& n : adjList) {
            string n_first = n.first.substr(0, 1);
            if (find(inter_arr.begin(), inter_arr.end(), n_first) == inter_arr.end() && n_first != node) {
                outside_node = n_first;
                break;
            }
        }
        string node1 = lowestDegree("S", node, node, outside_node);
        string node2 = lowestDegree("D", node, node, outside_node);
        int node1_degree = degree(node1);
        int node2_degree = degree(node2);
        int s = (node1_degree < node2_degree) ? 1 : 2;
        if (s == 1) {
            addEdge(n1, node1);
            string node1_first = node1.substr(0, 1);
            string node_connect = lowestDegree("D", node, node1_first, outside_node);
            addEdge(n2, node_connect);
        } else {
            addEdge(n2, node2);
            string node2_first = node2.substr(0, 1);
            string node_connect = lowestDegree("S", node, node2_first, outside_node);
            addEdge(n1, node_connect);
        }
    }
    void addEdgeSerial2(const string& node, const vector<string>& inter_arr, int mode) {
        string n1 = node + "S";
        string n2 = node + "D";
        int i = 0, j = 0;
        int degree1, degree2;
        string node_connect1 = lowestDegreeArrNode("D", i, inter_arr, "", degree1);
        i = degree1;
        string node_connect2 = lowestDegreeArrNode("S", j, inter_arr, "", degree2);
        if (mode == 0) {
            addEdge(n1, node_connect1);
            for (const auto& n : adjList) {
                if (n.first[0] == node_connect1[0]) continue;
                if (hasEdge(node_connect1, n.first) && n.first[1] == 'D') {
                    addEdge(n1, n.first);
                }
            }
        } else if (mode == 1 && checkSerialConnected(node, inter_arr)) {
            addEdge(n2, node_connect2);
        }
    }
    void addEdgeParallel3(const vector<string>& inter_arr, int mode) {
        int i = 0, j = 0;
        int n1_degree, n2_degree, n3_degree, n4_degree;
        string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
        int t1 = i;
        string n2 = lowestDegreeArrNode("S", i, inter_arr, "", n2_degree);
        string n3 = lowestDegreeArrNode("D", j, inter_arr, "", n3_degree);
        string n4 = lowestDegreeArrNode("D", j, inter_arr, "", n4_degree);
        int sel = 3;
        if (n1_degree + n2_degree < n3_degree + n4_degree) {
            if (checkingParallelConnected("S", inter_arr) && mode == 1 || mode == 0) {
                sel = 0;
                addEdge(n1, n2);
            }
        } else {
            if (checkingParallelConnected("D", inter_arr) && mode == 1 || mode == 0) {
                sel = 1;
                addEdge(n3, n4);
            }
        }
        if (sel == 1) {
            string except_node = n4.substr(0, 1);
            int temp_degree;
            string node_connect = lowestDegreeArrNode("S", t1, inter_arr, except_node, temp_degree);
            addEdge(n1, node_connect);
        } else if (sel == 2) {
            string except_node = n2.substr(0, 1);
            int temp_degree;
            string node_connect = lowestDegreeArrNode("D", t1, inter_arr, except_node, temp_degree);
            addEdge(n3, node_connect);
        }
    }
    void addEdgeSerial3(const vector<string>& inter_arr, int mode) {
        int i = 0;
        int n1_degree, n2_degree;
        string n1 = lowestDegreeArrNode("S", i, inter_arr, "", n1_degree);
        string n2 = lowestDegreeArrNode("D", i, inter_arr, n1, n2_degree);
        if (mode == 0) {
            addEdge(n1, n2);
            for (const auto& n : adjList) {
                if (hasEdge(n.first, n1) && n.first[1] == 'S') {
                    addEdge(n.first, n2);
                }
                if (hasEdge(n.first, n2) && n.first[1] == 'D') {
                    addEdge(n.first, n1);
                }
            }
        } else if (mode == 1 && checkSerialConnected("", inter_arr)) {
            addEdge(n1, n2);
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
        for (const auto& n : adjList) {
            if (n.first[n.first.size() - 1] != k[0]) continue;
            if (n.first.substr(0, 1) == except_node1 || n.first.substr(0, 1) == except_node2 || n.first.substr(0, 1) == except_node3) continue;
            int deg = degree(n.first);
            if (deg < min_degree) {
                min_degree = deg;
                min_node = n.first;
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

    /**
     * @brief Computes the intersection of two logical expressions and the remainder.
     * 
     * Finds common alphabetic characters between `expr1` and `expr2`, adds them to `inter_arr`, and stores the leftover in `remain`. A "0" is appended to `inter_arr` to separate intersection groups.
     *
     * @param expr1 The first logical expression.
     * @param expr2 The second logical expression.
     * @param inter_arr Array storing the intersection groups.
     * @param remain The remainder of `expr1` after removing the intersection.
     */
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
                addEdgeParallel1(node1, node2);
            } else if (operator_char == '*') {
                addEdgeSerial1(node1, node2, inter_arr, mode);
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
            if (operator_char == '+' ? 1 : 2 == 1) {
                if (!node.empty()) {
                    addEdgeParallel2(node, inter_arr);
                } else {
                    addEdgeParallel3(inter_arr, mode);
                }
            } else {
                if (!node.empty()) {
                    addEdgeSerial2(node, inter_arr, mode);
                } else {
                    addEdgeSerial3(inter_arr, mode);
                }
            }
            end_node = node;
        }
    }

    void createNmos(const string& expression, string& end_node) {
        expr e;
        stringstream(expression) >> e;
        e.evaluate_expression(false);
        vector<string> q = e.steps;
        for (size_t i = 0; i < q.size(); i++) {
            createGraph(q, i, q[i], 0, end_node);
        }
    }
    void createPmos(const string& expression, const vector<string>& euler_path) {
        expr e;
        stringstream(expression) >> e;
        e.evaluate_expression(true);
        vector<string> q = e.steps;

        for (size_t i = 0; i < euler_path.size(); i++) {
            addNode(euler_path[i]);
            if (i != 0) {
                addEdge(euler_path[i], euler_path[i - 1]);
            }
        }
        for (size_t i = 0; i < q.size(); i++) {
            string dummy;
            createGraph(q, i, q[i], 1, dummy);
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
        return find(adjList[last_node].begin(), adjList[last_node].end(), v) != adjList[last_node].end();
    }
    vector<string> hamiltonianDfs(const string& start, const string& end_node, vector<string> path = {}) {
        path.push_back(start);
        if (path.size() == adjList.size()) {
            if (!end_node.empty() && (path[0][0] != end_node[0] && path.back()[0] != end_node[0])) {
                return {};
            }
            return path;
        }
        for (const string& v : neighbors(start)) {
            if (isValidNextNode(v, path)) {
                vector<string> new_path = hamiltonianDfs(v, end_node, path);
                if (!new_path.empty()) return new_path;
            }
        }
        return {};
    }
    vector<string> findHamiltonPath(const string& end_node) {
        vector<string> path;
        for (const auto& node : adjList) {
            if (node.first[1] == 'S') continue;
            path = hamiltonianDfs(node.first, end_node);
            if (!path.empty()) break;
        }
        if (path.empty()) {
            for (const auto& node : adjList) {
                if (node.first[1] == 'D') continue;
                path = hamiltonianDfs(node.first, end_node);
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
    void filterEdgePmos(const vector<string>& serial_arr, const vector<string>& parallel_arr, const vector<string>& euler_path) {
        vector<string> check_serial, check_parallel;
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

        vector<pair<string, string>> edges_to_remove;
        for (const auto& edge : adjList) {
            for (const string& neighbor : edge.second) {
                if (edge.first >= neighbor) continue;
                string n1 = edge.first.substr(0, 1);
                string n2 = neighbor.substr(0, 1);
                if (n1 == n2) continue;
                string n3 = edge.first.substr(edge.first.size() - 1);
                string n4 = neighbor.substr(neighbor.size() - 1);
                if (n3 != n4) {
                    string node1 = n1 + n2;
                    string node2 = n2 + n1;
                    int s = 0;
                    if ((find(serial_arr.begin(), serial_arr.end(), node1) != serial_arr.end() || 
                         find(serial_arr.begin(), serial_arr.end(), node2) != serial_arr.end()) &&
                        find(check_serial.begin(), check_serial.end(), node1) == check_serial.end() &&
                        find(check_serial.begin(), check_serial.end(), node2) == check_serial.end()) {
                        if (hasEdge(n1 + "S", n2 + "D")) {
                            edges_to_remove.emplace_back(n1 + "S", n2 + "D");

                            s = 1;
                        }
                        if (hasEdge(n1 + "D", n2 + "S")) {
                            edges_to_remove.emplace_back(n1 + "D", n2 + "S");
                            
                            s = 2;
                        }
                        if (s == 1) {
                            for (const auto& n : adjList) {
                                if (hasEdge(n.first, n2 + "D") && n.first[1] == 'S') {
                                    addEdge(n1 + "S", n.first.substr(0, 1) + "D");
                                    break;
                                }
                                if (hasEdge(n.first, n1 + "D") && n.first[1] == 'S') {
                                    addEdge(n2 + "S", n.first.substr(0, 1) + "D");
                                    break;
                                }
                            }
                        } else if (s == 2) {
                            for (const auto& n : adjList) {
                                if (hasEdge(n.first, n2 + "S") && n.first[1] == 'D') {
                                    addEdge(n1 + "D", n.first.substr(0, 1) + "S");
                                    break;
                                }
                                if (hasEdge(n.first, n1 + "S") && n.first[1] == 'D') {
                                    addEdge(n2 + "D", n.first.substr(0, 1) + "S");
                                    break;
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
                                if (hasEdge(n1 + n3, n2 + n3)) {
                                    edges_to_remove.emplace_back(n1 + n3, n2 + n3);
                                }
                            }
                        }
                    }
                }
            }
        }

        for (const auto& edge : edges_to_remove) {
            removeEdge(edge.first, edge.second);
        }
    }
    bool checkingEdge(const string& full_node, const string& char_connected) {
        for (const auto& n : adjList) {
            if (n.first[0] == full_node[0]) continue;
            if (char_connected.empty()) {
                if (hasEdge(full_node, n.first)) return false;
            } else if (n.first[1] == char_connected[0]) {
                if (hasEdge(full_node, n.first)) return false;
            }
        }
        return true;
    }
    pair<vector<string>, vector<string>> findNodeSourceAndOut() {
        vector<pair<string, string>> source_nodes, out_nodes;
        for (const auto& edge : adjList) {
            for (const string& neighbor : edge.second) {
                if (edge.first >= neighbor) continue;
                if (edge.first[0] == neighbor[0]) continue;
                if (edge.first[1] == neighbor[1]) {
                    if (edge.first[1] == 'S') {
                        source_nodes.emplace_back(edge.first.substr(0, 1), neighbor.substr(0, 1));
                    } else {
                        out_nodes.emplace_back(edge.first.substr(0, 1), neighbor.substr(0, 1));
                    }
                }
            }
        }

        vector<string> final_source_nodes, final_out_nodes;
        if (!source_nodes.empty()) {
            vector<pair<string, string>> temp = source_nodes;
            for (auto it = temp.begin(); it != temp.end();) {
                string n1 = it->first, n2 = it->second;
                string n1_s = n1 + "S", n2_s = n2 + "S";
                if (!checkingEdge(n1_s, "D") || !checkingEdge(n2_s, "D")) {
                    it = temp.erase(it);
                } else {
                    ++it;
                }
            }
            for (const auto& pair : temp) {
                final_source_nodes.push_back(pair.first + "S");
                final_source_nodes.push_back(pair.second + "S");
            }
        }
        if (!out_nodes.empty()) {
            vector<pair<string, string>> temp = out_nodes;
            for (auto it = temp.begin(); it != temp.end();) {
                string n1 = it->first, n2 = it->second;
                string n1_d = n1 + "D", n2_d = n2 + "D";
                if (!checkingEdge(n1_d, "S") || !checkingEdge(n2_d, "S")) {
                    it = temp.erase(it);
                } else {
                    ++it;
                }
            }
            for (const auto& pair : temp) {
                final_out_nodes.push_back(pair.first + "D");
                final_out_nodes.push_back(pair.second + "D");
            }
        }
        if (final_source_nodes.empty()) {
            for (const auto& node : adjList) {
                if (node.first[1] == 'S' && checkingEdge(node.first, "")) {
                    final_source_nodes.push_back(node.first);
                }
            }
        }
        if (final_out_nodes.empty()) {
            for (const auto& node : adjList) {
                if (node.first[1] == 'D' && checkingEdge(node.first, "")) {
                    final_out_nodes.push_back(node.first);
                }
            }
        }

        return {final_source_nodes, final_out_nodes};
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
    for (const auto& edge : result.g_nmos.adjList) {
        for (const string& neighbor : edge.second) {
            if (edge.first >= neighbor) continue;
            string n1 = edge.first.substr(0, 1);
            string n2 = neighbor.substr(0, 1);
            if (n1 == n2) continue;
            string n3 = string(1, edge.first[edge.first.size() - 1]);
            string n4 = string(1, neighbor[neighbor.size() - 1]);
            if (n3 == n4) {
                serial_array_pmos.push_back(n1 + n2);
            } else {
                parallel_array_pmos.push_back(n1 + n2);
            }
        }
    }
    auto [euler_path_nmos, euler_path_pmos] = result.g_nmos.eulerPath(end_node);
    result.g_pmos.createPmos(expression, euler_path_pmos);
    result.g_pmos.filterEdgePmos(serial_array_pmos, parallel_array_pmos, euler_path_pmos);
    auto [source_nodes_pmos, out_nodes_pmos] = result.g_pmos.findNodeSourceAndOut();
    result.euler_path_nmos = euler_path_nmos;
    result.euler_path_pmos = euler_path_pmos;
    result.source_nodes_nmos = source_nodes_nmos;
    result.out_nodes_nmos = out_nodes_nmos;
    result.source_nodes_pmos = source_nodes_pmos;
    result.out_nodes_pmos = out_nodes_pmos;
    return result;
}

int main() {
    string expression = "A*(B+C)+(D*E*F)";
    Result result = CreateAll(expression);
    cout << "NMOS Graph:\n";
    result.g_nmos.printGraph();
    cout << "\nPMOS Graph:\n";
    result.g_pmos.printGraph();
    cout << "\nEuler Path NMOS: ";
    for (const auto& node : result.euler_path_nmos) cout << node << " ";
    cout << "\nEuler Path PMOS: ";
    for (const auto& node : result.euler_path_pmos) cout << node << " ";
    cout << "\nSource Nodes NMOS: ";
    for (const auto& node : result.source_nodes_nmos) cout << node << " ";
    cout << "\nOut Nodes NMOS: ";
    for (const auto& node : result.out_nodes_nmos) cout << node << " ";
    cout << "\nSource Nodes PMOS: ";
    for (const auto& node : result.source_nodes_pmos) cout << node << " ";
    cout << "\nOut Nodes PMOS: ";
    for (const auto& node : result.out_nodes_pmos) cout << node << " ";
    cout << endl;
    return 0;
}