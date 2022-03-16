// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90

#include "includes/neogeo.h"

#if 0
DRIVER_INIT_MEMBER( neogeo_state, mslugx )
{
	mslugx_install_protection();
}

ROM_START( mslug )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201.p1", 0x100000, 0x100000, CRC(08d8daa5) SHA1(b888993dbb7e9f0a28a01d7d2e1da00ef9cf6f38) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END
#endif

/*****************
 Metal Slug Hacks
*******************/

ROM_START( mslughacks01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks01.p1", 0x100000, 0x100000, CRC(61e1ba6e) SHA1(aafc5b39a61d330b95f29c663900fbc02b01bd1d) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks02.p1", 0x100000, 0x100000, CRC(2750fe1b) SHA1(0de9e1cf728cdf21fb6ed243637576644e786fc8) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks03.p1", 0x100000, 0x100000, CRC(2b3d433f) SHA1(94a1a88bff5f1b6bdf8d8a0406004982ef9d7b0c) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks04 ) //mslugdg
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks04.p1", 0x100000, 0x100000, CRC(0a739521) SHA1(74e637a6a77140f8ce1128cb8f456ecae0a7a7ef) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks05 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks05.p1", 0x100000, 0x100000, CRC(582b4d77) SHA1(9de833c621d3b26153e3b22c86d6137beb58e5e4) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks06 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks06.p1", 0x100000, 0x100000, CRC(4d6af5bd) SHA1(2c8d782dba605d9148d8e519c803be2f14145642) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks07 ) //mslugunity
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks07.p1", 0x100000, 0x100000, CRC(a3186dfd) SHA1(9241e8bf40b878f2372d8da9f008c8895a87394b) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks08 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks08.p1", 0x100000, 0x100000, CRC(47ae2445) SHA1(615aeb5ed8f7e0197ed599b3f20eaed88ab1086d) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

ROM_START( mslughacks09 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks09.p1", 0x100000, 0x100000, CRC(09888e87) SHA1(d61adb16503c30b3290ea41b3955607b7ebba70f) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

/********
 Bootleg
**********/

ROM_START( msboot )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201.p1", 0x100000, 0x100000, CRC(08d8daa5) SHA1(b888993dbb7e9f0a28a01d7d2e1da00ef9cf6f38) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201boot.v1", 0x000000, 0x200000, CRC(2C05FD7C) SHA1(4f28bc799da0e27b2f3cde364d8c9822468abb8f) )
	ROM_LOAD( "201boot.v2", 0x200000, 0x200000, CRC(752DE272) SHA1(1e9ffb296a1089e15b23455841ad91e8a565ba62) )
	ROM_LOAD( "201boot.v3", 0x400000, 0x200000, CRC(AA48FAA6) SHA1(4695b9dafdc13997410cbd7954c2a7f1b5e4649d) )
	ROM_LOAD( "201boot.v4", 0x600000, 0x200000, CRC(4264444B) SHA1(7c248e8e559c67d6e8474ec6ce5cf2e3e41235e4) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

 /**************
 Only exclusive
*****************/
 /******************************
 Revised Remix Standard Edition
*********************************/

ROM_START( mslugla01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_la01.p1", 0x100000, 0x100000, CRC(70ca457c) SHA1(a5ae3651f21e5061762f0f21ff5659732a3de9f4) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	ROM_REGION16_BE( 0x20000, "mainbios", 0 )
	ROM_LOAD16_WORD_SWAP( "uni-bios_4_0.rom", 0x00000, 0x20000, CRC(a7aab458) SHA1(938a0bda7d9a357240718c2cec319878d36b8f72) )

	ROM_REGION( 0x20000, "audiobios", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_REGION( 0x30000, "audiocpu", 0 )
	ROM_LOAD( "201.m1", 0x00000, 0x20000, CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )
	ROM_RELOAD(     0x10000, 0x20000 )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

/*************************
 Extreme MegaMods Edition
***************************/

ROM_START( mslugrmxe0s01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_mxe1s01.p1", 0x100000, 0x100000, CRC(07829c7d) SHA1(d90be05a617d9c3e3f966ca2d5b22c216e447f28) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	ROM_REGION16_BE( 0x20000, "mainbios", 0 )
	ROM_LOAD16_WORD_SWAP( "uni-bios_4_0.rom", 0x00000, 0x20000, CRC(a7aab458) SHA1(938a0bda7d9a357240718c2cec319878d36b8f72) )

	ROM_REGION( 0x20000, "audiobios", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_REGION( 0x30000, "audiocpu", 0 )
	ROM_LOAD( "201.m1", 0x00000, 0x20000, CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )
	ROM_RELOAD(     0x10000, 0x20000 )

	ROM_REGION( 0x800000, "ymsnd", 0 )
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Metal Slug
HACK( 2016, mslughacks01,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "DDJ",           "Metal Slug (Change Weapon 2016-03-26)", MACHINE_SUPPORTS_SAVE )
HACK( 2017, mslughacks02,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "DDJ",           "Metal Slug (Jump In Mid Air 2017-08-24)", MACHINE_SUPPORTS_SAVE )
HACK( 2016, mslughacks03,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "DDJ",           "Metal Slug (War Chariot 2016-03-26)", MACHINE_SUPPORTS_SAVE )
HACK( 2018, mslughacks04,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "hack",          "Metal Slug (Multi-Function Version 2018-04-30)", MACHINE_SUPPORTS_SAVE )
HACK( 2021, mslughacks05,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "I love Nimes",  "Metal Slug (Burst Enhanced Edition 2021-09-19)", MACHINE_SUPPORTS_SAVE )
HACK( 2018, mslughacks06,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "hack",          "Metal Slug (Easy Mode Difficulty 2018-11-26)", MACHINE_SUPPORTS_SAVE )
HACK( 2021, mslughacks07,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "KofKill, Wang Hua, Czk", "Metal Slug (Unity Time 2021-02-05)", MACHINE_SUPPORTS_SAVE )
HACK( 2016, mslughacks08,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "DDJ",           "Metal Slug (Stage Select 2016-10-17)", MACHINE_SUPPORTS_SAVE )
HACK( 2021, mslughacks09,      mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "hack",          "Metal Slug (Random Items Explosives 2021-06-08)", MACHINE_SUPPORTS_SAVE )

// Bootleg
HACK( 2005, msboot,            mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "bootleg",       "Metal Slug (Bootleg)", MACHINE_SUPPORTS_SAVE )

 /**************
 Only exclusive
*****************/
/*********************************************************************************
* This game sector is exclusive, its modifications that load have been customized
  241.p1 and 241.p2 files. Therefore, a unique and distinctive game will be displayed 
  from the rest, we have been working to Optimize performance and avoid any type of crash 
  that occurs during the game. This it just means there are some details of the roms that 
  still need to be improved, things to add and things that could later be updated or 
  permanently removed if some kind a problem is detected that cannot be solved ...
********************************************************************************************/

// Revised Remix Standard Edition
// Universe Bios v4.0 boot system
HACK( 2021, mslugla01,         mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                 "Metal Slug (Multi-Function Version 2018-04-30)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )

// Extreme MegaMods Edition
HACK( 2021, mslugrmxe0s01,     mslug,    neogeo_noslot, neogeo, neogeo_state,         neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, DDJ And I love Nimes)",            "Metal Slug (Multi-Function Version 2018-04-30)(Revised Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )