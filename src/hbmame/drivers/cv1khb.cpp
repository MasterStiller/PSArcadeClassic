// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90
#include "../mame/drivers/cv1k.cpp"


/********************
  Hack And Homebrow
***********************/

ROM_START( deathsmlhc01 )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u4", 0x000000, 0x200000, CRC(1a7b98bf) SHA1(07798a4a846e5802756396b34df47d106895c1f1) )
	ROM_RELOAD(0x200000,0x200000)

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "h01.u2", 0x000000, 0x8400000, CRC(d7dd7bad) SHA1(579f14a7f3ec399f6694b20eaae2990fa699aa72) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u23", 0x000000, 0x400000, CRC(aab718c8) SHA1(0e636c46d06151abd6f73232bc479dafcafe5327) )
	ROM_LOAD16_WORD_SWAP( "u24", 0x400000, 0x400000, CRC(83881d84) SHA1(6e2294b247dfcbf0ced155dc45c706f29052775d) )
ROM_END

ROM_START( ddpsdoj )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP("u4", 0x000000, 0x400000, CRC(e2a4411c) SHA1(f8b2b6326dd8eeae99b8b1ab2bd5a3f0b9c7f027) )
	ROM_IGNORE( 0x000100 )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD("u2", 0x000000, 0x8400000, CRC(668e4cd6) SHA1(da0b10865df3a3e46cf8a109ca88a551faba4483) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP("u23", 0x000000, 0x400000, CRC(ac94801c) SHA1(cbcc6d5d89860bc961967e1d3b7c329adaf200c5) )
	ROM_IGNORE( 0x000100 )
	ROM_LOAD16_WORD_SWAP("u24", 0x400000, 0x400000, CRC(f593045b) SHA1(91b92870d0dd2a7817cb0059cc750e2393686f4c) )
	ROM_IGNORE( 0x000100 )
ROM_END

ROM_START( ibarahc01 ) //ibara
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u4_hc01", 0x000000, 0x200000, CRC(d5fb6657) SHA1(4b2c9057cc765054232f520ee1c93495bc6b301e) )
	ROM_RELOAD(0x200000,0x200000)

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "u2", 0x000000, 0x8400000, CRC(55840976) SHA1(4982bdce84f9603adfed7a618f18bc80359ab81e) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "u23", 0x000000, 0x400000, CRC(ee5e585d) SHA1(7eeba4ee693060e927f8c46b16e39227c6a62392) )
	ROM_LOAD16_WORD_SWAP( "u24", 0x400000, 0x400000, CRC(f0aa3cb6) SHA1(f9d137cd879e718811b2d21a0af2a9c6b7dca2f9) )
ROM_END

ROM_START( ddpsdojo )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP("u4", 0x000000, 0x400000, CRC(e2a4411c) SHA1(f8b2b6326dd8eeae99b8b1ab2bd5a3f0b9c7f027) )
	ROM_IGNORE( 0x000100 )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD("old.u2", 0x000000, 0x8400000, CRC(7420302f) SHA1(7db4bd7a72f84fbd6e9c7966769d7b6fccc17cea) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP("u23", 0x000000, 0x400000, CRC(ac94801c) SHA1(cbcc6d5d89860bc961967e1d3b7c329adaf200c5) )
	ROM_IGNORE( 0x000100 )
	ROM_LOAD16_WORD_SWAP("u24", 0x400000, 0x400000, CRC(f593045b) SHA1(91b92870d0dd2a7817cb0059cc750e2393686f4c) )
	ROM_IGNORE( 0x000100 )
ROM_END

ROM_START( sdojak )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojak.u4", 0x000000, 0x400000, CRC(a878ff4c) SHA1(cf5f2f987a775099992009c376466c44170e0e3f) )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "sdojak.u2", 0x000000, 0x8400000, CRC(54353425) SHA1(5c749cd2e93a1b51bfdf1a0d4fd436b4d17acf9f) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojak.u23", 0x000000, 0x400000, CRC(32b91544) SHA1(49e914bfdf33a328fbfa7efb42c5b89915e37643) )
	ROM_LOAD16_WORD_SWAP( "sdojak.u24", 0x400000, 0x400000, CRC(7b9e749f) SHA1(374085a60098cc62501abae4d148bfd88c88149f) )
ROM_END

ROM_START( sdoja )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdoja.u4", 0x000000, 0x400000, CRC(5a3b94e8) SHA1(a546f2baa366ca02dbd462b8ee7808c2643927cb) )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "sdoja.u2", 0x000000, 0x8400000, CRC(7e19adc7) SHA1(6a6e2220096c80471d391d93979e0c883f733c1b) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojak.u23", 0x000000, 0x400000, CRC(32b91544) SHA1(49e914bfdf33a328fbfa7efb42c5b89915e37643) )
	ROM_LOAD16_WORD_SWAP( "sdojak.u24", 0x400000, 0x400000, CRC(7b9e749f) SHA1(374085a60098cc62501abae4d148bfd88c88149f) )
ROM_END

ROM_START( sdojb )
	ROM_REGION( 0x400000, "maincpu", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP( "sdojb.u4", 0x000000, 0x400000, CRC(be54ca1a) SHA1(d7ad13cc6676ab3e938c65b7e6f5c870033f42f1) )

	ROM_REGION( 0x8400000, "game", ROMREGION_ERASEFF)
	ROM_LOAD( "sdojb.u2", 0x000000, 0x8400000, CRC(2a6353a6) SHA1(29b180263d5f08bce7527cf1d4cae33a98d93149) )

	ROM_REGION( 0x800000, "ymz770", ROMREGION_ERASEFF)
	ROM_LOAD16_WORD_SWAP("u23", 0x000000, 0x400000, CRC(ac94801c) SHA1(cbcc6d5d89860bc961967e1d3b7c329adaf200c5) )
	ROM_IGNORE( 0x000100 )
	ROM_LOAD16_WORD_SWAP("u24", 0x400000, 0x400000, CRC(f593045b) SHA1(91b92870d0dd2a7817cb0059cc750e2393686f4c) )
	ROM_IGNORE( 0x000100 )
ROM_END

/*    YEAR     NAME         PARENT          MACHINE    INPUT                       INIT       MONITOR   COMPANY   FULLNAME FLAGS */
// Hack/Homebrow
GAME( 2007, deathsmlhc01,   deathsml,        cv1k,     cv1k, cv1k_state,       init_deathsml,  ROT0,    "hack",   "Death Smiles (Ex Super Version)", 0 )
GAME( 2012, ddpsdoj,        0,               cv1k_d,   cv1k, cv1k_state,       init_dpddfk,    ROT270,  "hack",   "DoDonPachi SaiDaiOuJou (clean dump)", 0 )
GAME( 2012, ddpsdojo,       ddpsdoj,         cv1k_d,   cv1k, cv1k_state,       init_dpddfk,    ROT270,  "hack",   "DoDonPachi SaiDaiOuJou (dirty dump)", 0 )
GAME( 2022, ibarahc01,      ibara,           cv1k,     cv1ks,cv1k_state,       init_ibara,     ROT270,  "hack",   "Ibara (Revised Edition 2022-10-31)", 0 )
GAME( 2021, sdojak,         0,               cv1k_d,   cv1k, cv1k_state,       init_dpddfk,    ROT270,  "hack",   "DoDonPachi SaiDaiOuJou & Knuckles (2021-12-01)", 0 )
GAME( 2021, sdoja,          sdojak,          cv1k_d,   cv1k, cv1k_state,       init_dpddfk,    ROT270,  "hack",   "DoDonPachi SaiDaiOuJou hack", 0 )
GAME( 2022, sdojb,          sdojak,          cv1k_d,   cv1k, cv1k_state,       init_dpddfk,    ROT270,  "hack",   "DoDonPachi SaiDaiOuJou Saya Type B (2022-01-06)", 0 )
