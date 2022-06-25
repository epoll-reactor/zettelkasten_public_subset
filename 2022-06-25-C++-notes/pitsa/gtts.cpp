#include "./gtts.hpp"

namespace gtts {

GoogleTTS::GoogleTTS(string msg, string lang, string speed) {
    _speed += speed;
    _lang += lang;
    if (msg.length() > 200) {
        std::vector<string> msgs = this->split(msg);
        this->parse(msgs);
    } else {
        _text = msg;
        this->parse();
    }
}

std::vector<string> GoogleTTS::split(string& msg) {
    std::vector<string> vec;
    std::istringstream iss(msg);
    std::vector<string> words(std::istream_iterator<string>{iss},
            std::istream_iterator<string>());
    string part = "";
    for (string s : words) {
        if(part.size()-1 + s.size() <= 200) {
            part += s + " ";
        } else {
            vec.push_back(part);
            part = s + " ";
        }
    }
    if(part != "") vec.push_back(part);

    return vec;
}

void GoogleTTS::unite() {
    system(_cat.c_str());
    system(_rm.c_str());
}

void GoogleTTS::execute() {
    if (_cmds.size() == 1) {
        std::system(_cmds[0].c_str());
    } else {
        for(string cmd : _cmds) {
            std::system(cmd.c_str());
        }
        this->unite();
    }
    _mpv += _speed + _play;
    std::system(_mpv.c_str());
}

void GoogleTTS::replace(string& text) {
    size_t start_pos = 0;
    while ((start_pos = text.find(" ", start_pos)) != string::npos) {
       text.replace(start_pos, 1, "%20");
       start_pos += 3; // Handles case where 'to' is a substring of 'from'
    }
}

void GoogleTTS::parse() {
    this->replace(_text);
    string cmd = _curl + _text + _lang + _client + "-H" + _ref + "-H";
    cmd += _agent + _out + " 2>/dev/null";
    _cmds.push_back(cmd);
}

void GoogleTTS::parse(std::vector<string>& vec) {
    string cmd = "";
    int i = 0;
    for (string msg : vec) {
        this->replace(msg);
        cmd = _curl + msg + _lang + _client + "-H" + _ref + "-H";
        cmd += _agent + _outv + std::to_string(i) + ".mp3" + " 2>/dev/null";
        _cmds.push_back(cmd);
        i++;
    }
}
} //namespace gtts
