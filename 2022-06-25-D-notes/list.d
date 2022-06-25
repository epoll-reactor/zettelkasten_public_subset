import std.stdio;
import std.container : DList;

void main() {
  auto list = DList!int(1, 2, 3);
  writeln(list);
}
