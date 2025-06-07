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
void drawText(sf::RenderWindow& window, const std::string& content, float x, float y, int fontSize, sf::Color color, sf::Font& font, float scale = 50.0f, float offsetX = 200.0f, float offsetY = 300.0f) {
    sf::Text text(content, font, static_cast<unsigned int>(fontSize));
    text.setFillColor(color);
    text.setPosition(sf::Vector2f(x * scale + offsetX, y * scale + offsetY));
    window.draw(text);
}

// Draw column (line)
void drawColumn(sf::RenderWindow& window, float x1, float y1, float x2, float y2, const sf::Color& color, float scale = 50.0f, float offsetX = 200.0f, float offsetY = 300.0f) {
    sf::Vertex line[] = {
        sf::Vertex{sf::Vector2f(x1 * scale + offsetX, y1 * scale + offsetY), sf::Color(color)},
        sf::Vertex{sf::Vector2f(x2 * scale + offsetX, y2 * scale + offsetY), sf::Color(color)}
    };
    window.draw(line, 2, sf::PrimitiveType::Lines);
}

// Draw cross marker
void drawCrossMarker(sf::RenderWindow& window, float x, float y, sf::Color color, float scale = 50.0f, float offsetX = 200.0f, float offsetY = 300.0f) {
    const float s = 0.1f; // Size of cross in coordinate units
    sf::Vertex diag_1[] = 
    {
        {{(x - s) * scale + offsetX, (y - s) * scale + offsetY}, color},
        {{(x + s) * scale + offsetX, (y + s) * scale + offsetY}, color}
    };
    sf::Vertex diag_2[] = 
    {
        {{(x - s) * scale + offsetX, (y + s) * scale + offsetY}, color},
        {{(x + s) * scale + offsetX, (y - s) * scale + offsetY}, color}
    };
    window.draw(diag_1, 2, sf::PrimitiveType::Lines);
    window.draw(diag_2, 2, sf::PrimitiveType::Lines);
}

// Draw stick diagram
void drawStickDiagram(sf::RenderWindow& window, sf::Font& font, const vector<char>& logic_elements, const vector<string>& euler_path_pmos, const vector<string>& euler_path_nmos, const vector<pair<string, string>>& pmos_edges, const vector<pair<string, string>>& nmos_edges, const vector<string>& source_nodes_pmos, const vector<string>& source_nodes_nmos, const vector<string>& out_nodes_pmos, const vector<string>& out_nodes_nmos) {
    // Kiểm tra rỗng cho tất cả các đầu vào
    bool hasError = false;
    hasError |= checkEmptyVector(logic_elements, "logic_elements");
    hasError |= checkEmptyVector(euler_path_pmos, "euler_path_pmos");
    hasError |= checkEmptyVector(euler_path_nmos, "euler_path_nmos");
    checkEmptyVector(pmos_edges, "pmos_edges");
    checkEmptyVector(nmos_edges, "nmos_edges");
    hasError |= checkEmptyVector(source_nodes_pmos, "source_nodes_pmos");
    hasError |= checkEmptyVector(source_nodes_nmos, "source_nodes_nmos");
    hasError |= checkEmptyVector(out_nodes_pmos, "out_nodes_pmos");
    hasError |= checkEmptyVector(out_nodes_nmos, "out_nodes_nmos");

    if (hasError) 
    {
        drawText(window, "Error: Missing critical data. Check console for details.", 0, 7, 20, sf::Color::Red, font);
        return;
    }

    const float scale = 50.0f; // Scale factor for visibility in SFML window

    // Draw VDD and GND
    drawColumn(window, -4.5f, 6.0f, 14.0f, 6.0f, sf::Color::Blue, scale);
    drawText(window, "Vdd", 14.5f, 6.0f, 12, sf::Color::Black, font, scale);
    drawColumn(window, -4.5f, -1.0f, 14.0f, -1.0f, sf::Color::Blue, scale);
    drawText(window, "Gnd", 14.5f, -1.0f, 12, sf::Color::Black, font, scale);

    // Draw Pdiff and Ndiff
    drawColumn(window, -3.5f, 5.0f, 13.0f, 5.0f, sf::Color::Yellow, scale);
    drawColumn(window, -3.5f, 0.0f, 13.0f, 0.0f, sf::Color::Green, scale);

    map<string, float> pmos_unit_x, nmos_unit_x;
    map<string, pair<float, float>> coordinate_pmos, coordinate_nmos;

    // Draw logic elements
    for (size_t i = 0; i < logic_elements.size(); i++) 
    {
        float x = 1.5f + float(i) * 2.0f;
        float y_top = -0.5f;
        float y_bot = 5.5f;
        float l_y = 5.6f;
        drawColumn(window, x, y_top, x, y_bot, sf::Color::Red, scale);
        drawText(window, string(1, logic_elements[i]), x, l_y, 10, sf::Color::Black, font, scale);
    }

    // Connect PMOS
    for (size_t i = 0; i < euler_path_pmos.size(); i++) 
    {
        float ux = 1.0f + float(i);
        float sx = ux;
        float sy = 5.0f;
        string lbl = euler_path_pmos[i];
        float label_y = 5.1f;
        drawText(window, lbl, sx, label_y, 10, sf::Color::Red, font, scale);
        coordinate_pmos[lbl] = {ux, 5.0f};
        pmos_unit_x[lbl] = ux;
    }
    float count = 0.2f;

    vector<pair<float, float>> connected_pmos;
    for (const auto& e : pmos_edges) 
    {
        const auto& n1 = e.first;
        const auto& n2 = e.second;
        if (coordinate_pmos.find(n1) == coordinate_pmos.end() || coordinate_pmos.find(n2) == coordinate_pmos.end()) 
        {
            continue;
        }

        float ux1 = coordinate_pmos[n1].first, uy1 = coordinate_pmos[n1].second;
        float ux2 = coordinate_pmos[n2].first, uy2 = coordinate_pmos[n2].second;
        if (fabs(ux2 - ux1) > 1.0f) 
        {
            connected_pmos.push_back({ux1, ux2});
            pair<float, float> p1 = {ux1, ux2 + 1};
            pair<float, float> p2 = {ux1, ux2 - 1};
            pair<float, float> p3 = {ux2, ux1 + 1};
            pair<float, float> p4 = {ux2, ux1 - 1};
            pair<float, float> p5 = {ux1 + 1, ux2};
            pair<float, float> p6 = {ux1 - 1, ux2};
            pair<float, float> p7 = {ux2 + 1, ux1};
            pair<float, float> p8 = {ux2 - 1, ux1};
            if (find(connected_pmos.begin(), connected_pmos.end(), p1) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p2) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p3) != connected_pmos.end() ||
                find(connected_pmos.begin(), connected_pmos.end(), p4) != connected_pmos.end()) 
            {
                continue;
            } 
            else if (find(connected_pmos.begin(), connected_pmos.end(), p5) != connected_pmos.end() ||
                       find(connected_pmos.begin(), connected_pmos.end(), p6) != connected_pmos.end() ||
                       find(connected_pmos.begin(), connected_pmos.end(), p7) != connected_pmos.end() ||
                       find(connected_pmos.begin(), connected_pmos.end(), p8) != connected_pmos.end()) 
            {
                continue;
            } 
            else 
            {
                drawColumn(window, ux1, uy1, ux1, uy1 - count, sf::Color::Blue, scale);
                drawColumn(window, ux2, uy2, ux2, uy2 - count, sf::Color::Blue, scale);
                drawColumn(window, ux1, uy1 - count, ux2, uy2 - count, sf::Color::Blue, scale);
                drawCrossMarker(window, ux1, uy1, sf::Color::Black, scale);
                drawCrossMarker(window, ux2, uy2, sf::Color::Black, scale);
            }
        }
        count += 0.2f;
    }

    // Connect NMOS
    for (size_t i = 0; i < euler_path_nmos.size(); i++) 
    {
        float ux = 1.0f + float(i);
        float sx = ux;
        float sy = 0.0f;
        string lbl = euler_path_nmos[i];
        float label_y = 0.1f;
        drawText(window, lbl, sx, label_y, 10, sf::Color::Red, font, scale);
        nmos_unit_x[lbl] = ux;
        coordinate_nmos[lbl] = {ux, sy};
    }
    count = 0.2f;

    vector<pair<float, float>> connected_nmos;
    for (const auto& e : nmos_edges) 
    {
        const auto& n1 = e.first;
        const auto& n2 = e.second;
        if (coordinate_nmos.find(n1) == coordinate_nmos.end() || coordinate_nmos.find(n2) == coordinate_nmos.end()) 
        {
            continue;
        }
        float ux1 = coordinate_nmos[n1].first, uy1 = coordinate_nmos[n1].second;
        float ux2 = coordinate_nmos[n2].first, uy2 = coordinate_nmos[n2].second;
        if (fabs(ux2 - ux1) > 1.0f) 
        {
            connected_nmos.push_back({ux1, ux2});
            pair<float, float> n1 = {ux1, ux2 + 1};
            pair<float, float> n2 = {ux1, ux2 - 1};
            pair<float, float> n3 = {ux2, ux1 + 1};
            pair<float, float> n4 = {ux2, ux1 - 1};
            pair<float, float> n5 = {ux1 + 1, ux2};
            pair<float, float> n6 = {ux1 - 1, ux2};
            pair<float, float> n7 = {ux2 + 1, ux1};
            pair<float, float> n8 = {ux2 - 1, ux1};
            if (find(connected_nmos.begin(), connected_nmos.end(), n1) != connected_nmos.end() ||
                find(connected_nmos.begin(), connected_nmos.end(), n2) != connected_nmos.end() ||
                find(connected_nmos.begin(), connected_nmos.end(), n3) != connected_nmos.end() ||
                find(connected_nmos.begin(), connected_nmos.end(), n4) != connected_nmos.end()) 
            {
                continue;
            } 
            else if (find(connected_nmos.begin(), connected_nmos.end(), n5) != connected_nmos.end() ||
                       find(connected_nmos.begin(), connected_nmos.end(), n6) != connected_nmos.end() ||
                       find(connected_nmos.begin(), connected_nmos.end(), n7) != connected_nmos.end() ||
                       find(connected_nmos.begin(), connected_nmos.end(), n8) != connected_nmos.end()) 
            {
                continue;
            } 
            else 
            {
                drawColumn(window, ux1, uy1, ux1, uy1 + count, sf::Color::Blue, scale);
                drawColumn(window, ux2, uy2, ux2, uy2 + count, sf::Color::Blue, scale);
                drawColumn(window, ux1, uy1 + count, ux2, uy2 + count, sf::Color::Blue, scale);
                drawCrossMarker(window, ux1, uy1, sf::Color::Black, scale);
                drawCrossMarker(window, ux2, uy2, sf::Color::Black, scale);
            }
        }
        count += 0.2f;
    }

    // Connect PMOS with VDD
    vector<float> vdd_already_connected;
    auto PmosIsConnected = [&](float x1, float x2) 
    {
        for (const auto& [a, b] : connected_pmos) 
        {
            if ((a == x1 && b == x2) || (a == x2 && b == x1)) 
            {
                return true;
            }
        }
        return false;
    };
    auto PmosIsVddConnected = [&](float x) 
    {
        return find(vdd_already_connected.begin(), vdd_already_connected.end(), x) != vdd_already_connected.end();
    };

    set<string> unique_sources(source_nodes_pmos.begin(), source_nodes_pmos.end());
    vector<string> unique_list(unique_sources.begin(), unique_sources.end());

    for (size_t i = 0; i < unique_list.size(); i++) 
    {
        for (size_t j = i + 1; j < unique_list.size(); j++) 
        {
            const string& node_1 = unique_list[i];
            const string& node_2 = unique_list[j];
            if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end()) 
            {
                continue;
            }

            float x1 = coordinate_pmos[node_1].first;
            float x2 = coordinate_pmos[node_2].first;
            if (fabs(x2 - x1) == 1.0f) 
            {
                vdd_already_connected.push_back(x2);
                vdd_already_connected.push_back(x1);
            }
        }
    }

    if (unique_sources.size() > 1) 
    {
        for (size_t i = 0; i < unique_list.size(); i++) 
        {
            for (size_t j = i + 1; j < unique_list.size(); j++) 
            {
                const string& node_1 = unique_list[i];
                const string& node_2 = unique_list[j];
                if (coordinate_pmos.find(node_1) == coordinate_pmos.end() || coordinate_pmos.find(node_2) == coordinate_pmos.end()) 
                {
                    continue;
                }
                float x1 = coordinate_pmos[node_1].first;
                float y1 = coordinate_pmos[node_1].second;
                float x2 = coordinate_pmos[node_2].first;
                float y2 = coordinate_pmos[node_2].second;

                if (fabs(x2 - x1) == 1.0f) 
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3, 5.0f, x3, 6.0f, sf::Color::Blue, scale);
                    drawCrossMarker(window, x3, y3, sf::Color::Black, scale);
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
                                for (float i : vdd_already_connected) 
                                {
                                    if (PmosIsConnected(x2, i) || PmosIsConnected(i, x2)) 
                                    {
                                        vdd_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2, 5.0f, x2, 6.0f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x2, y2, sf::Color::Blue, scale);
                                vdd_already_connected.push_back(x2);
                                continue;
                            } 
                            else if (PmosIsVddConnected(x2)) 
                            {
                                for (float i : vdd_already_connected) 
                                {
                                    if (PmosIsConnected(x1, i) || PmosIsConnected(i, x1)) 
                                    {
                                        vdd_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1, 5.0f, x1, 6.0f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x1, y1, sf::Color::Blue, scale);
                                vdd_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    } 
                    else 
                    {
                        if (PmosIsConnected(x1, x2) || PmosIsConnected(x2, x1)) 
                        {
                            drawColumn(window, x1, 5.0f, x1, 6.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Blue, scale);
                            vdd_already_connected.push_back(x1);
                            vdd_already_connected.push_back(x2);
                        } 
                        else 
                        {
                            drawColumn(window, x1, 5.0f, x1, 6.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Blue, scale);
                            drawColumn(window, x2, 5.0f, x2, 6.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x2, y2, sf::Color::Blue, scale);
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
        for (const string& node : source_nodes_pmos) 
        {
            if (coordinate_pmos.find(node) == coordinate_pmos.end()) 
            {
                continue;
            }
            float x1 = coordinate_pmos[node].first;
            float y1 = coordinate_pmos[node].second;
            drawColumn(window, x1, 5.0f, x1, 6.0f, sf::Color::Blue, scale);
            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
        }
    }

    // Connect NMOS to gravyND
    vector<float> gnd_already_connected;
    auto NmosIsConnected = [&](float x1, float x2) 
    {
        for (const auto& [a, b] : connected_nmos) 
        {
            if ((a == x1 && b == x2) || (a == x2 && b == x1)) 
            {
                return true;
            }
        }
        return false;
    };
    auto NmosIsGndConnected = [&](float x) 
    {
        return find(gnd_already_connected.begin(), gnd_already_connected.end(), x) != gnd_already_connected.end();
    };

    set<string> unique_ground(source_nodes_nmos.begin(), source_nodes_nmos.end());
    vector<string> unique_list_gnd(unique_ground.begin(), unique_ground.end());

    for (size_t i = 0; i < unique_list_gnd.size(); i++) 
    {
        for (size_t j = i + 1; j < unique_list_gnd.size(); j++) 
        {
            const string& node_1 = unique_list_gnd[i];
            const string& node_2 = unique_list_gnd[j];
            if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end()) 
            {
                continue;
            }
            float x1 = coordinate_nmos[node_1].first;
            float x2 = coordinate_nmos[node_2].first;
            if (fabs(x2 - x1) == 1.0f) 
            {
                gnd_already_connected.push_back(x2);
                gnd_already_connected.push_back(x1);
            }
        }
    }

    if (unique_ground.size() > 1) 
    {
        for (size_t i = 0; i < unique_list_gnd.size(); i++) 
        {
            for (size_t j = i + 1; j < unique_list_gnd.size(); j++) 
            {
                const string& node_1 = unique_list_gnd[i];
                const string& node_2 = unique_list_gnd[j];
                if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end()) 
                {
                    continue;
                }
                float x1 = coordinate_nmos[node_1].first;
                float y1 = coordinate_nmos[node_1].second;
                float x2 = coordinate_nmos[node_2].first;
                float y2 = coordinate_nmos[node_2].second;

                if (fabs(x2 - x1) == 1.0f) 
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3, 0.0f, x3, -1.0f, sf::Color::Blue, scale);
                    drawCrossMarker(window, x3, y3, sf::Color::Black, scale);
                } 
                else
                {
                    if (NmosIsGndConnected(x1) || NmosIsGndConnected(x2)) {
                        if (NmosIsConnected(x1, x2) || NmosIsConnected(x2, x1)) 
                        {
                            continue;
                        } else 
                        {
                            if (NmosIsGndConnected(x1) && NmosIsGndConnected(x2)) 
                            {
                                continue;
                            } else if (NmosIsGndConnected(x1)) 
                            {
                                for (float i : gnd_already_connected) 
                                {
                                    if (NmosIsConnected(x2, i) || NmosIsConnected(i, x2)) 
                                    {
                                        gnd_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2, 0.0f, x2, -1.0f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                                gnd_already_connected.push_back(x2);
                                continue;
                            } 
                            else if (NmosIsGndConnected(x2)) 
                            {
                                for (float i : gnd_already_connected) 
                                {
                                    if (NmosIsConnected(x1, i) || NmosIsConnected(i, x1)) 
                                    {
                                        gnd_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1, 0.0f, x1, -1.0f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                                gnd_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    } 
                    else 
                    {
                        if (NmosIsConnected(x1, x2) || NmosIsConnected(x2, x1)) 
                        {
                            drawColumn(window, x1, 0.0f, x1, -1.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            gnd_already_connected.push_back(x1);
                            gnd_already_connected.push_back(x2);
                        } 
                        else 
                        {
                            drawColumn(window, x1, 0.0f, x1, -1.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            drawColumn(window, x2, 0.0f, x2, -1.0f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                            gnd_already_connected.push_back(x1);
                            gnd_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }
    } 
    else 
    {
        for (const string& node : source_nodes_nmos) 
        {
            if (coordinate_nmos.find(node) == coordinate_nmos.end()) 
            {
                continue;
            }
            float x1 = coordinate_nmos[node].first;
            float y1 = coordinate_nmos[node].second;
            drawColumn(window, x1, 0.0f, x1, -1.0f, sf::Color::Blue, scale);
            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
        }
    }

    // Connect to output PMOS
    vector<float> out_pmos_already_connected;
    auto OutIsConnected = [&](float x1, float x2) 
    {
        for (const auto& [a, b] : connected_pmos) 
        {
            if ((a == x1 && b == x2) || (a == x2 && b == x1)) 
            {
                return true;
            }
        }
        return false;
    };
    auto PmosIsOutConnected = [&](float x) 
    {
        return find(out_pmos_already_connected.begin(), out_pmos_already_connected.end(), x) != out_pmos_already_connected.end();
    };

    set<string> unique_sources_pout(out_nodes_pmos.begin(), out_nodes_pmos.end());
    vector<string> unique_list_pout(unique_sources_pout.begin(), unique_sources_pout.end());

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
            if (fabs(x2 - x1) == 1.0f) {
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
                float y1 = coordinate_pmos[node_1].second;
                float x2 = coordinate_pmos[node_2].first;
                float y2 = coordinate_pmos[node_2].second;
                if (fabs(x2 - x1) == 1.0f) 
                {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3, y3 - 2.5f, x3, y3, sf::Color::Blue, scale);
                    drawColumn(window, x3, y3 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                    drawCrossMarker(window, x3, y3, sf::Color::Black, scale);
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
                                for (float i : out_pmos_already_connected) 
                                {
                                    if (OutIsConnected(x2, i) || OutIsConnected(i, x2)) 
                                    {
                                        out_pmos_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2, y2 - 2.5f, x2, y2, sf::Color::Blue, scale);
                                drawColumn(window, x2, y2 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                                out_pmos_already_connected.push_back(x2);
                                continue;
                            } 
                            else if (PmosIsOutConnected(x2)) 
                            {
                                for (float i : out_pmos_already_connected) 
                                {
                                    if (OutIsConnected(x1, i) || OutIsConnected(i, x1)) 
                                    {
                                        out_pmos_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1, y1 - 2.5f, x1, y1, sf::Color::Blue, scale);
                                drawColumn(window, x1, y1 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                                out_pmos_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    } 
                    else 
                    {
                        if (OutIsConnected(x1, x2) || OutIsConnected(x2, x1)) 
                        {
                            drawColumn(window, x1, y1 - 2.5f, x1, y1, sf::Color::Blue, scale);
                            drawColumn(window, x1, y1 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            out_pmos_already_connected.push_back(x1);
                            out_pmos_already_connected.push_back(x2);
                        } 
                        else 
                        {
                            drawColumn(window, x1, y1 - 2.5f, x1, y1, sf::Color::Blue, scale);
                            drawColumn(window, x1, y1 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            drawColumn(window, x2, y2 - 2.5f, x2, y2, sf::Color::Blue, scale);
                            drawColumn(window, x2, y2 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                            out_pmos_already_connected.push_back(x1);
                            out_pmos_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }
    } else {
        for (const string& node_pmos : out_nodes_pmos) {
            if (coordinate_pmos.find(node_pmos) == coordinate_pmos.end()) {
                continue;
            }
            float x1 = coordinate_pmos[node_pmos].first;
            float y1 = coordinate_pmos[node_pmos].second;
            drawColumn(window, x1, y1 - 2.5f, x1, y1, sf::Color::Blue, scale);
            drawColumn(window, x1, y1 - 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
        }
    }

    // Connect to output NMOS
    vector<float> out_nmos_already_connected;
    auto N_OutIsConnected = [&](float x1, float x2) {
        for (const auto& [a, b] : connected_nmos) {
            if ((a == x1 && b == x2) || (a == x2 && b == x1)) {
                return true;
            }
        }
        return false;
    };
    auto NmosIsOutConnected = [&](float x) {
        return find(out_nmos_already_connected.begin(), out_nmos_already_connected.end(), x) != out_nmos_already_connected.end();
    };

    set<string> unique_sources_nout(out_nodes_nmos.begin(), out_nodes_nmos.end());
    vector<string> unique_list_nout(unique_sources_nout.begin(), unique_sources_nout.end());

    for (size_t i = 0; i < unique_list_nout.size(); i++) {
        for (size_t j = i + 1; j < unique_list_nout.size(); j++) {
            const string& node_1 = unique_list_nout[i];
            const string& node_2 = unique_list_nout[j];
            if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end()) {
                continue;
            }
            float x1 = coordinate_nmos[node_1].first;
            float x2 = coordinate_nmos[node_2].first;
            if (fabs(x2 - x1) == 1.0f) {
                out_nmos_already_connected.push_back(x1);
                out_nmos_already_connected.push_back(x2);
            }
        }
    }

    if (unique_sources_nout.size() > 1) {
        for (size_t i = 0; i < unique_list_nout.size(); i++) {
            for (size_t j = i + 1; j < unique_list_nout.size(); j++) {
                const string& node_1 = unique_list_nout[i];
                const string& node_2 = unique_list_nout[j];
                if (coordinate_nmos.find(node_1) == coordinate_nmos.end() || coordinate_nmos.find(node_2) == coordinate_nmos.end()) {
                    continue;
                }
                float x1 = coordinate_nmos[node_1].first;
                float y1 = coordinate_nmos[node_1].second;
                float x2 = coordinate_nmos[node_2].first;
                float y2 = coordinate_nmos[node_2].second;
                if (fabs(x2 - x1) == 1.0f) {
                    float x3 = (x1 + x2) / 2.0f;
                    float y3 = (y1 + y2) / 2.0f;
                    drawColumn(window, x3, y3 + 2.5f, x3, y3, sf::Color::Blue, scale);
                    drawColumn(window, x3, y3 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                    drawCrossMarker/window, x3, y3, sf::Color::Black, scale);
                } else {
                    if (NmosIsOutConnected(x1) || NmosIsOutConnected(x2)) {
                        if (N_OutIsConnected(x1, x2) || N_OutIsConnected(x2, x1)) {
                            continue;
                        } else {
                            if (NmosIsOutConnected(x1) && NmosIsOutConnected(x2)) {
                                continue;
                            } else if (NmosIsOutConnected(x1)) {
                                for (float i : out_nmos_already_connected) {
                                    if (N_OutIsConnected(x2, i) || N_OutIsConnected(i, x2)) {
                                        out_nmos_already_connected.push_back(x2);
                                        continue;
                                    }
                                }
                                drawColumn(window, x2, y2 + 2.5f, x2, y2, sf::Color::Blue, scale);
                                drawColumn(window, x2, y2 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                                out_nmos_already_connected.push_back(x2);
                                continue;
                            } else if (NmosIsOutConnected(x2)) {
                                for (float i : out_nmos_already_connected) {
                                    if (N_OutIsConnected(x1, i) || N_OutIsConnected(i, x1)) {
                                        out_nmos_already_connected.push_back(x1);
                                        continue;
                                    }
                                }
                                drawColumn(window, x1, y1 + 2.5f, x1, y1, sf::Color::Blue, scale);
                                drawColumn(window, x1, y1 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                                drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                                out_nmos_already_connected.push_back(x1);
                                continue;
                            }
                        }
                    } else {
                        if (N_OutIsConnected(x1, x2) || N_OutIsConnected(x2, x1)) {
                            drawColumn(window, x1, y1 + 2.5f, x1, y1, sf::Color::Blue, scale);
                            drawColumn(window, x1, y1 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            out_nmos_already_connected.push_back(x1);
                            out_nmos_already_connected.push_back(x2);
                        } else {
                            drawColumn(window, x1, y1 + 2.5f, x1, y1, sf::Color::Blue, scale);
                            drawColumn(window, x1, y1 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
                            drawColumn(window, x2, y2 + 2.5f, x2, y2, sf::Color::Blue, scale);
                            drawColumn(window, x2, y2 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
                            drawCrossMarker(window, x2, y2, sf::Color::Black, scale);
                            out_nmos_already_connected.push_back(x1);
                            out_nmos_already_connected.push_back(x2);
                        }
                    }
                }
            }
        }
    } else {
        for (const string& node_nmos : out_nodes_nmos) {
            if (coordinate_nmos.find(node_nmos) == coordinate_nmos.end()) {
                continue;
            }
            float x1 = coordinate_nmos[node_nmos].first;
            float y1 = coordinate_nmos[node_nmos].second;
            drawColumn(window, x1, y1 + 2.5f, x1, y1, sf::Color::Blue, scale);
            drawColumn(window, x1, y1 + 2.5f, 12.0f, 2.5f, sf::Color::Blue, scale);
            drawCrossMarker(window, x1, y1, sf::Color::Black, scale);
        }
    }

    // Output
    set<string> out_nodes;
    out_nodes.insert(out_nodes_pmos.begin(), out_nodes_pmos.end());
    out_nodes.insert(out_nodes_nmos.begin(), out_nodes_nmos.end());

    for (const string& node : out_nodes) {
        if (coordinate_pmos.find(node) != coordinate_pmos.end() && coordinate_nmos.find(node) != coordinate_nmos.end()) {
            float x_pmos = coordinate_pmos[node].first;
            float y_pmos = coordinate_pmos[node].second;
            float x_nmos = coordinate_nmos[node].first;
            float y_nmos = coordinate_nmos[node].second;
            float x_output = x_pmos;
            float y_top = y_pmos - 0.3f;
            float y_bot = y_nmos + 0.3f;
            drawColumn(window, x_output, y_top, x_output, y_bot, sf::Color(255, 165, 0), scale);
            drawCrossMarker(window, x_output, (y_top + y_bot) / 2.0f, sf::Color(255, 165, 0), scale);
            drawText(window, "OUT", x_output + 0.2f, (y_top + y_bot) / 2.0f, 10, sf::Color(255, 165, 0), font, scale);
        }
    }
    // Draw output label 'Y' as in Python
    drawText(window, "Y", 12.0f, 2.5f, 15, sf::Color::Red, font, scale);
}


// Hàm trích xuất cạnh từ graph sang vector<pair<string, string>>
vector<pair<string, string>> extractEdges(const graph& g) {
    vector<pair<string, string>> edges;
    if (g.adjList.empty()) {
        cerr << "Warning: Graph is empty!" << endl;
        return edges;
    }
    for (const auto& node : g.adjList) {
        const string& u = node.first;
        for (const string& v : node.second) {
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

