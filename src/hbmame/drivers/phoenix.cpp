// license:BSD-3-Clause
// copyright-holders:Robbbert
#include "../mame/drivers/phoenix.cpp"

 /*******
 Phoenix
**********/

ROM_START( phoenixr )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix3.46",  0x0800, 0x0800, CRC(5748f486) SHA1(49e6fd836d26ec24105e95227b24cf668e8a470a) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenixr.48",  0x1800, 0x0800, CRC(9253e642) SHA1(9f72d7beb22ffc8dffc43765356e34def36efc12) )
	ROM_LOAD( "h5-ic49.5a",   0x2000, 0x0800, CRC(a105e4e7) SHA1(b35142a91b6b7fdf7535202671793393c9f4685f) )
	ROM_LOAD( "h6-ic50.6a",   0x2800, 0x0800, CRC(ac5e9ec1) SHA1(0402e5241d99759d804291998efd43f37ce99917) )
	ROM_LOAD( "h7-ic51.7a",   0x3000, 0x0800, CRC(2eab35b4) SHA1(849bf8273317cc869bdd67e50c68399ee8ece81d) )
	ROM_LOAD( "phoenix3.52",  0x3800, 0x0800, CRC(d2c5c984) SHA1(a9432f9aff8a2f5ca1d347443efc008a177d8ae0) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "r_ic23",       0x0000, 0x0800, CRC(02bc87ea) SHA1(bd672a5c62339e18d627d5bb195a9da2a76fd239) )
	ROM_LOAD( "r_ic24",       0x0800, 0x0800, CRC(675388ee) SHA1(4bef2b7cd28b2311a5775f89ee8756caf405c182) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "r_ic39",       0x0000, 0x0800, CRC(8b1653ba) SHA1(5677fd96d49165b4d71047771e4a0bd8d6fb2400) )
	ROM_LOAD( "r_ic40",       0x0800, 0x0800, CRC(3b811dfb) SHA1(e2a9fada30bdb096ddfd35abc4717e1cac38c943) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic40",   0x0000, 0x0100, CRC(79350b25) SHA1(57411be4c1d89677f7919ae295446da90612c8a8) )
	ROM_LOAD( "mmi6301.ic41",   0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )
ROM_END

ROM_START( phoenixg )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "phoenix.45",   0x0000, 0x0800, CRC(5b8c55a8) SHA1(839c1ca9766f730ec3accd48db70f6429a9c3362) )
	ROM_LOAD( "phoenix.46",   0x0800, 0x0800, CRC(dbc942fa) SHA1(9fe224e6ced407289dfa571468259a021d942b7d) )
	ROM_LOAD( "phoenix.47",   0x1000, 0x0800, CRC(cbbb8839) SHA1(b7f449374cac111081559e39646f973e7e99fd64) )
	ROM_LOAD( "phoenixg.48",  0x1800, 0x0800, CRC(63d4c322) SHA1(96bac0950940f6a3a7972ed403431c6fb1941667) )
	ROM_LOAD( "phoenix.49",   0x2000, 0x0800, CRC(c8a5d6d6) SHA1(ef6ade323544e3edd4101609138ecf35e8cb9577) )
	ROM_LOAD( "h6-ic50.6a",   0x2800, 0x0800, CRC(ac5e9ec1) SHA1(0402e5241d99759d804291998efd43f37ce99917) )
	ROM_LOAD( "h7-ic51.7a",   0x3000, 0x0800, CRC(2eab35b4) SHA1(849bf8273317cc869bdd67e50c68399ee8ece81d) )
	ROM_LOAD( "phoenix.52",   0x3800, 0x0800, CRC(b9915263) SHA1(f61396077b23364b5b26f62c6923394d23a37eb3) )

	ROM_REGION( 0x1000, "bgtiles", 0 )
	ROM_LOAD( "ic23.3d",      0x0000, 0x0800, CRC(3c7e623f) SHA1(e7ff5fc371664af44785c079e92eeb2d8530187b) )
	ROM_LOAD( "ic24.4d",      0x0800, 0x0800, CRC(59916d3b) SHA1(71aec70a8e096ed1f0c2297b3ae7dca1b8ecc38d) )

	ROM_REGION( 0x1000, "fgtiles", 0 )
	ROM_LOAD( "ic39g",        0x0000, 0x0800, CRC(0ed46d30) SHA1(dd21172c11ef60351921ed7067736682237a8db9) )
	ROM_LOAD( "b2-ic40.4b",   0x0800, 0x0800, CRC(0be2ba91) SHA1(af9243ee23377b632b9b7d0b84d341d06bf22480) )

	ROM_REGION( 0x0200, "proms", 0 )
	ROM_LOAD( "mmi6301.ic40",   0x0000, 0x0100, CRC(79350b25) SHA1(57411be4c1d89677f7919ae295446da90612c8a8) )
	ROM_LOAD( "mmi6301.ic41",   0x0100, 0x0100, CRC(e176b768) SHA1(e2184dd495ed579f10b6da0b78379e02d7a6229f) )
ROM_END


/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Phoenix
GAME( 2000, phoenixr, phoenix, phoenix,  phoenix , phoenix_state, empty_init, ROT90, "CYBERYOGI =CO= Windler", "PhoenixR (GDR hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1980, phoenixg, phoenix, phoenix,  phoenixt, phoenix_state, empty_init, ROT90, "GreekRoms",              "Phoenix (Greek)", MACHINE_SUPPORTS_SAVE )
