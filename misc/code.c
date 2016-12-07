
Vec3 positions[] = [vec3(0, 0, 0), ...];

reg("animal_pos", *positions, Vec3);
reg("animal_hp", *hp, Int32);

Vec3 move_animals(Vec3 pos, Int32 hp) {
  return hp > 0 ? vec3_mul(pos, 2.0f) : pos;
}

reg("animal_tick", Mutator(move_animals, "animal_pos", "animal_hp"));

trigger(Every(0.033), "animal_tick");
trigger(Keyboard(), "player_move");
