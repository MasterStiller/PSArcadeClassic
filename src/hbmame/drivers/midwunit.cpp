// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90
#include "../mame/drivers/midwunit.cpp"

 /****************
 Mortal Kombat 3
*******************/

ROM_START( mk3hack01 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u2_music_spch.u2", 0x000000, 0x100000, CRC(5273436f) SHA1(e1735842a0159eafe79d878d44e3828df9bfa5bb) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3", 0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4", 0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5", 0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "mk321u54_k01.bin",  0x00000, 0x80000, CRC(e2ddd5d3) SHA1(2529b198a4207b97ccbce7c27752c8e51f44d37f) )
	ROM_LOAD16_BYTE( "mk321u63_k01.bin",  0x00001, 0x80000, CRC(c0acfa7e) SHA1(823fd8d80a09d1aa19271d5c045fb3affcdcdde2) )

	ROM_REGION( 0x2000000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u121_game_rom.u121",  0x0c00000, 0x100000, CRC(b6c6296a) SHA1(7b92a92d65493bb201daf5ece6f00140f017ac51) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u120_game_rom.u120",  0x0c00001, 0x100000, CRC(8d1ccc3b) SHA1(35d91c00113718a08a9d56eb04366f8cf4069ba6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u119_game_rom.u119",  0x0c00002, 0x100000, CRC(63215b59) SHA1(709bce15fba1520bcba40f0a5cb614542f1b460f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u118_game_rom.u118",  0x0c00003, 0x100000, CRC(8b681e34) SHA1(524104ba2eb2deb3cfae9760e6b2125ce6b6633e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u117_game_rom.u117",  0x1000000, 0x080000, CRC(1ab20377) SHA1(0c0d14464d8b23a60e0693669af2ddb82655eff8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u116_game_rom.u116",  0x1000001, 0x080000, CRC(ba246ad0) SHA1(ec6bdd4b9cd3007bb66bb8de36d148abb30e7f11) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u115_game_rom.u115",  0x1000002, 0x080000, CRC(3ee8b124) SHA1(1523d51e36d3c336b134a562da36a29ba137c0f6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u114_game_rom.u114",  0x1000003, 0x080000, CRC(a8d99922) SHA1(04ff8d85448701070672f44dbf5bcfd744f1bc8a) )
ROM_END

 /**************************
 Ultimate Mortal Kombat 3
*****************************/

ROM_START( umk3hack01 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2",  0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3",  0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4",  0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5",  0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.2_mortal_kombat_3_u54_ultimate_hack01.u54",  0x00000, 0x80000, CRC(7bd7de7b) SHA1(b1787f732c15ff217bdc9ed5f83dbf2589e32cfb) )
	ROM_LOAD16_BYTE( "l1.2_mortal_kombat_3_u63_ultimate_hack01.u63",  0x00001, 0x80000, CRC(03ba8907) SHA1(8cf79dc054fe40bc28dbd74a6198468c9ae8c948) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )   /* security PIC (provides game ID code and serial number) */
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2000000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image_hack01.u121",  0x0c00000, 0x100000, CRC(d17b30f3) SHA1(d9f65a475bea10aca7aae29dd32fc4b3fdd43aca) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image_hack01.u120",  0x0c00001, 0x100000, CRC(c80dbff9) SHA1(e8f903f276487b8aec899d5726b04bef38baf521) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image_hack01.u119",  0x0c00002, 0x100000, CRC(518f8083) SHA1(7e2f8e8a40224db00a658dc121df686492bd5d81) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image_hack01.u118",  0x0c00003, 0x100000, CRC(bbc57990) SHA1(fcf1a90a607d2a054aa633578671c688a8df580c) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image.u113",  0x1400000, 0x100000, CRC(99d74a1e) SHA1(ed3068afa98287ea290d1f537f5009d3b6d683da) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image.u112",  0x1400001, 0x100000, CRC(b5a46488) SHA1(dbf22e55d200eb9ff550f48b223cf0c6114a9357) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image.u111",  0x1400002, 0x100000, CRC(a87523c8) SHA1(e70b7599fef82001f762fc2c48f7b85474431ccc) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image.u110",  0x1400003, 0x100000, CRC(0038f205) SHA1(059c1c71a2d92ee6db36c09831d213a48a7e81d0) )
ROM_END

ROM_START( umk3hack02 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2",  0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3",  0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4",  0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5",  0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack02.u54",  0x00000, 0x80000, CRC(7ea6c327) SHA1(266f67302c8a98e96d077ba42bba47dbe5127882) )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack02.u63",  0x00001, 0x80000, CRC(51f66368) SHA1(a1e0142e39e123df0e4de525f62328d3c9049244) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )   /* security PIC (provides game ID code and serial number) */
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2000000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image.u121",  0x0c00000, 0x100000, CRC(cc4b95db) SHA1(3d53180eec649e9616c4b87db55573f12d9bfee3) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image.u120",  0x0c00001, 0x100000, CRC(1c8144cd) SHA1(77cdc1eaf630ccb7233f5532f8b08191d00f0816) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image.u119",  0x0c00002, 0x100000, CRC(5f10c543) SHA1(24dc83b7aa531ebd399258ffa7b2e028f1c4a28e) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image.u118",  0x0c00003, 0x100000, CRC(de0c4488) SHA1(227cab34798c440b2a45223567113df5f17d913f) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image.u113",  0x1400000, 0x100000, CRC(99d74a1e) SHA1(ed3068afa98287ea290d1f537f5009d3b6d683da) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image.u112",  0x1400001, 0x100000, CRC(b5a46488) SHA1(dbf22e55d200eb9ff550f48b223cf0c6114a9357) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image.u111",  0x1400002, 0x100000, CRC(a87523c8) SHA1(e70b7599fef82001f762fc2c48f7b85474431ccc) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image.u110",  0x1400003, 0x100000, CRC(0038f205) SHA1(059c1c71a2d92ee6db36c09831d213a48a7e81d0) )
ROM_END

ROM_START( umk3hack03 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2", 0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3", 0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4", 0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5", 0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack03.u54",  0x00000, 0x80000, CRC(d0df2866) SHA1(dfff0bbee046bf95134d3850196013b06390aa9a) )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack03.u63",  0x00001, 0x80000, CRC(42314fb4) SHA1(3c3024c0598c808c8ef9225ffe46c1dd11ab8e39) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2000000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image_hack01.u121",  0x0c00000, 0x100000, CRC(d17b30f3) SHA1(d9f65a475bea10aca7aae29dd32fc4b3fdd43aca) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image_hack01.u120",  0x0c00001, 0x100000, CRC(c80dbff9) SHA1(e8f903f276487b8aec899d5726b04bef38baf521) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image_hack01.u119",  0x0c00002, 0x100000, CRC(518f8083) SHA1(7e2f8e8a40224db00a658dc121df686492bd5d81) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image_hack01.u118",  0x0c00003, 0x100000, CRC(bbc57990) SHA1(fcf1a90a607d2a054aa633578671c688a8df580c) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image.u113",  0x1400000, 0x100000, CRC(99d74a1e) SHA1(ed3068afa98287ea290d1f537f5009d3b6d683da) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image.u112",  0x1400001, 0x100000, CRC(b5a46488) SHA1(dbf22e55d200eb9ff550f48b223cf0c6114a9357) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image.u111",  0x1400002, 0x100000, CRC(a87523c8) SHA1(e70b7599fef82001f762fc2c48f7b85474431ccc) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image.u110",  0x1400003, 0x100000, CRC(0038f205) SHA1(059c1c71a2d92ee6db36c09831d213a48a7e81d0) )
ROM_END

ROM_START( umk3hack04 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2",  0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3",  0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4",  0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5",  0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack04.u54",  0x00000, 0x80000, CRC(6aba5ad4) SHA1(4e9fe13eac3755727ebe594de3ba2c54d52e2739) )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack04.u63",  0x00001, 0x80000, CRC(596c2325) SHA1(3e6fd4894bc4d2efc8c99e88de9d51675cef2967) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )   /* security PIC (provides game ID code and serial number) */
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2000000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image.u121",  0x0c00000, 0x100000, CRC(cc4b95db) SHA1(3d53180eec649e9616c4b87db55573f12d9bfee3) ) /* Both v1.0 & v1.1 have been found with mask roms  */
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image.u120",  0x0c00001, 0x100000, CRC(1c8144cd) SHA1(77cdc1eaf630ccb7233f5532f8b08191d00f0816) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image.u119",  0x0c00002, 0x100000, CRC(5f10c543) SHA1(24dc83b7aa531ebd399258ffa7b2e028f1c4a28e) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image.u118",  0x0c00003, 0x100000, CRC(de0c4488) SHA1(227cab34798c440b2a45223567113df5f17d913f) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image.u113",  0x1400000, 0x100000, CRC(99d74a1e) SHA1(ed3068afa98287ea290d1f537f5009d3b6d683da) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image.u112",  0x1400001, 0x100000, CRC(b5a46488) SHA1(dbf22e55d200eb9ff550f48b223cf0c6114a9357) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image.u111",  0x1400002, 0x100000, CRC(a87523c8) SHA1(e70b7599fef82001f762fc2c48f7b85474431ccc) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image.u110",  0x1400003, 0x100000, CRC(0038f205) SHA1(059c1c71a2d92ee6db36c09831d213a48a7e81d0) )
ROM_END

ROM_START( umk3hack05 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2",  0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3",  0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4",  0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5",  0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack05.u54", 0x00000, 0x80000, CRC(7d2d5ce3) SHA1(b96634ff7e0bea233b09c94c52f9b266c61660b6) )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack05.u63", 0x00001, 0x80000, CRC(df16455c) SHA1(c6dc0522b096a16478fac0e3ddd0417d8c1f8208) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2400000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom_hack05.u133",  0x0000000, 0x100000, CRC(62c6f8ab) SHA1(cfe77e21cb25b4513411658051b849e3c0dd728c) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom_hack05.u132",  0x0000001, 0x100000, CRC(87d977dc) SHA1(3bd6ded7c212621c3f45e29fa90a19b1c7abaac6) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom_hack05.u131",  0x0000002, 0x100000, CRC(483cbc7b) SHA1(39d03642ea62f8623b76ecf084a390db7c366dcd) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom_hack05.u130",  0x0000003, 0x100000, CRC(ecf63628) SHA1(3efeda0a59f110e6ab17247b1fed7965291ec885) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom_hack05.u125",  0x0800000, 0x100000, CRC(78e8b9e5) SHA1(9a5f87e896c023b70b74a40c4fcb82e46cd8b1cb) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom_hack05.u124",  0x0800001, 0x100000, CRC(1137536c) SHA1(8aee7855a454eeb68e51205ba47e94a0a2915ee3) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom_hack05.u123",  0x0800002, 0x100000, CRC(6a65e954) SHA1(b04e64f361500093450a1a648dad13412282bda1) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom_hack05.u122",  0x0800003, 0x100000, CRC(e6274718) SHA1(66bc57f4e05af9b27caa89fb0a3bd5aef3892117) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image_hack05.u121",  0x0c00000, 0x100000, CRC(efd4084c) SHA1(f8ac8227443abcc313c358c4dbb34ad896ad7b70) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image_hack05.u120",  0x0c00001, 0x100000, CRC(42a6c0e6) SHA1(853dce49454a86745ec4034130d6720457d703a1) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image_hack05.u119",  0x0c00002, 0x100000, CRC(948d4aa7) SHA1(e506327edcba86cb56d8ecb787e75babf4019a71) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image_hack05.u118",  0x0c00003, 0x100000, CRC(ab7ca588) SHA1(f7c539183d203ec9f48ee27ba8a9dc2efc4d5035) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u117_video_image_hack05.u117",  0x1000000, 0x100000, CRC(86267654) SHA1(5fb4ce7cc711f67131a7a992f0cd2c1c3a617111) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u116_video_image_hack05.u116",  0x1000001, 0x100000, CRC(eee8ebf0) SHA1(c811bd913dccdf13377a91cc835e1591e4134b0e) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u115_video_image_hack05.u115",  0x1000002, 0x100000, CRC(4bffa1d6) SHA1(8d61c1d5fbb9106d31ba66640c003cbe75d9bd95) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u114_video_image_hack05.u114",  0x1000003, 0x100000, CRC(128e3a4c) SHA1(6dcaf10cedea8a42a16a7310f70164c0138b1343) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image_hack05.u113",  0x1400000, 0x100000, CRC(b352019b) SHA1(fa4e902e5750cf542ac5b566caa27d790ceb2544) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image_hack05.u112",  0x1400001, 0x100000, CRC(71143b7c) SHA1(5e97e54406002aa2297879a7b358f1d263593ffe) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image_hack05.u111",  0x1400002, 0x100000, CRC(09821226) SHA1(740aa13569e59963ee87f32c39b558cb53f8a81d) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image_hack05.u110",  0x1400003, 0x100000, CRC(88f78828) SHA1(95f83c2f3d7026c2f592bb5de3142613d2a7e6ce) )
ROM_END

ROM_START( umk3hack06 )
	ROM_REGION16_LE( 0x800000, "dcs", ROMREGION_ERASEFF )
	ROM_LOAD16_BYTE( "l2.0_mortal_kombat_3_u2_ultimate.u2", 0x000000, 0x100000, CRC(3838cfe5) SHA1(e3d2901f3bae1362742fc6ee0aa31c9f63b4dfa3) ) /* verified labeled as L2.0 */
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u3_music_spch.u3", 0x200000, 0x100000, CRC(856fe411) SHA1(6165ebecfce7500e948d84492ffa19eed7f47091) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u4_music_spch.u4", 0x400000, 0x100000, CRC(428a406f) SHA1(e70ec83cd054de0da1e178720ed0035b8887f797) )
	ROM_LOAD16_BYTE( "l1_mortal_kombat_3_u5_music_spch.u5", 0x600000, 0x100000, CRC(3b98a09f) SHA1(edf1d02a56dcf3349e6b4bb4097acfe7592305f4) )

	ROM_REGION16_LE( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack06.u54",  0x00000, 0x80000, CRC(a46ee73c) SHA1(2ad13bf20b9e42729773307b55fa67e430b1cf87) )
	ROM_LOAD16_BYTE( "l1.0_mortal_kombat_3_hack06.u63",  0x00001, 0x80000, CRC(4f200db2) SHA1(25bab2c52df59056e3018d88491de1f2b1a8eed2) )

	ROM_REGION( 0x1009, "serial_security:pic", 0 )
	ROM_LOAD( "463_mk3_ultimate.u64",  0x0000, 0x1009, CRC(4f425218) SHA1(7f26045ed2c9ca94fadcb673ce10f28208aa720e) )

	ROM_REGION( 0x2400000, "gfxrom", 0 )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u133_game_rom.u133",  0x0000000, 0x100000, CRC(79b94667) SHA1(31bba640c351fdccc6685cadb74dd79a3f910ce8) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u132_game_rom.u132",  0x0000001, 0x100000, CRC(13e95228) SHA1(405b05f5a5a55667c2be17d4b399129bdacefd90) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u131_game_rom.u131",  0x0000002, 0x100000, CRC(41001e30) SHA1(2cec91116771951c0380cec5debf4cbb40c14c61) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u130_game_rom.u130",  0x0000003, 0x100000, CRC(49379dd7) SHA1(e6dfab4e23d9cc38ae56c1bbf10ccd160e8fad5e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u129_game_rom.u129",  0x0400000, 0x100000, CRC(a8b41803) SHA1(9697e35e8bb51d6d36b1d7ae47377b446e57682f) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u128_game_rom.u128",  0x0400001, 0x100000, CRC(b410d72f) SHA1(ac5c1c6f744186540f4ab100d9bd4ce6007e600b) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u127_game_rom.u127",  0x0400002, 0x100000, CRC(bd985be7) SHA1(f5183abea2e5eb2c2c8cefa72c9ed321679f5128) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u126_game_rom.u126",  0x0400003, 0x100000, CRC(e7c32cf4) SHA1(94ea7b2eed7dae66f5dd676c20d6b360140e3e0e) )

	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u125_game_rom.u125",  0x0800000, 0x100000, CRC(9a52227e) SHA1(0474a14fa8dbfea0b0889c1d1756b86391683558) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u124_game_rom.u124",  0x0800001, 0x100000, CRC(5c750ebc) SHA1(45d68af1a56994376e086d840502453c8d6be700) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u123_game_rom.u123",  0x0800002, 0x100000, CRC(f0ab88a8) SHA1(cdc9dc12e162255845c6627b1e35182b7e8502d0) )
	ROM_LOAD32_BYTE( "l1_mortal_kombat_3_u122_game_rom.u122",  0x0800003, 0x100000, CRC(9b87cdac) SHA1(a5f8db559293978f23e6f105543d8b2e170a2e0d) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u121_video_image.u121",  0x0c00000, 0x100000, CRC(cc4b95db) SHA1(3d53180eec649e9616c4b87db55573f12d9bfee3) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u120_video_image.u120",  0x0c00001, 0x100000, CRC(1c8144cd) SHA1(77cdc1eaf630ccb7233f5532f8b08191d00f0816) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u119_video_image.u119",  0x0c00002, 0x100000, CRC(5f10c543) SHA1(24dc83b7aa531ebd399258ffa7b2e028f1c4a28e) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u118_video_image.u118",  0x0c00003, 0x100000, CRC(de0c4488) SHA1(227cab34798c440b2a45223567113df5f17d913f) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u117_video_image_hack06.u117",  0x1000000, 0x080000, CRC(52ee547e) SHA1(97dba4d6dd503a623dad5709767e4617de211af0) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u116_video_image_hack06.u116",  0x1000001, 0x080000, CRC(f6df5c98) SHA1(e8317cf46d23738223f034c10f0f6f874d72165f) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u115_video_image_hack06.u115",  0x1000002, 0x080000, CRC(e003a51b) SHA1(e11c0cb0a9d647f17398b48d0b7d088526a18af8) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u114_video_image_hack06.u114",  0x1000003, 0x080000, CRC(b8504598) SHA1(fce32581d6ff9e1a7455c30fb10812ba9dc70db5) )

	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u113_video_image.u113",  0x1400000, 0x100000, CRC(99d74a1e) SHA1(ed3068afa98287ea290d1f537f5009d3b6d683da) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u112_video_image.u112",  0x1400001, 0x100000, CRC(b5a46488) SHA1(dbf22e55d200eb9ff550f48b223cf0c6114a9357) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u111_video_image.u111",  0x1400002, 0x100000, CRC(a87523c8) SHA1(e70b7599fef82001f762fc2c48f7b85474431ccc) )
	ROM_LOAD32_BYTE( "mortal_kombat_iii_ultimate_u110_video_image.u110",  0x1400003, 0x100000, CRC(0038f205) SHA1(059c1c71a2d92ee6db36c09831d213a48a7e81d0) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Mortal Kombat 3
GAME( 2018, mk3hack01,      mk3,  wunit_picsim, mk3, midwunit_state, init_mk3,  ROT0, "TEAM UMK 3",       "Mortal Kombat 3 (Free Play 2018-02-20)", MACHINE_SUPPORTS_SAVE )
// Ultimate Mortal Kombat 3
GAME( 2010, umk3hack01,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "Midway",           "Ultimate Mortal Kombat 3 (Juggernauts Hack Edition 1.0)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, umk3hack02,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "Midway",           "Ultimate Mortal Kombat 3 (Zeus hack Edition)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, umk3hack03,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "Midway",           "Ultimate Mortal Kombat 3 (Tournament Edition 2.0.042)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, umk3hack04,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "Mortalkombatplus", "Ultimate Mortal Kombat 3 (rev 1.3 Final Public Beta)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, umk3hack05,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "TEAM UMK 3",       "Ultimate Mortal Kombat 3 (Ultimate Cup Edition 2020-09-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, umk3hack06,     umk3, wunit_picemu, mk3, midwunit_state, init_umk3, ROT0, "Mortalkombatplus", "Ultimate Mortal Kombat 3 (Plus Beta 1 2019-09-21)", MACHINE_SUPPORTS_SAVE )
