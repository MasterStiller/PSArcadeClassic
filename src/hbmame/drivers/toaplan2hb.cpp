// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90
#include "../mame/drivers/toaplan2.cpp"

/********************
  Hack And Homebrow
***********************/

ROM_START( batsugunhc01 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "tp030_01_hc01.bin", 0x000000, 0x080000, CRC(63d28fd3) SHA1(aa3e111d596f408b735e4f79ab654eea24bd6f0c) )

	ROM_REGION( 0x400000, "gp9001_0", 0 )
	ROM_LOAD( "tp030_3l.bin", 0x000000, 0x100000, CRC(3024b793) SHA1(e161db940f069279356fca2c5bf2753f07773705) )
	ROM_LOAD( "tp030_3h.bin", 0x100000, 0x100000, CRC(ed75730b) SHA1(341f0f728144a049486d996c9bb14078578c6879) )
	ROM_LOAD( "tp030_4l.bin", 0x200000, 0x100000, CRC(fedb9861) SHA1(4b0917056bd359b21935358c6bcc729262be6417) )
	ROM_LOAD( "tp030_4h.bin", 0x300000, 0x100000, CRC(d482948b) SHA1(31be7dc5cff072403b783bf203b9805ffcad7284) )

	ROM_REGION( 0x200000, "gp9001_1", 0 )
	ROM_LOAD( "tp030_5.bin",  0x000000, 0x100000, CRC(bcf5ba05) SHA1(40f98888a29cdd30cda5dfb60fdc667c69b0fdb0) )
	ROM_LOAD( "tp030_6.bin",  0x100000, 0x100000, CRC(0666fecd) SHA1(aa8f921fc51590b5b05bbe0b0ad0cce5ff359c64) )

	ROM_REGION( 0x40000, "oki1", 0 )
	ROM_LOAD( "tp030_2.bin", 0x00000, 0x40000, CRC(276146f5) SHA1(bf11d1f6782cefcad77d52af4f7e6054a8f93440) )

	ROM_REGION( 0x1000, "plds", 0 )         /* Logic for mixing output of both GP9001 GFX controllers */
	ROM_LOAD( "tp030_u19_gal16v8b-15.bin", 0x0000, 0x117, CRC(f71669e8) SHA1(ec1fbe04605fee864af4b01f001af227938c9f21) )
//  ROM_LOAD( "tp030_u19_gal16v8b-15.jed", 0x0000, 0x991, CRC(31be54a2) SHA1(06278942a9a2ea858c0352b2ef5a65bf329b7b82) )
ROM_END

ROM_START( bgareggahc01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "prg0_hc01.bin", 0x000000, 0x080000, CRC(6f4af466) SHA1(fb77f3b86c0b25941ae0efe70ca779d06f66f02b) )
	ROM_LOAD16_BYTE( "prg1_hc01.bin", 0x000001, 0x080000, CRC(b4dc9a48) SHA1(9310a8c1de2a64e5597206a9974f25351d7825c2) )

	ROM_REGION( 0x20000, "audiocpu", 0 )
	ROM_LOAD( "snd.bin", 0x00000, 0x20000, CRC(68632952) SHA1(fb834db83157948e2b420b6051102a9c6ac3969b) )

	ROM_REGION( 0x800000, "gp9001_0", 0 )
	ROM_LOAD( "rom4.bin",  0x000000, 0x200000, CRC(b333d81f) SHA1(5481465f1304334fd55798be2f44324c57c2dbcb) )
	ROM_LOAD( "rom3.bin",  0x200000, 0x200000, CRC(51b9ebfb) SHA1(30e0c326f5175aa436df8dba08f6f4e08130b92f) )
	ROM_LOAD( "rom2.bin",  0x400000, 0x200000, CRC(b330e5e2) SHA1(5d48e9d56f99d093b6390e0af1609fd796df2d35) )
	ROM_LOAD( "rom1.bin",  0x600000, 0x200000, CRC(7eafdd70) SHA1(7c8da8e86c3f9491719b1d7d5d285568d7614f38) )

	ROM_REGION( 0x010000, "text", 0 )
	ROM_LOAD( "text.u81", 0x00000, 0x08000, CRC(e67fd534) SHA1(987d0edffc2c243a13d4567319ea3d185eaadbf8) )

	ROM_REGION( 0x140000, "oki1", 0 )
	ROM_LOAD( "rom5.bin", 0x040000, 0x100000, CRC(f6d49863) SHA1(3a3c354852adad06e8a051511abfab7606bce382) )
ROM_END

ROM_START( kingdmgphc01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "rom1_hc01.bin", 0x000000, 0x080000, CRC(a678b149) SHA1(8c1a631e023dbba0a3fa6cd1b7d10dec1663213a) )
	ROM_LOAD16_BYTE( "rom0_hc01.bin", 0x000001, 0x080000, CRC(f226a212) SHA1(526acf3d05fdc88054a772fbea3de2af532bf3d2) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "rom2_hc01.bin", 0x00000, 0x10000, CRC(dde8a57e) SHA1(f522a3f17e229c71512464349760a9e27778bf6a) )

	ROM_REGION( 0x400000, "gp9001_0", 0 )
	ROM_LOAD( "rom3_hc01.bin",  0x000000, 0x200000, CRC(0e797142) SHA1(a480ccd151e49b886d3175a6deff56e1f2c26c3e) )
	ROM_LOAD( "rom4_hc01.bin",  0x200000, 0x200000, CRC(72a6fa53) SHA1(ce92e65205b84361cfb90305a61e9541b5c4dc2f) )

	ROM_REGION( 0x008000, "text", 0 )
	ROM_LOAD( "kgpe.05",  0x000000, 0x008000, CRC(a0707702) SHA1(b6105291c1ae3d0792e69fd2495c31b56fbf61b7) )

	ROM_REGION( 0x80000, "oki1", 0 )
	ROM_LOAD( "rom6_hc01.bin", 0x00000, 0x80000, CRC(199e7cae) SHA1(0f5e13cc8ec42c80bb4bbff90aba29cdb15213d4) )
ROM_END

ROM_START( mahoudaihc01 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ra_ma_01_01.u65", 0x000000, 0x080000, CRC(970ccc5c) SHA1(c87cab83bde0284e631f02e50068407fee81d941) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "ra-ma-01_02.u66", 0x00000, 0x10000, CRC(eabfa46d) SHA1(402c99ebf88f9025f74f0a28ced22b7882a65eb3) )

	ROM_REGION( 0x200000, "gp9001_0", 0 )
	ROM_LOAD( "ra-ma01-rom2_hc01.u2",  0x000000, 0x100000, CRC(865bd232) SHA1(5e8f26b16356c314e244e939ebe696598c1e260d) )
	ROM_LOAD( "ra-ma01-rom3_hc01.u1",  0x100000, 0x100000, CRC(Be068595) SHA1(3e0f9179fce5d9337c18021debe3954157657568) )

	ROM_REGION( 0x008000, "text", 0 )
	ROM_LOAD( "ra_ma_01_05h_hc01.u81",  0x000000, 0x008000, CRC(95d5d1dc) SHA1(a42cabb68443fe71702e552754ed0326480e7c98) )

	ROM_REGION( 0x40000, "oki1", 0 )
	ROM_LOAD( "ra-ma01-rom1.u57", 0x00000, 0x40000, CRC(6edb2ab8) SHA1(e3032e8eda2686f30df4b7a088c5a4d4d45782ed) )
ROM_END

ROM_START( snowbro2hc01 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "pro-4_hc01", 0x000000, 0x080000, CRC(1ad75ae7) SHA1(4c9d7480df4b94aaee5616939547456a0c560f19) )

	ROM_REGION( 0x300000, "gp9001_0", 0 )
	ROM_LOAD( "rom2-l", 0x000000, 0x100000, CRC(e9d366a9) SHA1(e87e3966fce3395324b90db6c134b3345104c04b) )
	ROM_LOAD( "rom2-h", 0x100000, 0x080000, CRC(9aab7a62) SHA1(611f6a15fdbac5d3063426a365538c1482e996bf) )
	ROM_LOAD( "rom3-l", 0x180000, 0x100000, CRC(eb06e332) SHA1(7cd597bfffc153d178530c0f0903bebd751c9dd1) )
	ROM_LOAD( "rom3-h", 0x280000, 0x080000, CRC(df4a952a) SHA1(b76af61c8437caca573ff1312832898666a611aa) )

	ROM_REGION( 0x80000, "oki1", 0 )
	ROM_LOAD( "rom4", 0x00000, 0x80000, CRC(638f341e) SHA1(aa3fca25f099339ece1878ea730c5e9f18ec4823) )
ROM_END

ROM_START( snowbro2hc02 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "pro-4_hc02", 0x000000, 0x080000, CRC(2f219f0a) SHA1(49057ca9f736cd7c8c4588554599250b4aaf6cd1) )

	ROM_REGION( 0x300000, "gp9001_0", 0 )
	ROM_LOAD( "rom2-l", 0x000000, 0x100000, CRC(e9d366a9) SHA1(e87e3966fce3395324b90db6c134b3345104c04b) )
	ROM_LOAD( "rom2-h", 0x100000, 0x080000, CRC(9aab7a62) SHA1(611f6a15fdbac5d3063426a365538c1482e996bf) )
	ROM_LOAD( "rom3-l", 0x180000, 0x100000, CRC(eb06e332) SHA1(7cd597bfffc153d178530c0f0903bebd751c9dd1) )
	ROM_LOAD( "rom3-h", 0x280000, 0x080000, CRC(df4a952a) SHA1(b76af61c8437caca573ff1312832898666a611aa) )

	ROM_REGION( 0x80000, "oki1", 0 )
	ROM_LOAD( "rom4", 0x00000, 0x80000, CRC(638f341e) SHA1(aa3fca25f099339ece1878ea730c5e9f18ec4823) )
ROM_END

ROM_START( truxton2hc01 )
	ROM_REGION( 0x080000, "maincpu", 0 )
	ROM_LOAD16_WORD( "tp024_1_hc01.bin", 0x000000, 0x080000, CRC(599ed4e3) SHA1(43176fbd6440fe7baa00c02a763b641ea2581a4e) )

	ROM_REGION( 0x200000, "gp9001_0", 0 )
	ROM_LOAD( "tp024_4.bin", 0x000000, 0x100000, CRC(805c449e) SHA1(fdf985344145bd320b88b9b0c25e73066c9b2ada) )
	ROM_LOAD( "tp024_3.bin", 0x100000, 0x100000, CRC(47587164) SHA1(bac493e2d5507286b984957b289c929335d27eaa) )

	ROM_REGION( 0x80000, "oki1", 0 )
	ROM_LOAD( "tp024_2.bin", 0x00000, 0x80000, CRC(f2f6cae4) SHA1(bb4e8c36531bed97ced4696ca12fd40ede2531aa) )
ROM_END

/*    YEAR    NAME         PARENT    MACHINE   INPUT                           INIT      MONITOR   COMPANY       FULLNAME FLAGS */
// Hack/Homebrow
GAME( 1993, batsugunhc01,  batsugun, batsugun, batsugun,   toaplan2_state, init_dogyuun,  ROT270,  "hack",       "Batsugun Transparent (Crazy Gun Transparent Version 2015-03-22)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, bgareggahc01,  bgaregga, bgaregga, bgaregga,   toaplan2_state, init_bgaregga, ROT270,  "hack",       "Battle Garegga (Zakk version)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, kingdmgphc01,  kingdmgp, shippumd, kingdmgp,   toaplan2_state, empty_init,    ROT270,  "hack",       "Kingdom Grand Prix" , MACHINE_SUPPORTS_SAVE )
GAME( 1993, mahoudaihc01,  sstriker, mahoudai, mahoudai,   toaplan2_state, empty_init,    ROT270,  "hack",       "Mahou Daisakusen (Translation Chinese)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, snowbro2hc01,  snowbro2, snowbro2, snowbro2,   toaplan2_state, empty_init,    ROT0,    "hack",       "Snow Bros 2 (Ex Super Version)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, snowbro2hc02,  snowbro2, snowbro2, snowbro2,   toaplan2_state, empty_init,    ROT0,    "hack",       "Snow Bros 2 (Change Character)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, truxton2hc01,  truxton2, truxton2, truxton2,   toaplan2_state, empty_init,    ROT270,  "hack",       "Truxton II / Tatsujin Oh (New Version)",  MACHINE_SUPPORTS_SAVE )
