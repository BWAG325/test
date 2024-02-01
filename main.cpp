#include <iostream>
#include <vector>
#include <memory>

class Robot {
public:
    int HP = 0, HP_max = 0;
    int MP = 0, MP_max = 0;
    int team = 0;
    int code = 0;
    int level = 1;
    int time = 0;
    int type = 0;
};

class Engineering : virtual public Robot {
public:
    Engineering(int team, int code) {
        this->team = team;
        this->code = code;
        HP_max = 300;
        HP = 300;
        type = 1;
    }
};

class Infantry : virtual public Robot {
public:
    Infantry(int team, int code) {
        this->team = team;
        this->code = code;
        HP_max = 100;
        HP = 100;
        MP_max = 100;
        MP = 100;
        type = 0;
    }
};

class Manage {
public:
    std::vector<std::shared_ptr<Robot>> Live;
    std::vector<std::shared_ptr<Robot>> Dead;
    int findPlace = 0;

    bool check(int ti, int te, int co) {
        bool find = false;
        int i = 0;
        std::vector<int> v;
        for (auto &p: Live) {
            if (p->team == te && p->code == co && p->HP > 0) { //是否找到指定 bot
                find = true;
                findPlace = i;
            }
            if (p->type == 0 && p->MP < p->MP_max) { //热量检查
                int timeCount = ti - p->time;
                if (p->MP < 0) {
                    if (p->MP + timeCount >= 0) p->HP += p->MP;
                    else p->HP -= timeCount;
                }
                p->MP += timeCount;
                if (p->MP > p->MP_max) p->MP = p->MP_max;
                p->time = ti;
            }
            if (p->HP <= 0) { //血量检查
                std::cout << "D " << p->team << " " << p->code << std::endl;
                Dead.push_back(Live[i]);
                v.push_back(i);
                i--;
            }
            i++;
        }
        while (!v.empty()) {
            int k = v.back();
            Live.erase(Live.begin() + k);
            v.pop_back();
        }
        return find;
    }

    bool checkDead(int te, int co) {
        int i = 0, n = 0;
        bool find = false;
        for (auto &p: Dead) {
            if (p->team == te && p->code == co) { //复活机器人
                find = true;
                n = i;
                Live.push_back(Dead[i]);
                p->HP = p->HP_max;
                p->MP = p->MP_max;
            }
            i++;
        }
        if (find) Dead.erase(Dead.begin() + n);
        return find;
    }

    void Add(int ti, int te, int co, int x) {
        if (!check(ti, te, co) && !checkDead(te, co)) {
            if (x == 0) {
                std::shared_ptr<Robot> ptr = std::make_shared<Infantry>(te, co);
                Live.push_back(ptr);
            } else if (x == 1) {
                std::shared_ptr<Robot> ptr = std::make_shared<Engineering>(te, co);
                Live.push_back(ptr);
            }
        }
    }

    void Fight(int ti, int te, int co, int x) {
        if (check(ti, te, co)) {
            auto p = Live[findPlace];
            p->HP -= x;
            if (p->HP < 0) {
                std::cout << "D " << p->team << " " << p->code << std::endl;
                Dead.push_back(Live[findPlace]);
                Live.erase(Live.begin() + findPlace);
            }
        }
    }

    void MP_Count(int ti, int te, int co, int x) {
        if (check(ti, te, co)) {
            auto p = Live[findPlace];
            p->MP -= x;
            if (p->MP > p->MP_max) p->MP = p->MP_max;
        }
    }

    void Upgrade(int ti, int te, int co, int x) {
        if (check(ti, te, co)) {
            auto p = Live[findPlace];
            if (p->type == 0 && p->level < x) {
                if (x == 2) {
                    p->HP_max = 150;
                    p->MP_max = 200;
                    p->HP = p->HP_max;
                    p->MP += 100;
                }
                if (x == 3) {
                    p->HP_max = 250;
                    p->MP += 300 - p->MP_max;
                    p->MP_max = 300;
                    p->HP = p->HP_max;
                }
            }
        }
    }
};

int main() {
    char Command;
    int count, T, team, code, x;
    Manage M;
    std::cin >> count;
    for (int i = 0; i < count; i++) {
        std::cin >> T >> Command >> team >> code >> x;
        switch (Command) {
            case 'A':
                M.Add(T, team, code, x);
                break;
            case 'F':
                M.Fight(T, team, code, x);
                break;
            case 'H':
                M.MP_Count(T, team, code, x);
                break;
            case 'U':
                M.Upgrade(T, team, code, x);
                break;
            default:
                break;
        }
    }
    return 0;
}
