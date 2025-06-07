#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>
#include <string.h>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>

#include "../NVL/test.cpp"
//Today Khanh will execute function check
using namespace std;
// Hàm kiểm tra rỗng và in thông báo lỗi
bool checkEmptyVector(const vector<string>& vec, const string& name) {
    if (vec.empty()) {
        cerr << "Error: " << name << " is empty!" << endl;
        return true;
    }
    return false;
}

bool checkEmptyVector(const vector<pair<string, string>>& vec, const string& name) {
    if (vec.empty()) {
        cerr << "Warning: " << name << " is empty!" << endl;
        return true;
    }
    return false;
}

bool checkEmptyVector(const vector<char>& vec, const string& name) {
    if (vec.empty()) {
        cerr << "Error: " << name << " is empty!" << endl;
        return true;
    }
    return false;
}

vector<char> create_logic_element (const vector<string> &euler_path_nmos) {
    vector<char> logic_elements;
    set<char> seen;

    if (checkEmptyVector(euler_path_nmos, "Euler path nmos"))
    {
        return logic_elements;
    }
    
    for(const string &node : euler_path_nmos) 
    {
        if (!node.empty())
        {
            char first_char = node[0];
            if (seen.find(first_char) == seen.end())
            {
                logic_elements.push_back(first_char);
                seen.insert(first_char);
            }
        }
    }
    return logic_elements;
}

// Draw label
void drawText(sf::RenderWindow& window, const std::string& content, float x, float y, int fontSize, sf::Color color, sf::Font& font) {
    sf::Text text(content, font, static_cast<unsigned int>(fontSize));
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x, y));
    window.draw(text);
}


// Draw column (line)
void drawColumn(sf::RenderWindow& window, float x1, float y1, float x2, float y2, const sf::Color &color) {
    sf::Vertex line[] = {
        sf::Vertex{sf::Vector2f(x1, y1), sf::Color(color)},
        sf::Vertex{sf::Vector2f(x2, y2), sf::Color(color)}
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

//Draw x 
void drawCrossMarker (sf::RenderWindow& window, float x, float y, sf::Color color){
    const float s = 5.f;
    sf::Vertex diag_1[] = { 
        {{x - s, y - s}, color},
        {{x + s, y + s}, color}
    };
    sf::Vertex diag_2[] = {
        {{x - s, y + s}, color},
        {{x + s, y - s}, color}
    };
    window.draw(diag_1, 2, sf::PrimitiveType::Lines);
    window.draw(diag_2, 2, sf::PrimitiveType::Lines);
}

// Initial stick empty
void drawStickDiagram(sf::RenderWindow& window, sf::Font& font, const vector<char>& logic_elements, const vector<string>& euler_path_pmos, const vector<string>& euler_path_nmos, const vector<pair<string, string>>& pmos_edges, const vector<pair<string, string>>& nmos_edges, const vector<string>& source_nodes_pmos, const vector<string>& source_nodes_nmos, const vector<string>& out_nodes_pmos, const vector<string>& out_nodes_nmos) {
    // Kiểm tra rỗng cho tất cả các đầu vào
    bool hasError = false;
    hasError |= checkEmptyVector(logic_elements, "logic_elements");
    hasError |= checkEmptyVector(euler_path_pmos, "euler_path_pmos");
    hasError |= checkEmptyVector(euler_path_nmos, "euler_path_nmos");
    checkEmptyVector(pmos_edges, "pmos_edges"); // Chỉ là warning vì edges có thể rỗng
    checkEmptyVector(nmos_edges, "nmos_edges");
    hasError |= checkEmptyVector(source_nodes_pmos, "source_nodes_pmos");
    hasError |= checkEmptyVector(source_nodes_nmos, "source_nodes_nmos");
    hasError |= checkEmptyVector(out_nodes_pmos, "out_nodes_pmos");
    hasError |= checkEmptyVector(out_nodes_nmos, "out_nodes_nmos");

    if (hasError) {
        drawText(window, "Error: Missing critical data. Check console for details.", 50, 50, 20, sf::Color::Red, font);
        return;
    }

    float scale = 50.0f;
    float offsetX = 100.0f;
    float offsetY = 300.0f;
    //Draw and write VDD
    drawColumn(window, offsetX - 4.5f * scale, offsetY - 6 * scale, offsetX + 14 * scale, offsetY - 6 * scale, sf::Color::Blue);//Check //
    drawText(window, "Vdd", offsetX + 14.5f * scale, offsetY - 6 * scale - 10, 20, sf::Color::Black, font);
    //Draw and write GND
    drawColumn(window, offsetX - 4.5f * scale, offsetY + 1 * scale, offsetX + 14 * scale, offsetY + 1 * scale, sf::Color::Blue);//Check //
    drawText(window, "Gnd", offsetX + 14.5f * scale, offsetY + 1 * scale - 10, 20, sf::Color::Black, font);

    drawColumn(window, offsetX - 3.5f * scale, offsetY - 5 * scale, offsetX + 13 * scale, offsetY - 5 * scale, sf::Color::Yellow);//Check //
    drawColumn(window, offsetX - 3.5f * scale, offsetY, offsetX + 13 * scale, offsetY, sf::Color::Green);
    map<string, float> pmos_unit_x, nmos_unit_x;
    map<string, pair<float, float>> coordinate_pmos, coordinate_nmos;
    for (size_t i = 0; i < logic_elements.size(); i++)
    {
        float x = offsetX + (1.5f + i * 2.0f) * scale;
        float y_top = offsetY - 0.5f * scale;
        float y_bot = offsetY + 5.5f * scale;
        float l_y = y_top + 6.1f * scale;
        drawColumn(window, x, y_top, x, y_bot, sf::Color::Red);
        drawText(window, string(1, logic_elements[i]), x, l_y, 12, sf::Color::Black, font);
    }
    
    //Connect pmos

    for (size_t i = 0; i < euler_path_pmos.size(); i++)
    {
        float ux = 1 + float(i);
        float sx = offsetX + ux * scale;
        float sy = offsetY - 5.0f * scale; //Check
        string lbl = euler_path_pmos[i];
        float label_y = sy - 0.1f * scale;
        drawText(window, lbl, sx - 8, label_y - 10, 12, sf::Color::Red, font);
        coordinate_pmos[lbl] = {ux, 5.0f};
    }
    float count = 0.2f * scale;
    
    vector<pair<float, float>> connected_pmos;
    for (const auto &e : pmos_edges) 
    {
        const auto &n1 = e.first;
        const auto &n2 = e.second;
        if (coordinate_pmos.find(n1) == coordinate_pmos.end() || coordinate_pmos.find(n2) == coordinate_pmos.end())
        {
            continue;
        }
        
        float ux1 = coordinate_pmos[n1].first, ux2 = coordinate_nmos[n2].first;
        float uy1 = coordinate_pmos[n1].second, uy2 = coordinate_pmos[n2].second;
        pair<float, float> p1 = {ux1, ux2 + 1};
        pair<float, float> p2 = {ux1, ux2 - 1};
        pair<float, float> p3 = {ux2, ux1 + 1};
        pair<float, float> p4 = {ux2, ux1 - 1};
        pair<float, float> p5 = {ux1 + 1, ux2};
        pair<float, float> p6 = {ux1 - 1, ux2};
        pair<float, float> p7 = {ux2 + 1, ux1};
        pair<float, float> p8 = {ux2 - 1, ux1};
        if (fabs(ux2 - ux1) > 1.0f)
        {
            connected_pmos.push_back({ux1, ux2});
            if (find(connected_pmos.begin(), connected_pmos.end(), p1) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p2) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p3) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p4) != connected_pmos.end())
            {
                continue;
            }
            else 
            {
                if (find(connected_pmos.begin(), connected_pmos.end(), p5) != connected_pmos.end() ||
                    find(connected_pmos.begin(), connected_pmos.end(), p6) != connected_pmos.end() ||
                    find(connected_pmos.begin(), connected_pmos.end(), p7) != connected_pmos.end() ||
                    find(connected_pmos.begin(), connected_pmos.end(), p8) != connected_pmos.end())
                {
                    continue;
                }
                else {
                    drawColumn(window, ux1 + offsetX, uy1 + offsetY, ux1 + offsetX, uy1 - count + offsetY, sf::Color::Blue);
                    drawColumn(window, ux2 + offsetX, uy2 + offsetY, ux2 + offsetX, uy2 - count + offsetY, sf::Color::Blue);
                    drawColumn(window, ux1 + offsetX, uy1 - count + offsetY, ux2 + offsetX, uy2 - count + offsetY, sf::Color::Blue);
                    drawCrossMarker(window, ux1 + offsetX, uy1 + offsetY, sf::Color::Black);
                    drawCrossMarker(window, ux2 + offsetX, uy2 + offsetY, sf::Color::Black);
                }
            }
        }
        count += 0.2f * scale;
    }

    //Connect nmos
    for (size_t i = 0; i < euler_path_nmos.size(); i++)
    {
        float ux = 1 + float(i);
        float sx = offsetX + ux * scale;
        float sy = offsetY + 0.0f * scale;
        string lbl = euler_path_nmos[i];
        drawText(window, lbl, sx - 8, sy + 5, 12, sf::Color::Red, font);
        nmos_unit_x[lbl] = ux;
        coordinate_nmos[lbl] = {sx, sy};
    }
    count = 0.2f * scale;
    
    vector<pair<float, float>> connected_nmos;
    for(auto &e : nmos_edges) {
        auto &n1 = e.first;
        auto &n2 = e.second;
        if (coordinate_nmos.find(n1) == coordinate_nmos.end() || coordinate_nmos.find(n2) == coordinate_nmos.end())
        {
            continue;
        }
        float ux1 = coordinate_nmos[n1].first, ux2 = coordinate_nmos[n2].first;
        float uy1 = coordinate_nmos[n1].second, uy2 = coordinate_nmos[n2].second;
        if (fabs(ux2 - ux1) > 1.f)
        {
            pair<float, float> n1 = {ux1, ux2 + 1};
            pair<float, float> n2 = {ux1, ux2 - 1};
            pair<float, float> n3 = {ux2, ux1 + 1};
            pair<float, float> n4 = {ux2, ux1 - 1};
            pair<float, float> n5 = {ux1 + 1, ux2};
            pair<float, float> n6 = {ux1 - 1, ux2};
            pair<float, float> n7 = {ux2 + 1, ux1};
            pair<float, float> n8 = {ux2 - 1, ux1};
            connected_nmos.push_back({ux1, ux2});
                if (find(connected_nmos.begin(), connected_nmos.end(), n1) != connected_nmos.end() ||
                    find(connected_nmos.begin(), connected_nmos.end(), n2) != connected_nmos.end() ||
                    find(connected_nmos.begin(), connected_nmos.end(), n3) != connected_nmos.end() ||
                    find(connected_nmos.begin(), connected_nmos.end(), n4) != connected_nmos.end())
                {
                    continue;   
                }     
                else 
                {
                    if (find(connected_nmos.begin(), connected_nmos.end(), n5) != connected_nmos.end() ||
                        find(connected_nmos.begin(), connected_nmos.end(), n6) != connected_nmos.end() ||
                        find(connected_nmos.begin(), connected_nmos.end(), n7) != connected_nmos.end() ||
                        find(connected_nmos.begin(), connected_nmos.end(), n8) != connected_nmos.end())
                    {
                        continue;
                    }
                    else 
                    {
                        drawColumn(window, ux1 + offsetX, uy1 + offsetY, ux1 + offsetX, uy1 + count + offsetY, sf::Color::Blue);
                        drawColumn(window, ux2 + offsetX, uy2 + offsetY, ux2 + offsetX, uy2 + count + offsetY, sf::Color::Blue);
                        drawColumn(window, ux1 + offsetX, uy1 + count + offsetY, ux2 + offsetX, uy2 + count + offsetY, sf::Color::Blue);
                        drawCrossMarker(window, ux1 + offsetX, uy1 + offsetY, sf::Color::Black);
                        drawCrossMarker(window, ux2 + offsetX, uy2 + offsetY, sf::Color::Black);
                    }
                }
        }
        count += 0.2f * scale;
    }
    //Connect PMOS with VDD 
    vector<float> vdd_already_connected;
    auto PmosIsConnected = [&](float x1, float x2) {
        for ( const auto &[a, b] : connected_pmos) {
            if ((a == x1 && b == x2) || ( a == x2 && b == x1))          
            {
                return  true;
            }
        }
        return false;
    };
    auto PmosIsVddConnected = [&](float x) {
        return find(vdd_already_connected.begin(), vdd_already_connected.end(), x) != vdd_already_connected.end();
    };

    set<string> unique_sources (source_nodes_pmos.begin(), source_nodes_pmos.end());
    vector<string> unique_list(unique_sources.begin(), unique_sources.end());

    for (size_t i = 0; i < unique_list.size(); i++)
    {
        for (size_t j = i + 1; j < unique_list.size(); j++)
        {
            const string &node_1 = unique_list[i];
            const string &node_2 = unique_list[j];
            if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_pmos[node_1].first;
            float x2 = coordinate_pmos[node_2].first;
            float y1 = coordinate_pmos[node_1].second;
            float y2 = coordinate_pmos[node_2].second;
            if (fabs(x2 - x1) == 1.0f)
            {
                vdd_already_connected.push_back(x2);
                vdd_already_connected.push_back(x1);
            }   
        }
    }
    if(unique_sources.size() > 1)
    {
        for (size_t i = 0; i < unique_list.size(); i++)
        {
            for (size_t j = i + 1; j < unique_list.size(); j++)
            {
                const string &node_1 = unique_list[i];
                const string &node_2 = unique_list[j];
                if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end())
                {
                    continue;
                }
                float x1 = coordinate_pmos[node_1].first;
                float y1 = coordinate_pmos[node_1].second;
                float x2 = coordinate_pmos[node_2].first;
                float y2 = coordinate_pmos[node_2].second;

                if (fabs(x2- x1) == 1.0f)
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3 + offsetX, offsetY + 5.0f * scale, x3 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                    drawCrossMarker(window, x3 + offsetX, y3 + offsetY, sf::Color::Blue);
                }
                else 
                {
                    if (PmosIsVddConnected(x1) || PmosIsVddConnected(x2))
                    {
                        if (PmosIsConnected(x1, x2) || PmosIsConnected(x2, x1))
                        {
                            continue;
                        }
                        else
                        {
                            if (PmosIsVddConnected(x1) && PmosIsVddConnected(x2))
                            {
                                continue;   
                            }
                            else if (PmosIsVddConnected(x1))
                            {
                                for(float i : vdd_already_connected) 
                                {
                                    if (PmosIsConnected(x2, i) || PmosIsConnected(i, x2))
                                    {
                                        vdd_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2 + offsetX, offsetY + 5.0f * scale, x2 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                                drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Blue);
                                vdd_already_connected.push_back(x2);
                                continue;
                            }
                            else if (PmosIsVddConnected(x2)) 
                            {
                                for(float i : vdd_already_connected) 
                                {
                                    if (PmosIsConnected(x1, i) || PmosIsConnected(i, x1))
                                    {
                                        vdd_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1 + offsetX, offsetY + 5.0f * scale, x1 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                                drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                                vdd_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    }
                    else 
                    {
                        if (PmosIsConnected(x1, x2) || PmosIsConnected(x2, x1))
                        {
                            drawColumn(window, x1 + offsetX, offsetY + 5.0f * scale, x1 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Blue); //Change color to blue
                            vdd_already_connected.push_back(x1);
                            vdd_already_connected.push_back(x2);
                        }
                        else 
                        {
                            drawColumn(window, x1 + offsetX, offsetY + 5.0f * scale, x1 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Blue); //Change color to blue
                            drawColumn(window, x2 + offsetX, offsetY + 5.0f * scale, x2 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Blue); //Change color to blue
                            vdd_already_connected.push_back(x1);
                            vdd_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }
    }
    else 
    {
       for(const string& node : source_nodes_pmos)
       {
            if (coordinate_pmos.find(node) == coordinate_pmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_pmos[node].first;
            float y1 = coordinate_pmos[node].second;
            drawColumn(window, x1 + offsetX, offsetY + 5.0f * scale, x1 + offsetX, offsetY + 6.0f * scale, sf::Color::Blue);
            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
       }
    }

    //Connect NMOS to GND 
    vector<float> gnd_already_connected;
    auto NmosIsConnected_GND = [&](float x1, float x2) {
        for (const auto &[a, b] : connected_nmos) {
            if ((a == x1 && b == x2) || ( a == x2 && b == x1))          
            {
                return  true;
            }
        }
        return false;
    };
    auto NmosIsGndConnected = [&](float x) {
        return find(gnd_already_connected.begin(), gnd_already_connected.end(), x) != gnd_already_connected.end();
    };

    set<string> unique_ground (source_nodes_nmos.begin(), source_nodes_nmos.end());
    vector<string> unique_list_gnd(unique_ground.begin(), unique_ground.end());

    for (size_t i = 0; i < unique_list_gnd.size(); i++)
    {
        for (size_t j = i + 1; j < unique_list_gnd.size(); j++)
        {
            const string &node_1 = unique_list[i];
            const string &node_2 = unique_list[j];
            if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_nmos[node_1].first;
            float x2 = coordinate_nmos[node_2].first;
            float y1 = coordinate_nmos[node_1].second;
            float y2 = coordinate_nmos[node_2].second;
            if (fabs(x2 - x1) == 1.0f)
            {
                gnd_already_connected.push_back(x2);
                gnd_already_connected.push_back(x1);
            }   
        }
    }
    if(unique_ground.size() > 1)
    {
        for (size_t i = 0; i < unique_list_gnd.size(); i++)
        {
            for (size_t j = i + 1; j < unique_list_gnd.size(); j++)
            {
                const string &node_1 = unique_list_gnd[i];
                const string &node_2 = unique_list_gnd[j];
                if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end())
                {
                    continue;
                }
                
                float x1 = coordinate_nmos[node_1].first;
                float y1 = coordinate_nmos[node_1].second;
                float x2 = coordinate_nmos[node_2].first;
                float y2 = coordinate_nmos[node_2].second;

                if (fabs(x2- x1) == 1.0f)
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3 + offsetX, offsetY + 0.0f * scale, x3 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                    drawCrossMarker(window, x3, y3, sf::Color::Black);
                }
                else 
                {
                    if (NmosIsGndConnected(x1) || NmosIsGndConnected(x2))
                    {
                        if (NmosIsConnected_GND(x1, x2) || NmosIsConnected_GND(x2, x1))
                        {
                            continue;
                        }
                        else
                        {
                            if (NmosIsGndConnected(x1) && NmosIsGndConnected(x2))
                            {
                                continue;   
                            }
                            else if (NmosIsGndConnected(x1))
                            {
                                for(float i : gnd_already_connected) 
                                {
                                    if (NmosIsConnected_GND(x2, i) || NmosIsConnected_GND(i, x2))
                                    {
                                        gnd_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2 + offsetX, offsetY, x2 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                                drawCrossMarker(window, x2 + offsetX, y2 + offsetX, sf::Color::Black);
                                gnd_already_connected.push_back(x2);
                                continue;
                            }
                            else if (NmosIsGndConnected(x2)) 
                            {
                                for(float i : gnd_already_connected) 
                                {
                                    if (NmosIsConnected_GND(x1, i) || NmosIsConnected_GND(i, x1))
                                    {
                                        gnd_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1 + offsetX, offsetY, x1 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                                drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                                gnd_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    }
                    else 
                    {
                        if (NmosIsConnected_GND(x1, x2) || NmosIsConnected_GND(x2, x1))
                        {
                            drawColumn(window, x1 + offsetX, offsetY, x1 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black); //Change color to blue
                            vdd_already_connected.push_back(x1);
                            vdd_already_connected.push_back(x2);
                        }
                        else 
                        {
                            drawColumn(window, x1 + offsetX, offsetY, x1 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black); //Change color to black
                            drawColumn(window, x2 + offsetX, offsetY, x2 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
                            drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Black); //Change color to black
                            vdd_already_connected.push_back(x1);
                            vdd_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }
    }
    else 
    {
       for(const string& node : source_nodes_nmos)
       {
        if (coordinate_nmos.find(node) == coordinate_nmos.end())
        {
            continue;
        }
        
            float x1 = coordinate_nmos[node].first;
            float y1 = coordinate_nmos[node].second;
            drawColumn(window, x1 + offsetX, offsetY, x1 + offsetX, offsetY - 1.0f * scale, sf::Color::Blue);
            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
       }
    }

    //Connect to output pmos
    vector<float> out_pmos_already_connected; 
    auto OutIsConnected = [&](float x1, float x2) {
        for (const auto &[a, b] : connected_pmos) {
            if ((a == x1 && b == x2) || ( a == x2 && b == x1))          
            {
                return  true;
            }
        }
        return false;
    };
    auto PmosIsOutConnected = [&](float x) {
        return find(out_pmos_already_connected.begin(), out_pmos_already_connected.end(), x) != out_pmos_already_connected.end();
    };

    set<string> unique_sources_pout (out_nodes_pmos.begin(), out_nodes_pmos.end());
    vector<string> unique_list_pout (unique_sources_pout.begin(), unique_sources_pout.end());

    for (size_t i = 0; i < unique_list_pout.size(); i++)
    {
        for (size_t j = i + 1; j < unique_list_pout.size(); j++)
        {
            const string& node_1 = unique_list_pout[i];
            const string& node_2 = unique_list_pout[j];
            if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end()) 
            {
                continue;
            }
            
            float x1 = coordinate_pmos[node_1].first;
            float x2 = coordinate_pmos[node_2].first;
            float y1 = coordinate_pmos[node_1].second;
            float y2 = coordinate_pmos[node_2].second;
            if (fabs(x2 - x1) == 1.0f)
            {
                out_pmos_already_connected.push_back(x1);
                out_pmos_already_connected.push_back(x2);
            }
        }
    }
    if (unique_sources_pout.size() > 1)
    {
        for (size_t i = 0; i < unique_list_pout.size(); i++)
        {
            for (size_t j = i + 1; j < unique_list_pout.size(); j++)
            {
                const string& node_1 = unique_list_pout[i];
                const string& node_2 = unique_list_pout[j];
                if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end())
                {
                    continue;
                }
                
                float x1 = coordinate_pmos[node_1].first;
                float x2 = coordinate_pmos[node_2].first;
                float y1 = coordinate_pmos[node_1].second;
                float y2 = coordinate_pmos[node_2].second;
                if (fabs(x2 - x1) == 1)
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3 + offsetX, y3 + 2.5f * scale + offsetX+ offsetY, x3 + offsetX, y3 + offsetY, sf::Color::Blue);
                    drawColumn(window, x3 + offsetX, y3 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                    drawCrossMarker(window, x3 + offsetX, y3 + offsetY, sf::Color::Black);
                }
                else
                {
                    if (PmosIsOutConnected(x1) || PmosIsOutConnected(x2))
                    {
                        if (OutIsConnected(x1, x2) || OutIsConnected(x2, x1))
                        {
                            continue;
                        }
                        else 
                        {
                            if (PmosIsOutConnected(x1) && PmosIsOutConnected(x2))
                            {
                                continue;
                            }
                            else if (PmosIsOutConnected(x1))
                            {
                                for(float i : out_pmos_already_connected) 
                                {
                                    if (OutIsConnected(x2, i) || OutIsConnected(i, x2))
                                    {
                                        out_pmos_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2 + offsetX, y2 - 2.5f * scale + offsetY, x2 + offsetX, y2 + offsetY, sf::Color::Blue);
                                drawColumn(window, x2 + offsetX, y2 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                                drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Black);
                                out_pmos_already_connected.push_back(x2);
                                continue;
                            }
                            else if (PmosIsOutConnected(x2))
                            {
                                for(float i : out_pmos_already_connected) 
                                {
                                    if (OutIsConnected(x1, i) || OutIsConnected(i, x1))
                                    {
                                        out_pmos_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                                drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                                drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                                out_pmos_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    }
                    else 
                    {
                        if (OutIsConnected(x1, x2) || OutIsConnected(x2, x1))
                        {
                            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                            out_pmos_already_connected.push_back(x1);
                            out_pmos_already_connected.push_back(x2);
                        }
                        else 
                        {
                            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                            drawColumn(window, x2 + offsetX, y2 - 2.5f * scale + offsetY, x2 + offsetX, y2 + offsetY, sf::Color::Blue);
                            drawColumn(window, x2 + offsetX, y2 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Black);
                            out_pmos_already_connected.push_back(x1);
                            out_pmos_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }   
    }
    else 
    {
        for(const string& node_pmos : out_nodes_pmos) 
        {
            if (coordinate_pmos.find(node_pmos) == coordinate_pmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_pmos[node_pmos].first;
            float y1 = coordinate_pmos[node_pmos].second;
            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, x1 + offsetX, 5.0f * scale + offsetY, sf::Color::Blue);
            drawColumn(window, x1 + offsetX, y1 - 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
        }
    }
    
    //Connect to output nmos
    vector<float> out_nmos_already_connected; 
    auto N_OutIsConnected = [&](float x1, float x2) {
        for (const auto &[a, b] : connected_nmos) {
            if ((a == x1 && b == x2) || ( a == x2 && b == x1))          
            {
                return  true;
            }
        }
        return false;
    };
    auto NmosIsOutConnected = [&](float x) {
        return find(out_nmos_already_connected.begin(), out_nmos_already_connected.end(), x) != out_nmos_already_connected.end();
    };

    set<string> unique_sources_nout (out_nodes_nmos.begin(), out_nodes_nmos.end());
    vector<string> unique_list_nout (unique_list_nout.begin(), unique_list_nout.end());

    for (size_t i = 0; i < unique_list_nout.size(); i++)
    {
        for (size_t j = i + 1; j < unique_list_nout.size(); j++)
        {
            const string& node_1 = unique_list_nout[i];
            const string& node_2 = unique_list_nout[j];
            if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_nmos[node_1].first;
            float x2 = coordinate_nmos[node_2].first;
            float y1 = coordinate_nmos[node_1].second;
            float y2 = coordinate_nmos[node_2].second;
            if (fabs(x2 - x1) == 1.0f)
            {
                out_pmos_already_connected.push_back(x1);
                out_pmos_already_connected.push_back(x2);
            }
        }
    }
    if (unique_sources_nout.size() > 1)
    {
        for (size_t i = 0; i < unique_list_nout.size(); i++)
        {
            for (size_t j = i + 1; j < unique_list_nout.size(); j++)
            {
                const string& node_1 = unique_list_nout[i];
                const string& node_2 = unique_list_nout[j];
                if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end())
                {
                    continue;
                }
                
                float x1 = coordinate_nmos[node_1].first;
                float x2 = coordinate_nmos[node_2].first;
                float y1 = coordinate_nmos[node_1].second;
                float y2 = coordinate_nmos[node_2].second;
                if (fabs(x2 - x1) == 1)
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3 + offsetX, y3 + 2.5f * scale + offsetY, x3 + offsetX, y3 + offsetY, sf::Color::Blue);
                    drawColumn(window, x3 + offsetX, y3 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                    drawCrossMarker(window, x3 + offsetX, y3 + offsetY, sf::Color::Black);
                }
                else
                {
                    if (NmosIsOutConnected(x1) || NmosIsOutConnected(x2))
                    {
                        if (N_OutIsConnected(x1, x2) || N_OutIsConnected(x2, x1))
                        {
                            continue;
                        }
                        else 
                        {
                            if (NmosIsOutConnected(x1) && NmosIsOutConnected(x2))
                            {
                                continue;
                            }
                            else if (NmosIsOutConnected(x1))
                            {
                                for(float i : out_nmos_already_connected) 
                                {
                                    if (N_OutIsConnected(x2, i) || N_OutIsConnected(i, x2))
                                    {
                                        out_nmos_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2 + offsetX, y2 + 2.5f * scale + offsetY, x2 + offsetX, y2 + offsetY, sf::Color::Blue);
                                drawColumn(window, x2 + offsetX, y2 + 2.5f * scale + offsetY, 12.0f * scale, 2.5f * scale + offsetY, sf::Color::Blue);
                                drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Black);
                                out_pmos_already_connected.push_back(x2);
                                continue;
                            }
                            else if (NmosIsOutConnected(x2))
                            {
                                for(float i : out_nmos_already_connected) 
                                {
                                    if (N_OutIsConnected(x1, i) || N_OutIsConnected(i, x1))
                                    {
                                        out_nmos_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                                drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                                drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                                out_pmos_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    }
                    else 
                    {
                        if (N_OutIsConnected(x1, x2) || N_OutIsConnected(x2, x1))
                        {
                            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                            out_nmos_already_connected.push_back(x1);
                            out_nmos_already_connected.push_back(x2);
                        }
                        else 
                        {
                            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
                            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x1 + offsetX, y1 + offsetY, sf::Color::Black);
                            drawColumn(window, x2 + offsetX, y2 + 2.5f * scale + offsetY, x2 + offsetX, y2 + offsetY, sf::Color::Blue);
                            drawColumn(window, x2 + offsetX, y2 + 2.5f * scale + offsetY, 22.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
                            drawCrossMarker(window, x2 + offsetX, y2 + offsetY, sf::Color::Black);
                            out_nmos_already_connected.push_back(x1);
                            out_nmos_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }   
    }
    else 
    {
        for(const string& node_nmos : out_nodes_nmos) 
        {
            if (coordinate_nmos.find(node_nmos) == coordinate_nmos.end())
            {
                continue;
            }
            
            float x1 = coordinate_nmos[node_nmos].first;
            float y1 = coordinate_nmos[node_nmos].second;
            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, x1 + offsetX, y1 + offsetY, sf::Color::Blue);
            drawColumn(window, x1 + offsetX, y1 + 2.5f * scale + offsetY, 12.0f * scale + offsetX, 2.5f * scale + offsetY, sf::Color::Blue);
            drawCrossMarker(window, x1, y1, sf::Color::Black);
        }
    }
    //Output 
    set<string> out_nodes;
    out_nodes.insert(out_nodes_pmos.begin(), out_nodes_pmos.end());
    out_nodes.insert(out_nodes_nmos.begin(), out_nodes_nmos.end());

    for(const string& node : out_nodes_pmos)
    {
        if (coordinate_pmos.find(node) != coordinate_pmos.end() && coordinate_nmos.find(node) != coordinate_nmos.end())
        {
            float x_pmos = coordinate_pmos[node].first;
            float y_pmos = coordinate_pmos[node].second;
            float x_nmos = coordinate_nmos[node].first;
            float y_nmos = coordinate_nmos[node].second;
            float x_output = x_pmos;
            float y_top = y_pmos - 0.3f * scale;
            float y_bot = y_nmos + 0.3f * scale;
            drawColumn(window, x_output + offsetX, y_top + offsetY, x_output + offsetX, y_bot + offsetY, sf::Color(255, 165, 0));
            drawCrossMarker(window, x_output + offsetX, (y_top + y_bot) / 2.0f + offsetY, sf::Color(255, 165, 0));
            drawText(window, "OUT", x_output + 0.2f + offsetX, (y_top + y_bot) / 2.0f + offsetY, 10, sf::Color(255, 165, 0), font);
        }   
    }
}

// NVL added: PLS CHECK THIS
// Hàm trích xuất cạnh từ graph sang vector<pair<string, string>>
vector<pair<string, string>> extractEdges(const graph& g) {
    vector<pair<string, string>> edges;
    if (g.adjList.empty())
    {
        cerr << "Warning" << endl;
        return edges;
    }
    
    for (const auto& node : g.adjList) {
        const string& u = node.first; // Node hiện tại
        for (const string& v : node.second) { // Các hàng xóm
            // Chỉ thêm cạnh nếu u < v để tránh trùng lặp
            if (u < v) {
                edges.emplace_back(u, v);
            }
        }
    }
    return edges;
}

int main() {
    // Tạo cửa sổ SFML với VideoMode(width, height)
    sf::RenderWindow window(sf::VideoMode(1000, 800), "Stick Diagram Test");
    // Không gọi setVerticalSyncEnabled nữa, sử dụng framelimit thay thế:
    window.setFramerateLimit(60);

    // Load font (thử nhiều đường dẫn)
    sf::Font font;
    bool loaded = false;
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "LiberationSans-Regular.ttf",   // nếu bạn copy file vào thư mục chạy
        "DejaVuSans.ttf"
    };

    for (auto path : fontPaths) {
        if (font.loadFromFile(path)) {
            std::cout << "Loaded font from: " << path << "\n";
            loaded = true;
            break;
        }
    }
    if (!loaded) {
        std::cerr << "Failed to load any font. Checked paths:\n";
        for (auto path : fontPaths)
            std::cerr << "  " << path << "\n";
        return EXIT_FAILURE;
    }

    // Chuỗi logic để kiểm thử
    std::string expression = "A*B+C*D";
    Result result = CreateAll(expression);

    // Đường đi Euler cho PMOS và NMOS
    auto euler_path_pmos = result.euler_path_pmos;
    auto euler_path_nmos = result.euler_path_nmos;

    // Các cạnh (edges) giữa các transistor
    std::vector<std::pair<std::string, std::string>> g_pmos = extractEdges(result.g_pmos);
    std::vector<std::pair<std::string, std::string>> g_nmos = extractEdges(result.g_nmos);

    // Các nút nguồn (VDD/GND) và đầu ra
    std::vector<std::string> source_nodes_pmos = result.source_nodes_pmos;
    std::vector<std::string> source_nodes_nmos = result.source_nodes_nmos;
    std::vector<std::string> out_nodes_pmos    = result.out_nodes_pmos;
    std::vector<std::string> out_nodes_nmos    = result.out_nodes_nmos;

    // Tạo logic elements từ đường đi
    auto logic_elements = create_logic_element(euler_path_nmos);

    // Vòng lặp chính
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Vẽ sơ đồ
        drawStickDiagram(
            window,
            font,
            logic_elements,
            euler_path_pmos,
            euler_path_nmos,
            g_pmos,
            g_nmos,
            source_nodes_pmos,
            source_nodes_nmos,
            out_nodes_pmos,
            out_nodes_nmos
        );

        window.display();
    }

    return 0;
}

