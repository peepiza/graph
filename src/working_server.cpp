#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>

using namespace std;

string get_html() {
    return "<!DOCTYPE html>\n"
"<html>\n"
"<head><meta charset='UTF-8'><title>Визуализатор графов</title>\n"
"<style>\n"
"* { margin: 0; padding: 0; box-sizing: border-box; }\n"
"body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #f0f4f8 0%, #e2e8f0 100%); min-height: 100vh; padding: 20px; }\n"
".container { max-width: 1400px; margin: 0 auto; }\n"
"h1 { text-align: center; color: #1a3a6e; margin-bottom: 10px; }\n"
".subtitle { text-align: center; color: #4a6a8e; margin-bottom: 30px; }\n"
".flex { display: flex; gap: 25px; flex-wrap: wrap; }\n"
".panel { flex: 1; min-width: 300px; background: white; border-radius: 16px; box-shadow: 0 4px 20px rgba(0,0,0,0.08); overflow: hidden; }\n"
".panel-header { background: #1e5ad9; color: white; padding: 15px 20px; font-weight: 600; }\n"
".panel-body { padding: 20px; }\n"
".preview { flex: 2; min-width: 500px; background: white; border-radius: 16px; box-shadow: 0 4px 20px rgba(0,0,0,0.08); overflow: hidden; }\n"
".preview-header { background: #1e5ad9; color: white; padding: 15px 20px; font-weight: 600; }\n"
".preview-content { padding: 20px; background: #f8fafc; min-height: 550px; display: flex; align-items: center; justify-content: center; }\n"
".preview-content svg { background: white; border-radius: 12px; max-width: 100%; }\n"
"textarea { width: 100%; height: 200px; background: #f8fafc; color: #1a3a6e; border: 1px solid #cbd5e1; padding: 12px; border-radius: 10px; font-family: monospace; }\n"
"button { background: #e2e8f0; border: none; color: #1a3a6e; padding: 10px 20px; border-radius: 10px; cursor: pointer; margin: 5px 5px 5px 0; }\n"
"button:hover { background: #cbd5e1; }\n"
"button.primary { background: #1e5ad9; color: white; }\n"
"button.success { background: #10b981; color: white; }\n"
"label { display: block; margin-top: 12px; margin-bottom: 5px; font-weight: 500; color: #1a3a6e; }\n"
"input { width: 100%; padding: 10px; background: #f8fafc; border: 1px solid #cbd5e1; border-radius: 10px; }\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<div class='container'>\n"
"<h1>Визуализатор графов</h1>\n"
"<div class='subtitle'>C++ приложение с веб-интерфейсом</div>\n"
"<div class='flex'>\n"
"<div class='panel'>\n"
"<div class='panel-header'>Входные данные</div>\n"
"<div class='panel-body'>\n"
"<textarea id='graph'>4\n1 2\n0 2 3\n0 1\n1</textarea>\n"
"<div>\n"
"<button onclick=\"loadPreset('simple')\">Простой граф</button>\n"
"<button onclick=\"loadPreset('directed')\">Ориентированный</button>\n"
"<button onclick=\"loadPreset('complete')\">Полный K5</button>\n"
"</div>\n"
"<label>Ширина:</label><input type='number' id='width' value='800'>\n"
"<label>Высота:</label><input type='number' id='height' value='600'>\n"
"<label>Радиус:</label><input type='number' id='radius' value='20'>\n"
"<label><input type='checkbox' id='directedCheckbox'> Ориентированный граф</label>\n"
"<button class='success' onclick='generate()'>Сгенерировать граф</button>\n"
"<button class='primary' onclick='downloadSVG()'>Сохранить SVG</button>\n"
"</div>\n"
"</div>\n"
"<div class='preview'>\n"
"<div class='preview-header'>Результат</div>\n"
"<div class='preview-content' id='result'>\n"
"<div style='color: #999; text-align: center;'>Нажмите \"Сгенерировать граф\"</div>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"<script>\n"
"const presets = {\n"
"    simple: '4\\n1 2\\n0 2 3\\n0 1\\n1',\n"
"    directed: '4\\n1\\n2\\n3\\n0',\n"
"    complete: '5\\n1 2 3 4\\n0 2 3 4\\n0 1 3 4\\n0 1 2 4\\n0 1 2 3'\n"
"};\n"
"let currentSVG = '';\n"
"function loadPreset(type) {\n"
"    document.getElementById('graph').value = presets[type];\n"
"    if(type === 'directed') document.getElementById('directedCheckbox').checked = true;\n"
"    generate();\n"
"}\n"
"async function generate() {\n"
"    const graphData = document.getElementById('graph').value;\n"
"    const width = document.getElementById('width').value;\n"
"    const height = document.getElementById('height').value;\n"
"    const radius = document.getElementById('radius').value;\n"
"    const directed = document.getElementById('directedCheckbox').checked ? 'true' : 'false';\n"
"    \n"
"    const formData = new URLSearchParams();\n"
"    formData.append('graph', graphData);\n"
"    formData.append('width', width);\n"
"    formData.append('height', height);\n"
"    formData.append('radius', radius);\n"
"    formData.append('directed', directed);\n"
"    \n"
"    try {\n"
"        const resp = await fetch('/generate', { method: 'POST', body: formData });\n"
"        if (resp.ok) {\n"
"            const svg = await resp.text();\n"
"            currentSVG = svg;\n"
"            document.getElementById('result').innerHTML = svg;\n"
"        } else {\n"
"            document.getElementById('result').innerHTML = '<div style=\"color: red;\">Ошибка генерации</div>';\n"
"        }\n"
"    } catch(e) {\n"
"        document.getElementById('result').innerHTML = '<div style=\"color: red;\">Ошибка: ' + e.message + '</div>';\n"
"    }\n"
"}\n"
"function downloadSVG() {\n"
"    if(currentSVG) {\n"
"        const blob = new Blob([currentSVG], {type: 'image/svg+xml'});\n"
"        const a = document.createElement('a');\n"
"        a.href = URL.createObjectURL(blob);\n"
"        a.download = 'graph.svg';\n"
"        a.click();\n"
"    }\n"
"}\n"
"loadPreset('simple');\n"
"</script>\n"
"</body>\n"
"</html>\n";
}

void handle_client(int client_socket) {
    char buffer[32768] = {0};
    read(client_socket, buffer, 32767);
    
    string request(buffer);
    string response;
    
    if (request.find("GET / ") != string::npos || request.find("GET / HTTP") != string::npos) {
        string html = get_html();
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: text/html; charset=utf-8\r\n";
        response += "Content-Length: " + to_string(html.size()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += html;
    }
    else if (request.find("GET /favicon.ico") != string::npos) {
        response = "HTTP/1.1 204 No Content\r\n\r\n";
    }
    else if (request.find("POST /generate") != string::npos) {
        size_t body_pos = request.find("\r\n\r\n");
        if (body_pos == string::npos) body_pos = request.find("\n\n");
        string body = request.substr(body_pos + 4);
        
        // Парсим параметры
        auto get_param = [&](const string& name) -> string {
            size_t pos = body.find(name + "=");
            if (pos == string::npos) return "";
            size_t end = body.find("&", pos);
            if (end == string::npos) end = body.length();
            string val = body.substr(pos + name.length() + 1, end - pos - name.length() - 1);
            // Декодируем URL
            string decoded;
            for (size_t i = 0; i < val.length(); i++) {
                if (val[i] == '+') decoded += ' ';
                else if (val[i] == '%' && i+2 < val.length()) {
                    char hex[3] = {val[i+1], val[i+2], 0};
                    decoded += (char)strtol(hex, nullptr, 16);
                    i += 2;
                } else {
                    decoded += val[i];
                }
            }
            return decoded;
        };
        
        string graph_data = get_param("graph");
        string width_str = get_param("width");
        string height_str = get_param("height");
        string radius_str = get_param("radius");
        string directed_str = get_param("directed");
        
        int width = width_str.empty() ? 800 : stoi(width_str);
        int height = height_str.empty() ? 600 : stoi(height_str);
        int radius = radius_str.empty() ? 20 : stoi(radius_str);
        bool directed = (directed_str == "true");
        
        // Удаляем возможные лишние символы в начале
        while (!graph_data.empty() && graph_data[0] == '\n') {
            graph_data.erase(0, 1);
        }
        
        // Сохраняем в файл
        string input_file = "/tmp/graph_input.txt";
        ofstream f(input_file);
        f << graph_data;
        f.close();
        
        string output_file = "/tmp/graph_output.svg";
        string cmd = "/Users/elizaveta/Desktop/graph/build/graph_vis -i " + input_file + 
                     " -o " + output_file + 
                     " -width " + to_string(width) +
                     " -height " + to_string(height) +
                     " -radius " + to_string(radius);
        if (directed) cmd += " -directed";
        
        system(cmd.c_str());
        
        ifstream svg_file(output_file);
        string svg((istreambuf_iterator<char>(svg_file)), istreambuf_iterator<char>());
        
        response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Type: image/svg+xml\r\n";
        response += "Content-Length: " + to_string(svg.size()) + "\r\n";
        response += "Connection: close\r\n\r\n";
        response += svg;
    }
    else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n404";
    }
    
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Socket failed" << endl;
        return 1;
    }
    
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8085);
    
    if (::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        cerr << "Bind failed" << endl;
        close(server_fd);
        return 1;
    }
    
    if (listen(server_fd, 10) < 0) {
        cerr << "Listen failed" << endl;
        close(server_fd);
        return 1;
    }
    
    cout << "\n========================================" << endl;
    cout << "   Визуализатор графов" << endl;
    cout << "   Сервер запущен на http://localhost:8085" << endl;
    cout << "========================================\n" << endl;
    
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket < 0) continue;
        thread t(handle_client, client_socket);
        t.detach();
    }
    
    return 0;
}
