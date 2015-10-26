with Text_IO;
with Ada.Containers.Hashed_Maps; use Ada.Containers;
with Ada.Strings.Hash;

procedure RomanNumbers is
  roman: String(1..50);

  function Character_Hashed (id: Character) return Hash_Type is
    str: String(1..1);
  begin
    str(1) := id;
    return Ada.Strings.Hash(str);
  end Character_Hashed;

  package Romans is new Ada.Containers.Hashed_Maps
       (Key_Type => Character,
        Element_Type => Integer,
        Hash => Character_Hashed,
        Equivalent_Keys => "=");

  roman_map: Romans.Map;
begin
  roman_map.insert('V', 5);
  roman_map.insert('I', 1);
  roman_map.insert('X', 10);
  roman_map.insert('L', 50);
  roman_map.insert('C', 100);
  roman_map.insert('D', 500);
  roman_map.insert('M', 1000);
end RomanNumbers;
