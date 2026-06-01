#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <filesystem>
#include <random>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include <vector>

using namespace std;
namespace fs = std::filesystem;

string url_decode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '%' && i + 2 < str.length()) {
            char hex[3] = {str[i+1], str[i+2], 0};
            result += (char)strtol(hex, nullptr, 16);
            i += 2;
        } else if (str[i] == '+') {
            result += ' ';
        } else {
            result += str[i];
        }
    }
    return result;
}

string get_html() {
    return "<!DOCTYPE html>\n"
"<html>\n"
"<head><meta charset='UTF-8'><title>Визуализатор графов</title>\n"
"<style>\n"
"* { margin: 0; padding: 0; box-sizing: border-box; }\n"
"body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: linear-gradient(135deg, #f0f4f8 0%, #e2e8f0 100%); min-height: 100vh; padding: 20px; }\n"
".container { max-width: 1400px; margin: 0 auto; }\n"
"h1 { text-align: center; color: #1a3a6e; margin-bottom: 10px; font-weight: 600; }\n"
".subtitle { text-align: center; color: #4a6a8e; margin-bottom: 30px; font-size: 14px; }\n"
".flex { display: flex; gap: 25px; flex-wrap: wrap; }\n"
".panel { flex: 1; min-width: 300px; background: white; border-radius: 16px; box-shadow: 0 4px 20px rgba(0,0,0,0.08); overflow: hidden; }\n"
".panel-header { background: #1e5ad9; color: white; padding: 15px 20px; font-weight: 600; font-size: 18px; }\n"
".panel-body { padding: 20px; }\n"
".preview { flex: 2; min-width: 500px; background: white; border-radius: 16px; box-shadow: 0 4px 20px rgba(0,0,0,0.08); overflow: hidden; }\n"
".preview-header { background: #1e5ad9; color: white; padding: 15px 20px; font-weight: 600; font-size: 18px; }\n"
".preview-content { padding: 20px; background: #f8fafc; min-height: 550px; display: flex; align-items: center; justify-content: center; }\n"
".preview-content svg { background: white; border-radius: 12px; box-shadow: 0 2px 8px rgba(0,0,0,0.1); max-width: 100%; height: auto; }\n"
"textarea { width: 100%; height: 200px; background: #f8fafc; color: #1a3a6e; border: 1px solid #cbd5e1; padding: 12px; border-radius: 10px; font-family: 'Courier New', monospace; font-size: 13px; resize: vertical; }\n"
"textarea:focus { outline: none; border-color: #1e5ad9; box-shadow: 0 0 0 3px rgba(30,90,217,0.1); }\n"
"button { background: #e2e8f0; border: none; color: #1a3a6e; padding: 10px 20px; border-radius: 10px; cursor: pointer; font-size: 14px; font-weight: 500; margin: 5px 5px 5px 0; transition: all 0.2s; }\n"
"button:hover { background: #cbd5e1; transform: translateY(-1px); }\n"
"button.primary { background: #1e5ad9; color: white; }\n"
"button.primary:hover { background: #154aaf; }\n"
"button.success { background: #10b981; color: white; }\n"
"button.success:hover { background: #0e9f6e; }\n"
"button.danger { background: #ef4444; color: white; }\n"
"button.danger:hover { background: #dc2626; }\n"
"label { display: block; margin-top: 12px; margin-bottom: 5px; font-size: 13px; font-weight: 500; color: #1a3a6e; }\n"
"input, select { width: 100%; padding: 10px; background: #f8fafc; border: 1px solid #cbd5e1; border-radius: 10px; font-size: 14px; }\n"
"input:focus, select:focus { outline: none; border-color: #1e5ad9; box-shadow: 0 0 0 3px rgba(30,90,217,0.1); }\n"
".preset-buttons { display: flex; flex-wrap: wrap; gap: 8px; margin: 15px 0; }\n"
"hr { margin: 20px 0; border: none; border-top: 1px solid #e2e8f0; }\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<div class='container'>\n"
"<h1>📊 Визуализатор графов</h1>\n"
"<div class='subtitle'>C++ приложение с веб-интерфейсом | Круговая компоновка вершин</div>\n"
"<div class='flex'>\n"
"<div class='panel'>\n"
"<div class='panel-header'>📁 Входные данные</div>\n"
"<div class='panel-body'>\n"
"<label>Список смежности (первая строка — число вершин):</label>\n"
"<textarea id='graph'>4\n1 2\n0 2 3\n0 1\n1</textarea>\n"
"<div class='preset-buttons'>\n"
"<button onclick=\"loadPreset('simple')\">🔹 Простой граф</button>\n"
"<button onclick=\"loadPreset('directed')\">🔸 Ориентированный граф</button>\n"
"<button onclick=\"loadPreset('complete')\">⭐ Полный граф K5</button>\n"
"<button onclick=\"loadPreset('bipartite')\">🔷 Двудольный K3,3</button>\n"
"</div>\n"
"<hr>\n"
"<div class='panel-header' style='background: #4a6a8e; margin: -20px -20px 15px -20px;'>⚙️ Настройки</div>\n"
"<label>📐 Ширина:</label>\n"
"<input type='number' id='width' value='800' step='50'>\n"
"<label>📏 Высота:</label>\n"
"<input type='number' id='height' value='600' step='50'>\n"
"<label>🔘 Радиус вершин:</label>\n"
"<input type='number' id='radius' value='20' step='5'>\n"
"<label>🔄 Направление рёбер:</label>\n"
"<select id='directed'>\n"
"<option value='false'>Неориентированный</option>\n"
"<option value='true'>Ориентированный (со стрелками)</option>\n"
"</select>\n"
"<div style='margin-top: 20px;'>\n"
"<button class='success' onclick='generate()'>🎨 Сгенерировать граф</button>\n"
"<button class='primary' onclick='downloadSVG()'>💾 Сохранить SVG</button>\n"
"<button class='danger' onclick='clearGraph()'>🗑 Очистить</button>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"<div class='preview'>\n"
"<div class='preview-header'>🖼 Результат</div>\n"
"<div class='preview-content' id='result'>\n"
"<svg width='500' height='400' viewBox='0 0 500 400'>\n"
"<rect width='100%' height='100%' fill='#f8fafc'/>\n"
"<text x='250' y='200' text-anchor='middle' fill='#94a3b8'>Нажмите \"Сгенерировать граф\"</text>\n"
"</svg>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"</div>\n"
"<script>\n"
"const presets = {\n"
"    simple: '4\\n1 2\\n0 2 3\\n0 1\\n1',\n"
"    directed: '4\\n1\\n2\\n3\\n0',\n"
"    complete: '5\\n1 2 3 4\\n0 2 3 4\\n0 1 3 4\\n0 1 2 4\\n0 1 2 3',\n"
"    bipartite: '6\\n3 4 5\\n3 4 5\\n3 4 5\\n0 1 2\\n0 1 2\\n0 1 2'\n"
"};\n"
"let currentSVG = '';\n"
"function loadPreset(type) {\n"
"    document.getElementById('graph').value = presets[type];\n"
"    if(type === 'directed') document.getElementById('directed').value = 'true';\n"
"    generate();\n"
"}\n"
"async function generate() {\n"
"    const params = new URLSearchParams({\n"
"        graph: document.getElementById('graph').value,\n"
"        width: document.getElementById('width').value,\n"
"        height: document.getElementById('height').value,\n"
"        radius: document.getElementById('radius').value,\n"
"        directed: document.getElementById('directed').value === 'true'\n"
"    });\n"
"    try {\n"
"        const resp = await fetch('/generate', { method: 'POST', body: params });\n"
"        if (resp.ok) {\n"
"            const svg = await resp.text();\n"
"            currentSVG = svg;\n"
"            document.getElementById('result').innerHTML = svg;\n"
"        } else {\n"
"            document.getElementById('result').innerHTML = '<div style=\"color: red; padding: 20px;\">Ошибка</div>';\n"
"        }\n"
"    } catch(e) {\n"
"        document.getElementById('result').innerHTML = '<div style=\"color: red;\">Ошибка: ' + e.message + '</div>';\n"
"    }\n"
"}\n"
"function clearGraph() {\n"
"    document.getElementById('graph').value = '';\n"
"    document.getElementById('result').innerHTML = '<svg width=\"500\" height=\"400\"><rect width=\"100%\" height=\"100%\" fill=\"#f8fafc\"/><text x=\"250\" y=\"200\" text-anchor=\"middle\" fill=\"#94a3b8\">Очищено</text></svg>';\n"
"    currentSVG = '';\n"
"}\n"
"function downloadSVG() {\n"
"    if(currentSVG) {\n"
"        const blob = new Blob([currentSVG], {type: 'image/svg+xml'});\n"
"        const a = document.createElement('a');\n"
"        a.href = URL.createObjectURL(blob);\n"
"        a.download = 'graph.svg';\n"
"        a.click();\n"
"    } else {\n"
"        alert('Сначала сгенерируйте граф');\n"
"    }\n"
"}\n"
"loadPreset('simple');\n"
"</script>\n"
"</body>\n"
"</html>\n";
}

string handle_generate(const string& body, string& content_type) {
    string graph_data;
    int width = 800, height = 600, radius = 20;
    bool directed = false;
    
    auto extract = [&](const string& name, string& value) {
        size_t pos = body.find(name + "=");
        if (pos != string::npos) {
            size_t end = body.find("&", pos);
            if (end == string::npos) end = body.length();
            value = url_decode(body.substr(pos + name.length() + 1, end - pos - name.length() - 1));
        }
    };
    
    string width_str, height_str, radius_str, directed_str;
    extract("graph", graph_data);
    extract("width", width_str);
    extract("height", height_str);
    extract("radius", radius_str);
    extract("directed", directed_str);
    
    if (!width_str.empty()) width = stoi(width_str);
    if (!height_str.empty()) height = stoi(height_str);
    if (!radius_str.empty()) radius = stoi(radius_str);
    directed = (directed_str == "true");
    
    // Сохраняем во временный файл
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
    
    content_type = "image/svg+xml";
    
    if (svg.empty()) {
        svg = "<svg width='500' height='300'><rect width='100%' height='100%' fill='white'/><text x='250' y='150' text-anchor='middle' fill='red'>Ошибка</text></svg>";
    }
    
    return svg;
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
    else if (request.find("POST /generate") != string::npos) {
        size_t body_pos = request.find("\r\n\r\n");
        if (body_pos == string::npos) body_pos = request.find("\n\n");
        if (body_pos != string::npos) {
            string body = request.substr(body_pos + 4);
            string content_type;
            string content = handle_generate(body, content_type);
            
            response = "HTTP/1.1 200 OK\r\n";
            response += "Content-Type: " + content_type + "\r\n";
            response += "Content-Length: " + to_string(content.size()) + "\r\n";
            response += "Connection: close\r\n\r\n";
            response += content;
        } else {
            response = "HTTP/1.1 400 Bad Request\r\n\r\n";
        }
    }
    else {
        response = "HTTP/1.1 404 Not Found\r\n\r\n404";
    }
    
    send(client_socket, response.c_str(), response.size(), 0);
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(9090);
    
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 10);
    
    cout << "\n========================================" << endl;
    cout << "   Визуализатор графов" << endl;
    cout << "   Сервер запущен на http://localhost:9090" << endl;
    cout << "========================================\n" << endl;
    
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }
    
    close(server_fd);
    return 0;
}
