// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90
#include "../mame/drivers/silkroad.cpp"

/********************
  Hack And Homebrow
***********************/

ROM_START( silkroadhc01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "rom02_hc01.bin", 0x000000, 0x100000, CRC(32b93674) SHA1(eaa1dd00b561b32e0897c55c760318a8ddc75164) )
	ROM_LOAD32_WORD_SWAP( "rom03_hc01.bin", 0x000002, 0x100000, CRC(2cdc10a4) SHA1(7b902487073265ed55a68758d8f01e4b78b1b8dd) )

	ROM_REGION( 0x1800000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "rom12.rom12", 0x0000000, 0x0200000, CRC(96393d04) SHA1(f512bb8603510d39e649f4ec1c5e2d0e4bf3a2cc) )
	ROM_LOAD( "rom08.rom08", 0x0800000, 0x0200000, CRC(23f1d462) SHA1(6ca8052b16ccc1fe59716e03f66bd33af5145b37) )
	ROM_LOAD( "rom04.rom04", 0x1000000, 0x0200000, CRC(d9f0bbd7) SHA1(32c055ad5497c0bec5db40b528e589d7724e354f) )

	ROM_LOAD( "rom13.rom13", 0x0200000, 0x0200000, CRC(4ca1698e) SHA1(4fffc2f2a5fb434c42463ce904fd811866c53f81) )
	ROM_LOAD( "rom09.rom09", 0x0a00000, 0x0200000, CRC(ef0b5bf4) SHA1(acd3bc5070de84608c5da0d091094382853cb048) )
	ROM_LOAD( "rom05.rom05", 0x1200000, 0x0200000, CRC(512d6e25) SHA1(fc0a56663d77bbdfbd4242e14a55563073634582) )

	ROM_LOAD( "rom14.rom14", 0x0400000, 0x0200000, CRC(d00b19c4) SHA1(d5b955dca5d0d251166a7f35a0bbbda6a91ecbd0) )
	ROM_LOAD( "rom10.rom10", 0x0c00000, 0x0200000, CRC(7d324280) SHA1(cdf6d9342292f693cc5ec1b72816f2788963fcec) )
	ROM_LOAD( "rom06.rom06", 0x1400000, 0x0200000, CRC(3ac26060) SHA1(98ad8efbbf8020daf7469db3e0fda02af6c4c767) )

	ROM_LOAD( "rom07.rom07", 0x0600000, 0x0200000, CRC(9fc6ff9d) SHA1(51c3ca9709a01e0ad6bc76c0d674ed03f9822598) )
	ROM_LOAD( "rom11.rom11", 0x0e00000, 0x0200000, CRC(11abaf1c) SHA1(19e86f3ebfec518a96c0520f36cfc1b525e7e55c) )
	ROM_LOAD( "rom15.rom15", 0x1600000, 0x0200000, CRC(26a3b168) SHA1(a4b7955cc4d4fbec7c975a9456f2219ef33f1166) )

	ROM_REGION( 0x080000, "oki1", 0 )
	ROM_LOAD( "rom00.bin", 0x000000, 0x080000, CRC(b10ba7ab) SHA1(a6a3ae71b803af9c31d7e97dc86cfcc123ee9a40) )

	ROM_REGION( 0x040000, "oki2", 0 )
	ROM_LOAD( "rom01.bin", 0x000000, 0x040000, CRC(db8cb455) SHA1(6723b4018208d554bd1bf1e0640b72d2f4f47302) )
ROM_END

ROM_START( silkroadhc02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "rom02_hc02.bin", 0x000000, 0x100000, CRC(a4fea2c0) SHA1(0acd17bce5f62ff374dfa07ecdaed851d71e58bf) )
	ROM_LOAD32_WORD_SWAP( "rom03_hc02.bin", 0x000002, 0x100000, CRC(a4318e7b) SHA1(01dc82bdc421031c13c2b08b8c79457c5d1d644c) )

	ROM_REGION( 0x1800000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "rom12.rom12", 0x0000000, 0x0200000, CRC(96393d04) SHA1(f512bb8603510d39e649f4ec1c5e2d0e4bf3a2cc) )
	ROM_LOAD( "rom08.rom08", 0x0800000, 0x0200000, CRC(23f1d462) SHA1(6ca8052b16ccc1fe59716e03f66bd33af5145b37) )
	ROM_LOAD( "rom04.rom04", 0x1000000, 0x0200000, CRC(d9f0bbd7) SHA1(32c055ad5497c0bec5db40b528e589d7724e354f) )

	ROM_LOAD( "rom13.rom13", 0x0200000, 0x0200000, CRC(4ca1698e) SHA1(4fffc2f2a5fb434c42463ce904fd811866c53f81) )
	ROM_LOAD( "rom09.rom09", 0x0a00000, 0x0200000, CRC(ef0b5bf4) SHA1(acd3bc5070de84608c5da0d091094382853cb048) )
	ROM_LOAD( "rom05.rom05", 0x1200000, 0x0200000, CRC(512d6e25) SHA1(fc0a56663d77bbdfbd4242e14a55563073634582) )

	ROM_LOAD( "rom14.rom14", 0x0400000, 0x0200000, CRC(d00b19c4) SHA1(d5b955dca5d0d251166a7f35a0bbbda6a91ecbd0) )
	ROM_LOAD( "rom10.rom10", 0x0c00000, 0x0200000, CRC(7d324280) SHA1(cdf6d9342292f693cc5ec1b72816f2788963fcec) )
	ROM_LOAD( "rom06.rom06", 0x1400000, 0x0200000, CRC(3ac26060) SHA1(98ad8efbbf8020daf7469db3e0fda02af6c4c767) )

	ROM_LOAD( "rom07.rom07", 0x0600000, 0x0200000, CRC(9fc6ff9d) SHA1(51c3ca9709a01e0ad6bc76c0d674ed03f9822598) )
	ROM_LOAD( "rom11.rom11", 0x0e00000, 0x0200000, CRC(11abaf1c) SHA1(19e86f3ebfec518a96c0520f36cfc1b525e7e55c) )
	ROM_LOAD( "rom15.rom15", 0x1600000, 0x0200000, CRC(26a3b168) SHA1(a4b7955cc4d4fbec7c975a9456f2219ef33f1166) )

	ROM_REGION( 0x080000, "oki1", 0 )
	ROM_LOAD( "rom00.bin", 0x000000, 0x080000, CRC(b10ba7ab) SHA1(a6a3ae71b803af9c31d7e97dc86cfcc123ee9a40) )

	ROM_REGION( 0x040000, "oki2", 0 )
	ROM_LOAD( "rom01.bin", 0x000000, 0x040000, CRC(db8cb455) SHA1(6723b4018208d554bd1bf1e0640b72d2f4f47302) )
ROM_END

ROM_START( silkroadhc03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "rom02_hc03.bin", 0x000000, 0x100000, CRC(d8159448) SHA1(002cf208e57ae9ab9a4b4f7081ec139d61b28d06) )
	ROM_LOAD32_WORD_SWAP( "rom03_hc03.bin", 0x000002, 0x100000, CRC(fb215953) SHA1(435708ebc8027002bf2864c357760f599e611554) )

	ROM_REGION( 0x1800000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "rom12.rom12", 0x0000000, 0x0200000, CRC(96393d04) SHA1(f512bb8603510d39e649f4ec1c5e2d0e4bf3a2cc) )
	ROM_LOAD( "rom08.rom08", 0x0800000, 0x0200000, CRC(23f1d462) SHA1(6ca8052b16ccc1fe59716e03f66bd33af5145b37) )
	ROM_LOAD( "rom04.rom04", 0x1000000, 0x0200000, CRC(d9f0bbd7) SHA1(32c055ad5497c0bec5db40b528e589d7724e354f) )

	ROM_LOAD( "rom13.rom13", 0x0200000, 0x0200000, CRC(4ca1698e) SHA1(4fffc2f2a5fb434c42463ce904fd811866c53f81) )
	ROM_LOAD( "rom09.rom09", 0x0a00000, 0x0200000, CRC(ef0b5bf4) SHA1(acd3bc5070de84608c5da0d091094382853cb048) )
	ROM_LOAD( "rom05.rom05", 0x1200000, 0x0200000, CRC(512d6e25) SHA1(fc0a56663d77bbdfbd4242e14a55563073634582) )

	ROM_LOAD( "rom14.rom14", 0x0400000, 0x0200000, CRC(d00b19c4) SHA1(d5b955dca5d0d251166a7f35a0bbbda6a91ecbd0) )
	ROM_LOAD( "rom10.rom10", 0x0c00000, 0x0200000, CRC(7d324280) SHA1(cdf6d9342292f693cc5ec1b72816f2788963fcec) )
	ROM_LOAD( "rom06.rom06", 0x1400000, 0x0200000, CRC(3ac26060) SHA1(98ad8efbbf8020daf7469db3e0fda02af6c4c767) )

	ROM_LOAD( "rom07.rom07", 0x0600000, 0x0200000, CRC(9fc6ff9d) SHA1(51c3ca9709a01e0ad6bc76c0d674ed03f9822598) )
	ROM_LOAD( "rom11.rom11", 0x0e00000, 0x0200000, CRC(11abaf1c) SHA1(19e86f3ebfec518a96c0520f36cfc1b525e7e55c) )
	ROM_LOAD( "rom15.rom15", 0x1600000, 0x0200000, CRC(26a3b168) SHA1(a4b7955cc4d4fbec7c975a9456f2219ef33f1166) )

	ROM_REGION( 0x080000, "oki1", 0 )
	ROM_LOAD( "rom00.bin", 0x000000, 0x080000, CRC(b10ba7ab) SHA1(a6a3ae71b803af9c31d7e97dc86cfcc123ee9a40) )

	ROM_REGION( 0x040000, "oki2", 0 )
	ROM_LOAD( "rom01.bin", 0x000000, 0x040000, CRC(db8cb455) SHA1(6723b4018208d554bd1bf1e0640b72d2f4f47302) )
ROM_END

ROM_START( silkroadhc04 ) //silkroad1v2
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "rom02_hc04.bin", 0x000000, 0x100000, CRC(c53421b0) SHA1(20125655f3fcfe95b2b4614401cc818ec8a7ca90) )
	ROM_LOAD32_WORD_SWAP( "rom03_hc04.bin", 0x000002, 0x100000, CRC(37d639ec) SHA1(6ae2b4fb2741176894049eff6773bfa37a797969) )

	ROM_REGION( 0x1800000, "gfx1", ROMREGION_INVERT )
	ROM_LOAD( "rom12.rom12", 0x0000000, 0x0200000, CRC(96393d04) SHA1(f512bb8603510d39e649f4ec1c5e2d0e4bf3a2cc) )
	ROM_LOAD( "rom08.rom08", 0x0800000, 0x0200000, CRC(23f1d462) SHA1(6ca8052b16ccc1fe59716e03f66bd33af5145b37) )
	ROM_LOAD( "rom04.rom04", 0x1000000, 0x0200000, CRC(d9f0bbd7) SHA1(32c055ad5497c0bec5db40b528e589d7724e354f) )

	ROM_LOAD( "rom13.rom13", 0x0200000, 0x0200000, CRC(4ca1698e) SHA1(4fffc2f2a5fb434c42463ce904fd811866c53f81) )
	ROM_LOAD( "rom09.rom09", 0x0a00000, 0x0200000, CRC(ef0b5bf4) SHA1(acd3bc5070de84608c5da0d091094382853cb048) )
	ROM_LOAD( "rom05.rom05", 0x1200000, 0x0200000, CRC(512d6e25) SHA1(fc0a56663d77bbdfbd4242e14a55563073634582) )

	ROM_LOAD( "rom14.rom14", 0x0400000, 0x0200000, CRC(d00b19c4) SHA1(d5b955dca5d0d251166a7f35a0bbbda6a91ecbd0) )
	ROM_LOAD( "rom10.rom10", 0x0c00000, 0x0200000, CRC(7d324280) SHA1(cdf6d9342292f693cc5ec1b72816f2788963fcec) )
	ROM_LOAD( "rom06.rom06", 0x1400000, 0x0200000, CRC(3ac26060) SHA1(98ad8efbbf8020daf7469db3e0fda02af6c4c767) )

	ROM_LOAD( "rom07.rom07", 0x0600000, 0x0200000, CRC(9fc6ff9d) SHA1(51c3ca9709a01e0ad6bc76c0d674ed03f9822598) )
	ROM_LOAD( "rom11.rom11", 0x0e00000, 0x0200000, CRC(11abaf1c) SHA1(19e86f3ebfec518a96c0520f36cfc1b525e7e55c) )
	ROM_LOAD( "rom15.rom15", 0x1600000, 0x0200000, CRC(26a3b168) SHA1(a4b7955cc4d4fbec7c975a9456f2219ef33f1166) )

	ROM_REGION( 0x080000, "oki1", 0 )
	ROM_LOAD( "rom00.bin", 0x000000, 0x080000, CRC(b10ba7ab) SHA1(a6a3ae71b803af9c31d7e97dc86cfcc123ee9a40) )

	ROM_REGION( 0x040000, "oki2", 0 )
	ROM_LOAD( "rom01.bin", 0x000000, 0x040000, CRC(db8cb455) SHA1(6723b4018208d554bd1bf1e0640b72d2f4f47302) )
ROM_END

/*    YEAR  NAME           PARENT      MACHINE    INPUT                        INIT   MONITOR  COMPANY     FULLNAME FLAGS */
// Hack/Homebrow
GAME( 1999, silkroadhc01,  silkroad,   silkroad, silkroad, silkroad_state, empty_init, ROT0,  "hack",      "The Legend of Silkroad (Simple Attack)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, silkroadhc02,  silkroad,   silkroad, silkroad, silkroad_state, empty_init, ROT0,  "DDJ",       "The Legend of Silkroad (All Move Without Magic)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, silkroadhc03,  silkroad,   silkroad, silkroad, silkroad_state, empty_init, ROT0,  "DDJ",       "The Legend of Silkroad (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, silkroadhc04,  silkroad,   silkroad, silkroad, silkroad_state, empty_init, ROT0,  "hack",      "The Legend of Silkroad (1V2 Edition 2020-12-07)", MACHINE_SUPPORTS_SAVE )
