"""Compare the maintained loop with the historical baseline using C++ stubs.

Run: python3 tests/check_following.py
Requires c++ and Git history containing the original baseline commit.
Exercises loop control flow only, not hardware drivers or sensor functions.
"""
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
path = 'firmware/follow_me_bot/follow_me_bot.ino'
baseline = subprocess.check_output(['git', 'show', 'a1bb24bd26e80d29075c4a09a114fdf844fa3631:full_code.ino'], cwd=root, text=True)
current = (root / path).read_text()
harness = r'''
#include <iostream>
#include <vector>
#include <cstdlib>
std::vector<long> distances, temperatures;
unsigned di=0, ti=0;
int direction, temp, heat_direction;
long distance;
int left_motor_speed=10, right_motor_speed=9;
void emit(const char* name, long value=0) { std::cout << name << ":" << value << "\n"; }
long distance_read() { if(di>=distances.size()) std::exit(2); long v=distances[di++]; emit("distance",v); return v; }
long temp_read() { if(ti>=temperatures.size()) std::exit(3); long v=temperatures[ti++]; emit("temp",v); return v; }
long heat_search() { emit("search",direction); return direction; }
void go_forward() { emit("forward"); }
void go_backward(int n) { emit("backward",n); }
void turn_left(int n) { emit("left",n); }
void turn_right(int n) { emit("right",n); }
void delay(int n) { emit("delay",n); }
void analogWrite(int pin, int n) { emit("pin",pin); emit("pwm",n); }
LOOP
int main(int argc,char** argv) {
  direction=std::atoi(argv[1]);
  int mode=std::atoi(argv[2]);
  distances={100, 45, 44, 30, 45};
  temperatures={75,75,75,75};
  if(mode==1) temperatures={75,74};
  if(mode==2) temperatures={75,75,74};
  if(mode==3) { distances={44}; temperatures={74}; }
  if(mode==4) { distances={100,100,45,44,45}; temperatures={74,75,75,75,74}; }
  loop();
}
'''
with tempfile.TemporaryDirectory() as tmp:
    executables=[]
    for name, source in [('before', baseline), ('after', current)]:
        loop=source[source.index('void loop()'):source.index('void go_forward()')]
        cpp=Path(tmp)/f'{name}.cpp'
        cpp.write_text(harness.replace('LOOP',loop))
        executable=Path(tmp)/name
        subprocess.run(['c++','-std=c++11','-Wall','-Wextra',str(cpp),'-o',str(executable)],check=True,capture_output=True)
        executables.append(executable)
    count=0
    for direction in range(4):
        for mode in range(5):
            traces=[subprocess.check_output([str(exe),str(direction),str(mode)]) for exe in executables]
            assert traces[0]==traces[1], (direction,mode,traces)
            count+=1
    print(f'PASS: {count} baseline/refactor loop traces identical (sensor reads, motor commands, waits)')
