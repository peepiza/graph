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
"body { font-family: Arial; background: #f0f4f8; padding: 20px; }\n"
"h1 { color: #1a3a6e; text-align: center; }\n"
".container { max-width: 1200px; margin: 0 auto; }\n"
".flex { display: flex; gap: 20px; }\n"
".panel { background: white; border-radius: 10px; padding: 20px; flex: 1; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n"
".preview { background: white; border-radius: 10px; padding: 20px; flex: 2; box-shadow: 0 2px 10px rgba(0,0,0,0.1); min-height: 500px; }\n"
"textarea { width: 100%; height: 150px; font-family: monospace; }\n"
"button { background: #1e5ad9; color: white; border: none; padding: 10px 20px; margin: 5px; border-radius: 5px; cursor: pointer; }\n"
"button:hover { background: #154aaf; }\n"
"input { width: 100%; padding: 8px; margin: 5px 0; }\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<div class='container'>\n"
"<h1>Визуализатор графов</h1>\n"
"<div class='flex'>\n"
"<div class='panel'>\n"
"<h3>Входные данные</h3>\n"
"<textarea id='graph'>4\n1 2\n0 2 3\n0 1\n1</textarea>\n"
"<div>\n"
"<button onclick='setPreset(\"simple\")'>Простой</button>\n"
"<button onclick='setPreset(\"directed\")'>Ориентированный</button>\n"
"<button onclick='setPreset(\"complete\")'>Полный K5</button>\n"
"</div>\n"
"<h3>Настройки</h3>\n"
"<label>Ширина:</label><input type='number' id='width' value='800'>\n"
"<label>Высота:</label><input type='number' id='height' value='600'>\n"
"<label>Радиус:</label><input type='number' id='radius' value='20'>\n"
"<label><input type='checkbox' id='directed'> Ориентированный</label>\n"
"<button onclick='generate()'>Сгенерировать</button>\n"
"<button onclick='downloadSVG()'>Скачать SVG</button>\n"
"</div>\n"
"<div class='preview' id='result'>Нажмите Сгенерировать</div>\n"
"</div>\n"
"</div>\n"
"<script>\n"
"function setPreset(type) {\n"
"    const presets = {\n"
"        simple: '4\\n1 2\\n0 2 3\\n0 1\\n1',\n"
"        directed: '4\\n1\\n2\\n3\\n0',\n"
"        complete: '5\\n1 2 3 4\\n0 2 3 4\\n0 1 3 4\\n0 1 2 4\\n0 1 2 3'\n"
"    };\n"
"    document.getElementById('graph').value = presets[type];\n"
"    if(type === 'directed') document.getElementById('directed').checked = true;\n"
"    generate();\n"
"}\n"
"let currentSVG = '';\n"
"async function generate() {\n"
"    const formData = new URLSearchParams();\n"
"    formData.append('graph', document.getElementById('graph').value);\n"
"    formData.append('width', document.getElementById('width').value);\n"
"    formData.append('height', document.getElementById('height').value);\n"
"    formData.append('radius', document.getElementById('radius').value);\n"
"    formData.append('directed', document.getElementById('directed').checked);\n"
"    const resp = await fetch('/generate', { method: 'POST', body: formData });\n"
"    const svg = await resp.text();\n"
"    currentSVG = svg;\n"
"    document.getElementById('result').innerHTML = svg;\n"
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
"setPreset('simple');\n"
"</script>\n"
"</body>\n"
"</html>\n";
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8085);
    
    if (::bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Bind failed" << endl;
        return 1;
    }
    listen(server_fd, 10);
    
    cout << "\n========================================" << endl;
    cout << "   Визуализатор графов" << endl;
    cout << "   Сервер запущен на http://localhost:8085" << endl;
    cout << "========================================\n" << endl;
    
    while (true) {
        int client = accept(server_fd, nullptr, nullptr);
        char buf[65536] = {0};
        read(client, buf, 65535);
        string req(buf);
        
        string resp;
        if (req.find("GET / ") != string::npos || req.find("GET / HTTP") != string::npos) {
            string html = get_html();
            resp = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + to_string(html.size()) + "\r\n\r\n" + html;
        }
        else if (req.find("POST /generate") != string::npos) {
            size_t pos = req.find("\r\n\r\n");
            string body = req.substr(pos + 4);
            
            string graph_data;
            int width = 800, height = 600, radius = 20;
            bool directed = false;
            
            auto extract = [&](const string& key) {
                size_t p = body.find(key + "=");
                if (p != string::npos) {
                    size_t end = body.find("&", p);
                    if (end == string::npos) end = body.length();
                    string val = body.substr(p + key.length() + 1, end - p - key.length() - 1);
                    string dec;
                    for (size_t i = 0; i < val.size(); i++) {
                        if (val[i] == '+') dec += ' ';
                        else dec += val[i];
                    }
                    return dec;
                }
                return string("");
            };
            
            graph_data = extract("graph");
            string w = extract("width");
            string h = extract("height");
            string r = extract("radius");
            string d = extract("directed");
            
            if (!w.empty()) width = stoi(w);
            if (!h.empty()) height = stoi(h);
            if (!r.empty()) radius = stoi(r);
            directed = (d == "true");
            
            // ОТЛАДКА: выводим полученные данные
            cout << "=== DEBUG ===" << endl;
            cout << "Raw graph data: [" << graph_data << "]" << endl;
            cout << "Width: " << width << ", Height: " << height << ", Radius: " << radius << ", Directed: " << directed << endl;
            
            // Сохраняем в файл
            string input_file = "/tmp/graph_in.txt";
            ofstream f(input_file);
            f << graph_data;
            f.close();
            
            // Проверяем содержимое файла
            ifstream check(input_file);
            string line;
            cout << "File content:" << endl;
            while (getline(check, line)) {
                cout << "  [" << line << "]" << endl;
            }
            check.close();
            cout << "=============" << endl;
            
            string output_file = "/tmp/graph_out.svg";
            string cmd = "/Users/elizaveta/Desktop/graph/build/graph_vis -i " + input_file + " -o " + output_file +
                         " -width " + to_string(width) + " -height " + to_string(height) + " -radius " + to_string(radius);
            if (directed) cmd += " -directed";
            system(cmd.c_str());
            
            ifstream svgf(output_file);
            string svg((istreambuf_iterator<char>(svgf)), istreambuf_iterator<char>());
            
            resp = "HTTP/1.1 200 OK\r\nContent-Type: image/svg+xml\r\nContent-Length: " + to_string(svg.size()) + "\r\n\r\n" + svg;
        }
        else {
            resp = "HTTP/1.1 404 Not Found\r\n\r\n404";
        }
        
        send(client, resp.c_str(), resp.size(), 0);
        close(client);
    }
    return 0;
}
