// Proyecto Shadows Arcade Classic+
// copyright-holders:Gaston90
#include "includes/neogeo.h"

void neogeo_state::init_garoud()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_sma_prot->garou_decrypt_68k(cpuregion);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sma_prot->garou_install_protection(m_maincpu, m_banked_cart);
}

void neogeo_state::init_jckeygpd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);

	/* install some extra RAM */
	m_extra_ram = std::make_unique<uint16_t[]>(0x1000);
	m_maincpu->space(AS_PROGRAM).install_ram(0x200000, 0x201fff, m_extra_ram.get());
	save_pointer(NAME(m_extra_ram), 0x1000);
}

void neogeo_state::init_kof2k4pls()
{
	init_neogeo();

	u8 *src = cpuregion;
	std::vector<u8> dst( 0x200000 );

	memcpy(&dst[0],src,0x100000);
	memcpy(&dst[0x100000],src+0x500000,0x100000);
	memcpy(src+0x000000,&dst[0x1A0000],0x020000);
	memcpy(src+0x020000,&dst[0x080000],0x020000);
	memcpy(src+0x040000,&dst[0x140000],0x020000);
	memcpy(src+0x060000,&dst[0x000000],0x020000);
	memcpy(src+0x080000,&dst[0x180000],0x020000);
	memcpy(src+0x0A0000,&dst[0x0A0000],0x020000);
	memcpy(src+0x0C0000,&dst[0x100000],0x020000);
	memcpy(src+0x0E0000,&dst[0x040000],0x020000);

	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size, 1);
}

void neogeo_state::init_cthd2k3a()
{
	init_neogeo();

	u8 nBank[] = {
		0x06, 0x02, 0x04, 0x05, 0x01, 0x03, 0x00, 0x07,
		0x27, 0x0E, 0x1C, 0x15, 0x1B, 0x17, 0x0A, 0x0F,
		0x16, 0x14, 0x23, 0x0B, 0x22, 0x26, 0x08, 0x24,
		0x21, 0x13, 0x1A, 0x0C, 0x19, 0x1D, 0x25, 0x10,
		0x09, 0x20, 0x18, 0x1F, 0x1E, 0x12, 0x0D, 0x11
	};

	u8 *src = cpuregion;
	std::vector<u8> dst( cpuregion_size );

	for (int i = 0; i < 0x500000 / 0x20000; i++)
		memcpy (&dst[i * 0x20000], src + nBank[i] * 0x20000, 0x20000);

	memcpy (src, &dst[0], 0x500000);

	m_bootleg_prot->decrypt_cthd2003(spr_region, spr_region_size, audiocpu_region, audio_region_size, fix_region, fix_region_size);
}

// This fixes the Cart Jumper in KOGD
void neogeo_state::main_map1(address_map &map) {
	neogeo_main_map(map);
	map(0x000000,0x00007f).r(FUNC(neogeo_state::banked_vectors_r));
	map(0x000080,0x0fffff).rom();
}

void neogeo_state::hbmame_kog(machine_config &config)
{
	neogeo_arcade(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::main_map1);
	KOG_PROT(config, "kog_prot");

	//joystick controller
	NEOGEO_CTRL_EDGE_CONNECTOR(config, m_edge, neogeo_arc_edge, "joy", true);

	//no mahjong controller
	NEOGEO_CONTROL_PORT(config, "ctrl1", neogeo_arc_pin15, nullptr, true);
	NEOGEO_CONTROL_PORT(config, "ctrl2", neogeo_arc_pin15, nullptr, true);
}

void neogeo_state::init_kogd()
{
	init_neogeo();
	m_kog_prot->kog_install_protection(m_maincpu);
}

void neogeo_state::init_kof10thu()
{
	uint32_t tsize = cpuregion_size;
	uint8_t *rom = cpuregion;
	uint32_t i, ofst;
	std::vector<uint8_t> trom( tsize );
	memcpy( &trom[ 0 ], &rom[ 0 ], tsize );
	// We unscramble the lower 6 address lines of the program rom.
	for( i = 0; i < tsize / 2; i++ )
	{
		ofst = bitswap<8>( (i & 0x0000ff), 7, 6, 2, 3, 4, 5, 0, 1 ) | (i & 0xffff00);
		memcpy( &rom[ ofst * 2 ], &trom[ i * 2 ], 2 );
	}

	init_neogeo();
}

void neogeo_state::init_kof96ep()
{
	init_neogeo();
	int i,j;
	uint8_t *rom = cpuregion;
	for ( i=0; i < 0x080000; i++ )
	{
		j=i+0x300000;
		if (rom[j] - rom[i] == 8) rom[j]=rom[i];
	}
	memcpy(rom, rom+0x300000, 0x080000);
}

void neogeo_state::init_kof97pla()
{
	init_neogeo();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,1);
	// The following was used in Ash build, purpose unknown:
	//m_bootleg_prot->patch_kof97pla(cpuregion, cpuregion_size);
	// This contains:
	//void ngbootleg_prot_device::patch_kof97pla(UINT8* cpurom, UINT32 cpurom_size)
	//cpurom[0x1394A] = 0x6C;
	//cpurom[0x1394B] = 0xD1;
	//cpurom[0x1394C] = 0x3A;
	//cpurom[0x1394D] = 0x1;
	//cpurom[0x1394E] = 0x6C;
	//cpurom[0x1394F] = 0x91;
}

void neogeo_state::init_kf2k1pa()
{
	u32 sx_size = memregion("fixed")->bytes();
	u8 *rom = memregion("fixed")->base();

	/* S-rom has its own unique encryption */
	for( u32 i = 0; i < sx_size; i++ )
		rom[ i ] = bitswap<8>( rom[ i ], 3, 2, 4, 5, 1, 6, 0, 7 );

	init_kof2001();
}

void neogeo_state::init_kof2k2bd()
{
	init_neogeo();
#if 0
	std::vector<u8> src( fix_region_size );
	int j;
	u8 *dst = fix_region;
	memcpy (&src[0], &dst[0], fix_region_size);

	for (u32 i = 1; i < fix_region_size/0x80; i++)
	{
		j = bitswap<16>(i, 15, 14, 13, 12, 11, 10, 9, 7, 6, 2, 5, 4, 3, 1, 0, 8);
		memcpy(&dst[i*0x80], &src[j*0x80], 0x80);
	}
#endif
}

void neogeo_state::init_mp2s39()
{
	init_kof2002();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size, 1);
}

void neogeo_state::init_kof2002s20()
{
	init_kof2002();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size, 2);
}
void neogeo_state::init_kof2k2pl17()
{
	init_neogeo();
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 0);
}

// kf2k2ps2re
// This game can select a different m1 and vx, depending on the character chosen.
// Due to lack of info, this is not emulated. It could happen that bad sounds might
// occur.
#if 0
void neogeo_state::audio_cpu_enable_nmi_w(offs_t offset, u8 data)
{
	// out ($08) enables the nmi, out ($18) disables it
	m_audio_cpu_nmi_enabled = !(offset & 0x10);
	audio_cpu_check_nmi();
}

void neogeo_state::audio_command_w_x(offs_t offset, u8 data)
{
	offs_t which = offset >> 17;printf("%X ",which);
	switch (which)
	{
		case 0:
			m_soundlatch_m2->write(data);
			m_audiocpu_m2->set_input_line(INPUT_LINE_NMI, (m2_nmi_enabled) ? ASSERT_LINE : CLEAR_LINE);
			break;
		case 1:
			m_soundlatch_m3->write(data);
			m_audiocpu_m3->set_input_line(INPUT_LINE_NMI, (m3_nmi_enabled) ? ASSERT_LINE : CLEAR_LINE);
			break;
		case 2:
			m_soundlatch_m4->write(data);
			m_audiocpu_m3->set_input_line(INPUT_LINE_NMI, (m4_nmi_enabled) ? ASSERT_LINE : CLEAR_LINE);
			break;
	}
	m_audiocpu->set_input_line(INPUT_LINE_NMI, (m_audio_cpu_nmi_enabled && m_audio_cpu_nmi_pending) ? ASSERT_LINE : CLEAR_LINE);

	m_audio_cpu_nmi_pending = true;
	audio_cpu_check_nmi();
	/* boost the interleave to let the audio CPU read the command */
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}

void neogeo_state::audio_command_w_m3(u8 data)
{
	m_soundlatch_m3->write(data);
	m_audio_cpu_nmi_pending = true;
	audio_cpu_check_nmi();
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}

void neogeo_state::audio_command_w_m4(u8 data)
{
	m_soundlatch_m4->write( data);
	m_audio_cpu_nmi_pending = true;
	audio_cpu_check_nmi();
	machine().scheduler().boost_interleave(attotime::zero, attotime::from_usec(50));
}


void neogeo_state::ps2_map(address_map &map)
{
	gsc_map(map);
	//map(0x300000, 0x300001).mirror(0x01ff7e).portr("IN0");
	//map(0x340000, 0x340001).mirror(0x01fffe).portr("IN1");
	//map(0x3a0000, 0x3a001f).mirror(0x01ffe0).w(FUNC(neogeo_state::system_control_w_m4));
	//map(0xba0000, 0xba0001).mirror(0x01fffe).rw(FUNC(neogeo_state::get_audio_result_m4),FUNC(neogeo_state::audio_command_w_m4));        // music4 add
	//map(0xbc0000, 0xbc0001).mirror(0x01fffe).rw(FUNC(neogeo_state::get_audio_result_m3),FUNC(neogeo_state::audio_command_w_m3));        // music4 add
	//map(0xbe0000, 0xbe0001).mirror(0x01fffe).rw(FUNC(neogeo_state::get_audio_result_m2),FUNC(neogeo_state::audio_command_w_m2));        // music4 add
	map(0xba0000,0xba0001).w(FUNC(neogeo_state::audio_command_w_m4)).umask16(0xff00);
	map(0xbc0000,0xbc0001).w(FUNC(neogeo_state::audio_command_w_m3)).umask16(0xff00);
	map(0xbe0000,0xbe0001).w(FUNC(neogeo_state::audio_command_w_m2)).umask16(0xff00);
}

void neogeo_state::m2_map(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(neogeo_state::audio_command_r)).w("soundlatch_m2",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd_m2",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(neogeo_state::audio_cpu_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(neogeo_state::audio_cpu_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m2",FUNC(generic_latch_8_device::write));
}

void neogeo_state::m3_map(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(neogeo_state::audio_command_r)).w("soundlatch_m3",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd_m3",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(neogeo_state::audio_cpu_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(neogeo_state::audio_cpu_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m3",FUNC(generic_latch_8_device::write));
}

void neogeo_state::m4_map(address_map &map)
{
	map(0x00,0x00).mirror(0xff00).r(FUNC(neogeo_state::audio_command_r)).w("soundlatch_m4",FUNC(generic_latch_8_device::clear_w));
	map(0x04,0x07).mirror(0xff00).rw("ymsnd_m4",FUNC(ym2610_device::read),FUNC(ym2610_device::write));
	map(0x08,0x08).mirror(0xff00).select(0x0010).w(FUNC(neogeo_state::audio_cpu_enable_nmi_w));
	map(0x08,0x0b).mirror(0x00f0).select(0xff00).r(FUNC(neogeo_state::audio_cpu_bank_select_r));
	map(0x0c,0x0c).mirror(0xff00).w("soundlatch2_m4",FUNC(generic_latch_8_device::write));
}

void neogeo_state::ps2(machine_config &config)
{
	gsc(config);
	m_maincpu->set_addrmap(AS_PROGRAM, &neogeo_state::ps2_map);

	GENERIC_LATCH_8(config, m_soundlatch_m2);
	GENERIC_LATCH_8(config, m_soundlatch2_m2);
	GENERIC_LATCH_8(config, m_soundlatch_m3);
	GENERIC_LATCH_8(config, m_soundlatch2_m3);
	GENERIC_LATCH_8(config, m_soundlatch_m4);
	GENERIC_LATCH_8(config, m_soundlatch2_m4);

	Z80(config, "audiocpu_m2", NEOGEO_AUDIO_CPU_CLOCK);
	m_audiocpu->set_addrmap(AS_PROGRAM, &neogeo_state::audio_map);
	m_audiocpu->set_addrmap(AS_IO, &neogeo_state::m2_map);
	Z80(config, "audiocpu_m3", NEOGEO_AUDIO_CPU_CLOCK);
	m_audiocpu->set_addrmap(AS_PROGRAM, &neogeo_state::audio_map);
	m_audiocpu->set_addrmap(AS_IO, &neogeo_state::m3_map);
	Z80(config, "audiocpu_m4", NEOGEO_AUDIO_CPU_CLOCK);
	m_audiocpu->set_addrmap(AS_PROGRAM, &neogeo_state::audio_map);
	m_audiocpu->set_addrmap(AS_IO, &neogeo_state::m4_map);
}
#endif

void neogeo_state::init_kof2k3hd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_pvc_prot->kof2003h_decrypt_68k(cpuregion, cpuregion_size);  // different to kof2k3d
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_kof2k3pcd() // decrypted C & decrypted Bios
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_pvc_prot->kf2k3pcb_decrypt_68k(cpuregion, cpuregion_size);
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// Extra m1 decryption
	uint8_t* rom = memregion("audiocpu")->base();
	for (int i = 0; i < 0x90000; i++)
		rom[i] = bitswap<8>(rom[i], 5, 6, 1, 4, 3, 0, 7, 2);

	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 5);
	m_sma_prot->kf2k3pcb_decrypt_s1data(spr_region, spr_region_size, fix_region, fix_region_size);
	m_maincpu->space(AS_PROGRAM).install_rom(0xc00000, 0xc7ffff, 0x080000, memregion("mainbios")->base());  // 512k bios
}

void neogeo_state::init_kof2k3fd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_sma_prot->kf2k3pcb_sp1_decrypt((uint16_t*)memregion("mainbios")->base());
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	m_maincpu->space(AS_PROGRAM).install_rom(0xc00000, 0xc7ffff, 0x080000, memregion("mainbios")->base());  // 512k bios
}

// A3 is inverted in s1
void neogeo_state::init_xs02()
{
	for (u32 i = 0; i < fix_region_size; i+=0x10)
		for (u8 j = 0; j < 8; j++)
		{
			u8 k = fix_region[i+j];
			fix_region[i+j] = fix_region[i+j+8];
			fix_region[i+j+8] = k;
		}
	init_kof2003b();
}

void neogeo_state::init_matrima()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MATRIM_GFX_KEY);
}

void neogeo_state::init_matrimd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_kof2002_prot->matrim_decrypt_68k(cpuregion, cpuregion_size);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 1);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_matrmehc()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_kof2002_prot->matrim_decrypt_68k(cpuregion, cpuregion_size);
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MATRIM_GFX_KEY);
	m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 1);
}

#if 0
void neogeo_state::init_mslug3()
{
	init_neogeo();
	m_sma_prot->mslug3_decrypt_68k(cpuregion);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG3_GFX_KEY);
	m_sma_prot->mslug3_install_protection(m_maincpu, m_banked_cart);
}
#endif

void neogeo_state::init_mslug3n()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc42_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG3_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_mslug3d()
{
	init_neogeo();
	m_sma_prot->mslug3_decrypt_68k(cpuregion);
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_sma_prot->mslug3_install_protection(m_maincpu, m_banked_cart);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_mslug3fr()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_sma_prot->mslug3_install_protection(m_maincpu, m_banked_cart);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_ms6s16()
{
	init_neogeo();
	m_bootleg_prot->neogeo_bootleg_sx_decrypt(fix_region, fix_region_size,2);
}

void neogeo_state::init_mslug4hb()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	u8 *ram = memregion("ymsnd:adpcma")->base();
	if (ram[0x20] != 0x99)
	{
		//printf("ym=%X\n",ram[0x20]);
		m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 8);
	}

	// decrypt c roms if needed
	ram = memregion("sprites")->base();
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG4_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = memregion("fixed")->base();
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_mslug5hb() 
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 2; // for those sets with 512k of s1

	// decrypt p roms if needed
	u8 *ram = memregion("maincpu")->base();
	if (ram[0x100] != 0x45)
	{
		//printf("Maincpu=%X\n",ram[0x100]);fflush(stdout);
		m_pvc_prot->mslug5_decrypt_68k(cpuregion, cpuregion_size);
		m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = memregion("ymsnd:adpcma")->base();
	if (ram[0x60] != 0x82)
	{
		//printf("ym=%X\n",ram[0x60]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 2);
	}

	// decrypt c roms if needed
	ram = memregion("sprites")->base();
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, MSLUG5_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = memregion("fixed")->base();
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_mslug5b1()
{
	init_mslug5hb();
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
}

void neogeo_state::init_ms5pcbd()
{
	init_neogeo();

	m_pvc_prot->mslug5_decrypt_68k(cpuregion, cpuregion_size);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_sprgen->m_fixed_layer_bank_type = 2;
	m_sma_prot->svcpcb_s1data_decrypt(fix_region, fix_region_size);
	m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	install_banked_bios();
}

void neogeo_state::init_pnyaad()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 4);
	m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region,audio_region_size);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_shockt2w()
{
	init_neogeo();
	uint16_t *mem16= (uint16_t *)memregion("mainbios")->base();
	mem16[0x0200] = 1;  /* Force home mode */
	/* Above causes checksum failure, so patch it out */
	mem16[0x11c62/2] = 0x4e71;
	mem16[0x11c64/2] = 0x4e71;
}

void neogeo_state::init_sam5hb()
{
	init_neogeo();
	// This is needed if s1 size > 0x20000, ignored otherwise
	m_sprgen->m_fixed_layer_bank_type = 1;

	// decrypt p roms if needed
	u8 *ram = memregion("maincpu")->base();
	if (ram[0x100007] != 0xC2)
	{
		//printf("Maincpu=%X\n",ram[0x100007]);fflush(stdout);
		m_kof2002_prot->samsho5_decrypt_68k(cpuregion, cpuregion_size);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = memregion("ymsnd:adpcma")->base();
	if (ram[0] != 8)
	{
		//printf("ym=%X\n",ram[0]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 4);
	}

	// decrypt c roms if needed
	ram = memregion("sprites")->base();
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SAMSHO5_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = memregion("fixed")->base();
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_sam5sphb2() // for samsh5spds11
{
	// decrypt c roms
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SAMSHO5SP_GFX_KEY);
	init_sam5sphb();
}

void neogeo_state::init_sam5sphb()
{
	init_neogeo();
	// This is needed if s1 size > 0x20000, ignored otherwise
	m_sprgen->m_fixed_layer_bank_type = 1;

	// decrypt p roms if needed
	u8 *ram = memregion("maincpu")->base();
	if (ram[0x100007] == 0)
	{
		//printf("Maincpu=%X\n",ram[0x100007]);fflush(stdout);
		m_kof2002_prot->samsh5sp_decrypt_68k(cpuregion, cpuregion_size);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = memregion("ymsnd:adpcma")->base();
	if (ram[0] != 8)
	{
		//printf("ym=%X\n",ram[0]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 6);
	}

	// if no s rom, copy info from end of c roms
	ram = memregion("fixed")->base();
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}
}

void neogeo_state::init_sengo3d()
{
	init_neogeo();
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

void neogeo_state::init_svchb()
{
	init_neogeo();
	// This is needed if s1 size > 0x20000, ignored otherwise
	m_sprgen->m_fixed_layer_bank_type = 2;

	// decrypt p roms if needed
	u8 *ram = memregion("maincpu")->base();
	if (ram[0x100] != 0x45)
	{
		//printf("Maincpu=%X\n",ram[0x100]);fflush(stdout);
		m_pvc_prot->svc_px_decrypt(cpuregion, cpuregion_size);
		m_pvc_prot->install_pvc_protection(m_maincpu, m_banked_cart);
	}

	// decrypt m1 if needed
	if (memregion("audiocrypt"))
		m_cmc_prot->neogeo_cmc50_m1_decrypt(audiocrypt_region, audiocrypt_region_size, audiocpu_region, audio_region_size);

	// decrypt v roms if needed
	ram = memregion("ymsnd:adpcma")->base();
	if (ram[0x91] != 0x33)
	{
		//printf("ym=%X\n",ram[0x91]);
		m_pcm2_prot->neo_pcm2_swap(ym_region, ym_region_size, 3);
	}

	// decrypt c roms if needed
	ram = memregion("sprites")->base();
	if (ram[0] != 0)
	{
		//printf("Sprites=%X\n",ram[0]);
		m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, SVC_GFX_KEY);
	}

	// if no s rom, copy info from end of c roms
	ram = memregion("fixed")->base();
	if (ram[0x100] == 0)
	{
		//printf("Fixed1=%X\n",ram[0x100]);
		m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	}

	// decrypt s1 if needed
	if (ram[0x100] != 0xBB)
	{
		//printf("Fixed2=%X\n",ram[0]);
		m_sma_prot->svcpcb_s1data_decrypt(fix_region, fix_region_size);
	}
}

void neogeo_state::init_svcpcd()
{
	init_svchb();
	install_banked_bios();
}

void neogeo_state::init_rotdb()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->cmc50_neogeo_gfx_decrypt(spr_region, spr_region_size, ROTD_GFX_KEY);
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 16);
}

void neogeo_state::init_rotdd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
	m_pcm2_prot->neo_pcm2_snk_1999(ym_region, ym_region_size, 16);
}

void neogeo_state::init_rotdnd()
{
	init_neogeo();
	m_sprgen->m_fixed_layer_bank_type = 1;
	m_cmc_prot->neogeo_sfix_decrypt(spr_region, spr_region_size, fix_region, fix_region_size);
}

#define MSLUG_MAINCPU \
    ROM_FILL(0x1783E7,1,0x78)\
	ROM_FILL(0x1783EB,1,0x12)\
	ROM_FILL(0x1783E8,1,0x34)\
	ROM_FILL(0x1783EE,1,0x24)
	//ROM_FILL(0x1783ED,1,0x34)

#define MSLUGRM_MAINCPU \
    ROM_FILL(0x1783E7,1,0x78)\
	ROM_FILL(0x1783EB,1,0x12)\
	ROM_FILL(0x1783E8,1,0x34)\
	ROM_FILL(0x1783EE,1,0x24)\
	ROM_FILL(0x1783E4,1,0x1A)
	
#define MSLUGRMXE_MAINCPU \
    ROM_FILL(0x1783E7,1,0x78)\
	ROM_FILL(0x1783EB,1,0x12)\
	ROM_FILL(0x1783E8,1,0x34)\
	ROM_FILL(0x1783EE,1,0x24)\
	ROM_FILL(0x1783E4,1,0x98)

#define MSLUG2_MAINCPU \
    ROM_FILL(0x100D,1,0x78)\
	ROM_FILL(0x1011,1,0x12)\
	ROM_FILL(0x1013,1,0x34)\
	ROM_FILL(0x100E,1,0x34)\
	ROM_FILL(0x1014,1,0x34)

#define MSLUG2RM_MAINCPU \
    ROM_FILL(0x100D,1,0x78)\
	ROM_FILL(0x1011,1,0x12)\
	ROM_FILL(0x1013,1,0x34)\
	ROM_FILL(0x100E,1,0x34)\
	ROM_FILL(0x1014,1,0x34)\
    ROM_FILL(0x100A,1,0x1A)

#define MSLUG2RMXE_MAINCPU \
    ROM_FILL(0x100D,1,0x78)\
	ROM_FILL(0x1011,1,0x12)\
	ROM_FILL(0x1013,1,0x34)\
	ROM_FILL(0x100E,1,0x34)\
	ROM_FILL(0x1014,1,0x34)\
    ROM_FILL(0x100A,1,0x98)

#define MSLUG3H_MAINCPU \
    ROM_FILL(0x2EA7,1,0x78)\
	ROM_FILL(0x2EAE,1,0x12)\
	ROM_FILL(0x2EAF,1,0x12)\
	ROM_FILL(0x2EAB,1,0x34)\
	ROM_FILL(0x2EA8,1,0x34)\
	ROM_FILL(0x2EAD,1,0x24)

#define MSLUG3RM_MAINCPU \
    ROM_FILL(0x2EA7,1,0x78)\
	ROM_FILL(0x2EAE,1,0x12)\
	ROM_FILL(0x2EAF,1,0x12)\
	ROM_FILL(0x2EAB,1,0x34)\
	ROM_FILL(0x2EA8,1,0x34)\
	ROM_FILL(0x2EAD,1,0x24)\
	ROM_FILL(0x2EA4,1,0x1A)

#define MSLUG3RMXE_MAINCPU \
    ROM_FILL(0x2EA7,1,0x78)\
	ROM_FILL(0x2EAE,1,0x12)\
	ROM_FILL(0x2EAF,1,0x12)\
	ROM_FILL(0x2EAB,1,0x34)\
	ROM_FILL(0x2EA8,1,0x34)\
	ROM_FILL(0x2EAD,1,0x24)\
	ROM_FILL(0x2EA4,1,0x98)

#define MSLUG4_MAINCPU \
    ROM_FILL(0x1741,1,0x78)\
	ROM_FILL(0x1748,1,0x12)\
	ROM_FILL(0x1749,1,0x12)\
	ROM_FILL(0x1745,1,0x34)\
	ROM_FILL(0x1742,1,0x34)\
	ROM_FILL(0x1747,1,0x24)

#define MSLUG4RM_MAINCPU \
    ROM_FILL(0x1741,1,0x78)\
	ROM_FILL(0x1748,1,0x12)\
	ROM_FILL(0x1749,1,0x12)\
	ROM_FILL(0x1745,1,0x34)\
	ROM_FILL(0x1742,1,0x34)\
	ROM_FILL(0x1747,1,0x24)\
	ROM_FILL(0x173E,1,0x1A)

#define MSLUG4RMXE_MAINCPU \
    ROM_FILL(0x1741,1,0x78)\
	ROM_FILL(0x1748,1,0x12)\
	ROM_FILL(0x1749,1,0x12)\
	ROM_FILL(0x1745,1,0x34)\
	ROM_FILL(0x1742,1,0x34)\
	ROM_FILL(0x1747,1,0x24)\
	ROM_FILL(0x173E,1,0x98)

#define MSLUG5H_MAINCPU \
    ROM_FILL(0x2CD3,1,0x78)\
	ROM_FILL(0x2CDA,1,0x12)\
	ROM_FILL(0x2CDB,1,0x12)\
	ROM_FILL(0x2CD7,1,0x34)\
	ROM_FILL(0x2CD4,1,0x34)\
	ROM_FILL(0x2CD9,1,0x24)\

#define MSLUG5RM_MAINCPU \
    ROM_FILL(0x2CD3,1,0x78)\
	ROM_FILL(0x2CDA,1,0x12)\
	ROM_FILL(0x2CDB,1,0x12)\
	ROM_FILL(0x2CD7,1,0x34)\
	ROM_FILL(0x2CD4,1,0x34)\
	ROM_FILL(0x2CD9,1,0x24)\
	ROM_FILL(0x2CD0,1,0x1A)

#define MSLUG5RMXE_MAINCPU \
    ROM_FILL(0x2CD3,1,0x78)\
	ROM_FILL(0x2CDA,1,0x12)\
	ROM_FILL(0x2CDB,1,0x12)\
	ROM_FILL(0x2CD7,1,0x34)\
	ROM_FILL(0x2CD4,1,0x34)\
	ROM_FILL(0x2CD9,1,0x24)\
	ROM_FILL(0x2CD0,1,0x98)

#define MSLUGX_MAINCPU \
    ROM_FILL(0x3185,1,0x78)\
	ROM_FILL(0x3189,1,0x12)\
	ROM_FILL(0x318D,1,0x12)\
	ROM_FILL(0x318B,1,0x34)\
	ROM_FILL(0x3186,1,0x34)\
	ROM_FILL(0x318C,1,0x34)

#define MSLUGXRM_MAINCPU \
    ROM_FILL(0x3185,1,0x78)\
	ROM_FILL(0x3189,1,0x12)\
	ROM_FILL(0x318D,1,0x12)\
	ROM_FILL(0x318B,1,0x34)\
	ROM_FILL(0x3186,1,0x34)\
	ROM_FILL(0x318C,1,0x34)\
    ROM_FILL(0x3182,1,0x18)

#define MSLUGXRMXE_MAINCPU \
    ROM_FILL(0x3185,1,0x78)\
	ROM_FILL(0x3189,1,0x12)\
	ROM_FILL(0x318D,1,0x12)\
	ROM_FILL(0x318B,1,0x34)\
	ROM_FILL(0x3186,1,0x34)\
	ROM_FILL(0x318C,1,0x34)\
    ROM_FILL(0x3182,1,0x98)

#define MSLUG_YMSND \
	ROM_LOAD( "201.v1", 0x000000, 0x400000, CRC(23d22ed1) SHA1(cd076928468ad6bcc5f19f88cb843ecb5e660681) )\
	ROM_LOAD( "201.v2", 0x400000, 0x400000, CRC(472cf9db) SHA1(5f79ea9286d22ed208128f9c31ca75552ce08b57) )

#define MSLUG_SPRITES \
	ROM_LOAD16_BYTE( "201.c1", 0x000000, 0x400000, CRC(72813676) SHA1(7b045d1a48980cb1a140699011cb1a3d4acdc4d1) )\
	ROM_LOAD16_BYTE( "201.c2", 0x000001, 0x400000, CRC(96f62574) SHA1(cb7254b885989223bba597b8ff0972dfa5957816) )\
	ROM_LOAD16_BYTE( "201.c3", 0x800000, 0x400000, CRC(5121456a) SHA1(0a7a27d603d1bb2520b5570ebf5b34a106e255a6) )\
	ROM_LOAD16_BYTE( "201.c4", 0x800001, 0x400000, CRC(f4ad59a3) SHA1(4e94fda8ee63abf0f92afe08060a488546e5c280) )

#define MSLUGB_YMSND \
	ROM_LOAD( "201boot.v1", 0x000000, 0x200000, CRC(2C05FD7C) SHA1(4f28bc799da0e27b2f3cde364d8c9822468abb8f) )\
	ROM_LOAD( "201boot.v2", 0x200000, 0x200000, CRC(752DE272) SHA1(1e9ffb296a1089e15b23455841ad91e8a565ba62) )\
	ROM_LOAD( "201boot.v3", 0x400000, 0x200000, CRC(AA48FAA6) SHA1(4695b9dafdc13997410cbd7954c2a7f1b5e4649d) )\
	ROM_LOAD( "201boot.v4", 0x600000, 0x200000, CRC(4264444B) SHA1(7c248e8e559c67d6e8474ec6ce5cf2e3e41235e4) )

#define MSLUGE_SPRITES \
	ROM_LOAD16_BYTE( "201e.c1", 0x400000, 0x200000, CRC(d00bd152) SHA1(eb688dba2233bece1c3ba120ac8eb342f37fba37) )\
	ROM_CONTINUE(         0x000000, 0x200000 )\
	ROM_LOAD16_BYTE( "201e.c2", 0x400001, 0x200000, CRC(ddff1dea) SHA1(e6ac8950d8ad8498270097a248c4b49876804197) )\
	ROM_CONTINUE(         0x000001, 0x200000 )\
	ROM_LOAD16_BYTE( "201e.c3", 0xc00000, 0x200000, CRC(d3d5f9e5) SHA1(7d259314c2198ee81a380d76728c3c1ac2c8b528) )\
	ROM_CONTINUE(         0x800000, 0x200000 )\
	ROM_LOAD16_BYTE( "201e.c4", 0xc00001, 0x200000, CRC(5ac1d497) SHA1(313249ea47b3553974cde1c4c36f1ff3adeb07d1) )\
	ROM_CONTINUE(         0x800001, 0x200000 )

#define MSLUG2_YMSND \
	ROM_LOAD( "241.v1", 0x000000, 0x400000, CRC(99ec20e8) SHA1(80597707f1fe115eed1941bb0701fc00790ad504) )\
	ROM_LOAD( "241.v2", 0x400000, 0x400000, CRC(ecb16799) SHA1(b4b4ddc680836ed55942c66d7dfe756314e02211) )

#define MSLUG2_SPRITES \
	ROM_LOAD16_BYTE( "241.c1", 0x0000000, 0x800000, CRC(394b5e0d) SHA1(4549926f5054ee6aa7689cf920be0327e3908a50) )\
	ROM_LOAD16_BYTE( "241.c2", 0x0000001, 0x800000, CRC(e5806221) SHA1(1e5475cfab129c77acc610f09369ca42ba5aafa5) )\
	ROM_LOAD16_BYTE( "241.c3", 0x1000000, 0x800000, CRC(9f6bfa6f) SHA1(a4319b48004e723f81a980887678e3e296049a53) )\
	ROM_LOAD16_BYTE( "241.c4", 0x1000001, 0x800000, CRC(7d3e306f) SHA1(1499316fb381775218d897b81a6a0c3465d1a37c) )

#define MSLUG2FR_SPRITES \
	ROM_LOAD16_BYTE( "241_hacks04.c1", 0x0000000, 0x800000, CRC(7df74035) SHA1(08322a2f4e87cd125ebc5b7a98754c7a5b8e91d2) )\
	ROM_LOAD16_BYTE( "241_hacks04.c2", 0x0000001, 0x800000, CRC(97d9525d) SHA1(451a5bdafe1f2de63b76efb7d4afab454abdfff0) )\
	ROM_LOAD16_BYTE( "241.c3", 0x1000000, 0x800000, CRC(9f6bfa6f) SHA1(a4319b48004e723f81a980887678e3e296049a53) )\
	ROM_LOAD16_BYTE( "241.c4", 0x1000001, 0x800000, CRC(7d3e306f) SHA1(1499316fb381775218d897b81a6a0c3465d1a37c) )

#define MSLUG2DD_SPRITES \
	ROM_LOAD16_BYTE( "241.c1", 0x0000000, 0x800000, CRC(394b5e0d) SHA1(4549926f5054ee6aa7689cf920be0327e3908a50) )\
	ROM_LOAD16_BYTE( "241.c2", 0x0000001, 0x800000, CRC(e5806221) SHA1(1e5475cfab129c77acc610f09369ca42ba5aafa5) )\
	ROM_LOAD16_BYTE( "241_hacks21.c3", 0x1000000, 0x800000, CRC(c54c2548) SHA1(07045a7a004631b54ac35e02075ec8f6898a4b32) )\
	ROM_LOAD16_BYTE( "241_hacks21.c4", 0x1000001, 0x800000, CRC(f8168521) SHA1(3bb16caeced227c8bee95bfb92cd22f171ac86d3) )

#define MSLUG2EG_SPRITES \
	ROM_LOAD16_BYTE( "241.c1", 0x0000000, 0x800000, CRC(394b5e0d) SHA1(4549926f5054ee6aa7689cf920be0327e3908a50) )\
	ROM_LOAD16_BYTE( "241.c2", 0x0000001, 0x800000, CRC(e5806221) SHA1(1e5475cfab129c77acc610f09369ca42ba5aafa5) )\
	ROM_LOAD16_BYTE( "241.c3", 0x1000000, 0x800000, CRC(9f6bfa6f) SHA1(a4319b48004e723f81a980887678e3e296049a53) )\
	ROM_LOAD16_BYTE( "241.c4", 0x1000001, 0x800000, CRC(7d3e306f) SHA1(1499316fb381775218d897b81a6a0c3465d1a37c) )\
	ROM_LOAD16_BYTE( "241_hacks22.c5", 0x2000000, 0x800000, CRC(cae9af4f) SHA1(85f32cdc8069901f2f5fb628a2eda5e44f7994f7) )\
	ROM_LOAD16_BYTE( "241_hacks22.c6", 0x2000001, 0x800000, CRC(7cf51055) SHA1(ec5e78746190fc2bf087fa1b8efd9fc3433068d7) )


#define MSLUG3_YMSND \
	ROM_LOAD( "256.v1", 0x000000, 0x400000, CRC(f2690241) SHA1(fd56babc1934d10e0d27c32f032f9edda7ca8ce9) )\
	ROM_LOAD( "256.v2", 0x400000, 0x400000, CRC(7e2a10bd) SHA1(0d587fb9f64cba0315ce2d8a03e2b8fe34936dff) )\
	ROM_LOAD( "256.v3", 0x800000, 0x400000, CRC(0eaec17c) SHA1(c3ed613cc6993edd6fc0d62a90bcd85de8e21915) )\
	ROM_LOAD( "256.v4", 0xc00000, 0x400000, CRC(9b4b22d4) SHA1(9764fbf8453e52f80aa97a46fb9cf5937ef15a31) )

#define MSLUG3_SPRITES \
	ROM_LOAD16_BYTE( "256.c1", 0x0000000, 0x800000, CRC(5a79c34e) SHA1(b8aa51fa50935cae62ab3d125b723ab888691e60) )\
	ROM_LOAD16_BYTE( "256.c2", 0x0000001, 0x800000, CRC(944c362c) SHA1(3843ab300f956280475469caee70135658f67089) )\
	ROM_LOAD16_BYTE( "256.c3", 0x1000000, 0x800000, CRC(6e69d36f) SHA1(94e8cf42e999114b4bd8b30e0aa2f365578c4c9a) )\
	ROM_LOAD16_BYTE( "256.c4", 0x1000001, 0x800000, CRC(b755b4eb) SHA1(804700a0966a48f130c434ede3f970792ea74fa5) )\
	ROM_LOAD16_BYTE( "256.c5", 0x2000000, 0x800000, CRC(7aacab47) SHA1(312c1c9846175fe1a3cad51d5ae230cf674fc93d) )\
	ROM_LOAD16_BYTE( "256.c6", 0x2000001, 0x800000, CRC(c698fd5d) SHA1(16818883b06849ba2f8d61bdd5e21aaf99bd8408) )\
	ROM_LOAD16_BYTE( "256.c7", 0x3000000, 0x800000, CRC(cfceddd2) SHA1(7def666adf8bd1703f40c61f182fc040b6362dc9) )\
	ROM_LOAD16_BYTE( "256.c8", 0x3000001, 0x800000, CRC(4d9be34c) SHA1(a737bdfa2b815aea7067e7af2636e83a9409c414) )

#define MSLUG3GW_YMSND \
	ROM_LOAD( "256.v1", 0x000000, 0x400000, CRC(f2690241) SHA1(fd56babc1934d10e0d27c32f032f9edda7ca8ce9) )\
	ROM_LOAD( "256.v2", 0x400000, 0x400000, CRC(7e2a10bd) SHA1(0d587fb9f64cba0315ce2d8a03e2b8fe34936dff) )\
	ROM_LOAD( "256.v3", 0x800000, 0x400000, CRC(0eaec17c) SHA1(c3ed613cc6993edd6fc0d62a90bcd85de8e21915) )\
	ROM_LOAD( "256_hacks17.v4", 0xc00000, 0x400000, CRC(b45d9a3b) SHA1(eee342e11865facbbfcaf82b8e16b9e64b8c309c) )

#define MSLUG3D_SPRITES \
	ROM_LOAD16_BYTE( "256d.c1", 0x0000000, 0x800000, CRC(3540398c) SHA1(0a96f2360eb26c66bf02bbc6f52230b55cc95e4e) )\
	ROM_LOAD16_BYTE( "256d.c2", 0x0000001, 0x800000, CRC(bdd220f0) SHA1(f52851023f3bc120b05f622af0e0ab1bedc41604) )\
	ROM_LOAD16_BYTE( "256d.c3", 0x1000000, 0x800000, CRC(bfaade82) SHA1(66b07e592c9a9b35567fe463496f8f75c32a7db9) )\
	ROM_LOAD16_BYTE( "256d.c4", 0x1000001, 0x800000, CRC(1463add6) SHA1(4db91b46d6430da272d27d00a6dc0eb25949bea1) )\
	ROM_LOAD16_BYTE( "256d.c5", 0x2000000, 0x800000, CRC(48ca7f28) SHA1(e903876be5fb4fa582c988d74c6bef1c3b9c7083) )\
	ROM_LOAD16_BYTE( "256d.c6", 0x2000001, 0x800000, CRC(806eb36f) SHA1(a412a9cab80c326733dde7652d1db2a46afb3ebb) )\
	ROM_LOAD16_BYTE( "256d.c7", 0x3000000, 0x800000, CRC(9395b809) SHA1(ca9ac9832017094eee3623f0b6c4c4b7b4f1374d) )\
	ROM_LOAD16_BYTE( "256d.c8", 0x3000001, 0x800000, CRC(a369f9d4) SHA1(f8146ea80a1a23da7e7e04c88f778ee9abdfeb5c) )

#define MSLUG3FR_SPRITES \
	ROM_LOAD16_BYTE( "256_hacks08.c1", 0x0000000, 0x800000, CRC(d93a9a86) SHA1(4ff06026449c80ef8effc14f241590fef83006c9) )\
	ROM_LOAD16_BYTE( "256_hacks08.c2", 0x0000001, 0x800000, CRC(5773183c) SHA1(a09783bcaa2c071d4358e00148551e5c60e48753) )\
	ROM_LOAD16_BYTE( "256_hacks08.c3", 0x1000000, 0x800000, CRC(9bbc4d38) SHA1(65f34c09e58cfb59c45df2d14e74408d7d571b3d) )\
	ROM_LOAD16_BYTE( "256_hacks08.c4", 0x1000001, 0x800000, CRC(871eb92f) SHA1(5146975de14eb31f4df959c71659ab4cf2dbd193) )\
	ROM_LOAD16_BYTE( "256_hacks08.c5", 0x2000000, 0x800000, CRC(35f9d8d6) SHA1(da046812d9b4047e0ce84bda5e77c19cde5fce56) )\
	ROM_LOAD16_BYTE( "256_hacks08.c6", 0x2000001, 0x800000, CRC(7deeb565) SHA1(79c83d2b1887ec776fc0c24527405d386607342d) )\
	ROM_LOAD16_BYTE( "256_hacks08.c7", 0x3000000, 0x800000, CRC(a6785325) SHA1(e3377727c057dfd3e3bf91ad7a02217244335bb5) )\
	ROM_LOAD16_BYTE( "256_hacks08.c8", 0x3000001, 0x800000, CRC(b1a3a35d) SHA1(d7c41734e745445efcb33465eb7b26a88ce5a341) )

#define MSLUG3G_SPRITES \
	ROM_LOAD16_BYTE( "256d.c1", 0x0000000, 0x800000, CRC(3540398c) SHA1(0a96f2360eb26c66bf02bbc6f52230b55cc95e4e) )\
	ROM_LOAD16_BYTE( "256d.c2", 0x0000001, 0x800000, CRC(bdd220f0) SHA1(f52851023f3bc120b05f622af0e0ab1bedc41604) )\
	ROM_LOAD16_BYTE( "256d.c3", 0x1000000, 0x800000, CRC(bfaade82) SHA1(66b07e592c9a9b35567fe463496f8f75c32a7db9) )\
	ROM_LOAD16_BYTE( "256d.c4", 0x1000001, 0x800000, CRC(1463add6) SHA1(4db91b46d6430da272d27d00a6dc0eb25949bea1) )\
	ROM_LOAD16_BYTE( "256d.c5", 0x2000000, 0x800000, CRC(48ca7f28) SHA1(e903876be5fb4fa582c988d74c6bef1c3b9c7083) )\
	ROM_LOAD16_BYTE( "256d.c6", 0x2000001, 0x800000, CRC(806eb36f) SHA1(a412a9cab80c326733dde7652d1db2a46afb3ebb) )\
	ROM_LOAD16_BYTE( "256_hacks20.c7", 0x3000000, 0x800000, CRC(ed559fac) SHA1(e2e73b068785226cd97abbb8c2bd2b5800e9bf19) )\
	ROM_LOAD16_BYTE( "256_hacks20.c8", 0x3000001, 0x800000, CRC(1c52378b) SHA1(05cd499f5444eae6360c96bfd3d3db8870d9656a) )

#define MSLUG3DD_SPRITES \
	ROM_LOAD16_BYTE( "256d.c1", 0x0000000, 0x800000, CRC(3540398c) SHA1(0a96f2360eb26c66bf02bbc6f52230b55cc95e4e) )\
	ROM_LOAD16_BYTE( "256d.c2", 0x0000001, 0x800000, CRC(bdd220f0) SHA1(f52851023f3bc120b05f622af0e0ab1bedc41604) )\
	ROM_LOAD16_BYTE( "256d.c3", 0x1000000, 0x800000, CRC(bfaade82) SHA1(66b07e592c9a9b35567fe463496f8f75c32a7db9) )\
	ROM_LOAD16_BYTE( "256d.c4", 0x1000001, 0x800000, CRC(1463add6) SHA1(4db91b46d6430da272d27d00a6dc0eb25949bea1) )\
	ROM_LOAD16_BYTE( "256d.c5", 0x2000000, 0x800000, CRC(48ca7f28) SHA1(e903876be5fb4fa582c988d74c6bef1c3b9c7083) )\
	ROM_LOAD16_BYTE( "256d.c6", 0x2000001, 0x800000, CRC(806eb36f) SHA1(a412a9cab80c326733dde7652d1db2a46afb3ebb) )\
	ROM_LOAD16_BYTE( "256_hacks35.c7", 0x3000000, 0x800000, CRC(ec74423d) SHA1(af76e917219e46102fe4c0231fbbdf96f5c78fe7) )\
	ROM_LOAD16_BYTE( "256_hacks35.c8", 0x3000001, 0x800000, CRC(a9fe9b98) SHA1(165bf118ffde29c6434a6b3026e0b111889082af) )

#define MSLUG3N6P_SPRITES \
	ROM_LOAD16_BYTE( "256d.c1", 0x0000000, 0x800000, CRC(3540398c) SHA1(0a96f2360eb26c66bf02bbc6f52230b55cc95e4e) )\
	ROM_LOAD16_BYTE( "256d.c2", 0x0000001, 0x800000, CRC(bdd220f0) SHA1(f52851023f3bc120b05f622af0e0ab1bedc41604) )\
	ROM_LOAD16_BYTE( "256d.c3", 0x1000000, 0x800000, CRC(bfaade82) SHA1(66b07e592c9a9b35567fe463496f8f75c32a7db9) )\
	ROM_LOAD16_BYTE( "256d.c4", 0x1000001, 0x800000, CRC(1463add6) SHA1(4db91b46d6430da272d27d00a6dc0eb25949bea1) )\
	ROM_LOAD16_BYTE( "256d.c5", 0x2000000, 0x800000, CRC(48ca7f28) SHA1(e903876be5fb4fa582c988d74c6bef1c3b9c7083) )\
	ROM_LOAD16_BYTE( "256d.c6", 0x2000001, 0x800000, CRC(806eb36f) SHA1(a412a9cab80c326733dde7652d1db2a46afb3ebb) )\
	ROM_LOAD16_BYTE( "299_hacks01.c7", 0x3000000, 0x800000, CRC(f53108fe) SHA1(ee5a60410221c9e5139cc5bcffe19bb99f4745bc) )\
	ROM_LOAD16_BYTE( "299_hacks01.c8", 0x3000001, 0x800000, CRC(64c33010) SHA1(6b1ef285228720fd828cf317ebfc17cace0bc5cd) )

#define MSLUG4_YMSND \
	ROM_LOAD( "263.v1", 0x000000, 0x800000, CRC(01e9b9cd) SHA1(0b045c2999449f7dab5ae8a42e957d5b6650431e) )\
	ROM_LOAD( "263.v2", 0x800000, 0x800000, CRC(4ab2bf81) SHA1(77ccfa48f7e3daddef5fe5229a0093eb2f803742) )
	
#define MSLUG4_SPRITES \
 	ROM_LOAD16_BYTE( "263.c1", 0x0000000, 0x800000, CRC(84865f8a) SHA1(34467ada896eb7c7ca58658bf2a932936d8b632c) )\
	ROM_LOAD16_BYTE( "263.c2", 0x0000001, 0x800000, CRC(81df97f2) SHA1(2b74493b8ec8fd49216a627aeb3db493f76124e3) )\
	ROM_LOAD16_BYTE( "263.c3", 0x1000000, 0x800000, CRC(1a343323) SHA1(bbbb5232bba538c277ce2ee02e2956ca2243b787) )\
	ROM_LOAD16_BYTE( "263.c4", 0x1000001, 0x800000, CRC(942cfb44) SHA1(d9b46c71726383c4581fb042e63897e5a3c92d1b) )\
	ROM_LOAD16_BYTE( "263.c5", 0x2000000, 0x800000, CRC(a748854f) SHA1(2611bbedf9b5d8e82c6b2c99b88f842c46434d41) )\
	ROM_LOAD16_BYTE( "263.c6", 0x2000001, 0x800000, CRC(5c8ba116) SHA1(6034db09c8706d4ddbcefc053efbc47a0953eb92) )

#define MSLUG4ND_YMSND \
	ROM_LOAD( "263nd.v1", 0x000000, 0x400000, CRC(c1b2af81) SHA1(532dca0a2b2e03d50c6ac3adc9ebceb413f61690) )\
	ROM_LOAD( "263nd.v2", 0x400000, 0x400000, CRC(cbbdc6fa) SHA1(06abb738cf198804248ecfc92adea38f7f86f612) )\
	ROM_LOAD( "263nd.v3", 0x800000, 0x400000, CRC(c4f1a3cc) SHA1(109e8db93875e1b1f3db293d1756b2bbebe63a28) )\
	ROM_LOAD( "263nd.v4", 0xC00000, 0x400000, CRC(40126a0a) SHA1(4234e0f4d9a9dc486d7af1eec91a17840a57976d) )

#define MSLUG4ND_SPRITES \
	ROM_LOAD16_BYTE( "263nd.c1", 0x0000000, 0x800000, CRC(361ddda6) SHA1(95f329019660f1ec7f15f6ea1e0c2bfc84b02ca8) )\
	ROM_LOAD16_BYTE( "263nd.c2", 0x0000001, 0x800000, CRC(15d192c4) SHA1(0b9afb62a1d4d764d616408b09e17a05fc4a87e7) )\
	ROM_LOAD16_BYTE( "263nd.c3", 0x1000000, 0x800000, CRC(63fb79ff) SHA1(a7da0a554034792721abda4369a71b0c8045f334) )\
	ROM_LOAD16_BYTE( "263nd.c4", 0x1000001, 0x800000, CRC(e8623126) SHA1(66c2b874cd55c628e7e6768d6f31180c29a734e7) )\
	ROM_LOAD16_BYTE( "263nd.c5", 0x2000000, 0x400000, CRC(3348DC5D) SHA1(a119e80aa2b36a7d8c7e8debd0eb13441a19adff) )\
	ROM_LOAD16_BYTE( "263nd.c6", 0x2000001, 0x400000, CRC(D90FC1A0) SHA1(0eaf5f658212c19a4cdbcdbff3b04389a2be76bb) )

#define MSLUG4DD_YMSND \
	ROM_LOAD( "263dd.v1", 0x000000, 0x400000, CRC(8cb5a9ef) SHA1(272ad312f8694acf9190ebf48d1c8a5fec73c130) )\
	ROM_LOAD( "263dd.v2", 0x400000, 0x400000, CRC(94217b1e) SHA1(c468a192e2ec0952a20cfd9e391cf6c12346efb3) )\
	ROM_LOAD( "263dd.v3", 0x800000, 0x400000, CRC(7616fcec) SHA1(83006094379d0373b967603dcaa1cec69c2f746f) )\
	ROM_LOAD( "263dd.v4", 0xc00000, 0x400000, CRC(c5967f91) SHA1(874562109c3df9e1fdefbb3970092128aaa31a17) )

#define MSLUG4DD_SPRITES \
	ROM_LOAD16_BYTE( "263nd.c1", 0x0000000, 0x800000, CRC(361ddda6) SHA1(95f329019660f1ec7f15f6ea1e0c2bfc84b02ca8) )\
	ROM_LOAD16_BYTE( "263nd.c2", 0x0000001, 0x800000, CRC(15d192c4) SHA1(0b9afb62a1d4d764d616408b09e17a05fc4a87e7) )\
	ROM_LOAD16_BYTE( "263dd.c3", 0x1000000, 0x800000, CRC(e296612c) SHA1(57eb1861b738247aaf0643641268f676b71ed025) )\
	ROM_LOAD16_BYTE( "263dd.c4", 0x1000001, 0x800000, CRC(c22e3ad0) SHA1(11ce301d4e5b4b1b76e2b82d18c4a9b8b56eeb20) )\
	ROM_LOAD16_BYTE( "263dd.c5", 0x2000000, 0x400000, CRC(288cd91f) SHA1(a195be13d4ef19705cd3f08be1ec8cdacb29f159) )\
	ROM_LOAD16_BYTE( "263dd.c6", 0x2000001, 0x400000, CRC(b015aeee) SHA1(24e12e72b82eea9e52a4cefc774efb3bd7db822f) )

#define MSLUG4D_SPRITES \
	ROM_LOAD16_BYTE( "263d.c1", 0x0000000, 0x800000, CRC(a75ffcde) SHA1(97f405a95a56615ae49f79e1a69f98cc2f2434ef) )\
	ROM_LOAD16_BYTE( "263d.c2", 0x0000001, 0x800000, CRC(5ab0d12b) SHA1(8a3d95dd2e9cc1b6dcf6a957fed43ee390248307) )\
	ROM_LOAD16_BYTE( "263d.c3", 0x1000000, 0x800000, CRC(61af560c) SHA1(aa7bc45e03a6bbd18eb56d118d4932102ccb196a) )\
	ROM_LOAD16_BYTE( "263d.c4", 0x1000001, 0x800000, CRC(f2c544fd) SHA1(179b064f81b49f5808d7a7a5bce28e95b09e5abe) )\
	ROM_LOAD16_BYTE( "263d.c5", 0x2000000, 0x800000, CRC(84c66c44) SHA1(9273f44bf11891aa04ddd2cbb6442d084c2a2e04) )\
	ROM_LOAD16_BYTE( "263d.c6", 0x2000001, 0x800000, CRC(5ed018ab) SHA1(e78501fa8a80960093a4d54ce952681a98300148) )

#define MS4BOOT_YMSND \
	ROM_LOAD( "263bl.v1", 0x000000, 0x800000, CRC(fd6b982e) SHA1(100313166c9ec57f1c540de05625c506b30ad13c) )\
	ROM_LOAD( "263bl.v2", 0x800000, 0x800000, CRC(20125227) SHA1(2e350c0c580e87445bf103c01fc62b14f0c19216) )

#define MSLUG4HD_SPRITES \
	ROM_LOAD16_BYTE( "263d.c1", 0x0000000, 0x800000, CRC(a75ffcde) SHA1(97f405a95a56615ae49f79e1a69f98cc2f2434ef) )\
	ROM_LOAD16_BYTE( "263d.c2", 0x0000001, 0x800000, CRC(5ab0d12b) SHA1(8a3d95dd2e9cc1b6dcf6a957fed43ee390248307) )\
	ROM_LOAD16_BYTE( "263d.c3", 0x1000000, 0x800000, CRC(61af560c) SHA1(aa7bc45e03a6bbd18eb56d118d4932102ccb196a) )\
	ROM_LOAD16_BYTE( "263d.c4", 0x1000001, 0x800000, CRC(f2c544fd) SHA1(179b064f81b49f5808d7a7a5bce28e95b09e5abe) )\
	ROM_LOAD16_BYTE( "263nd.c5", 0x2000000, 0x400000, CRC(3348DC5D) SHA1(a119e80aa2b36a7d8c7e8debd0eb13441a19adff) )\
	ROM_LOAD16_BYTE( "263nd.c6", 0x2000001, 0x400000, CRC(D90FC1A0) SHA1(0eaf5f658212c19a4cdbcdbff3b04389a2be76bb) )

#define MSLUG4FR_SPRITES \
	ROM_LOAD16_BYTE( "263_hacks02.c1", 0x0000000, 0x800000, CRC(6e53b397) SHA1(5ace71164b860a9a7fcf87395cb200a2baf06b6e) )\
	ROM_LOAD16_BYTE( "263_hacks02.c2", 0x0000001, 0x800000, CRC(D1a7d312) SHA1(81180989e40b79f2be61115560154a2b1ece0304) )\
	ROM_LOAD16_BYTE( "263_hacks02.c3", 0x1000000, 0x800000, CRC(325d475e) SHA1(E6acdc51af211a856745391c2b7098eef4ddcc8f) )\
	ROM_LOAD16_BYTE( "263_hacks02.c4", 0x1000001, 0x800000, CRC(5c908d0f) SHA1(5092d8d215e85e7faa7f989e8ee16644dc16fe84) )\
	ROM_LOAD16_BYTE( "263_hacks02.c5", 0x2000000, 0x800000, CRC(B85f3ae8) SHA1(4f73e5c7024e337d65b323447226b474e20a9738) )\
	ROM_LOAD16_BYTE( "263_hacks02.c6", 0x2000001, 0x800000, CRC(9dfdad34) SHA1(Cfcdbf1d6b058ecccb8b1cc775397d0404bd5859) )

#define MSLUG4LW_YMSND \
	ROM_LOAD( "263dd.v1",       0x000000, 0x400000, CRC(8cb5a9ef) SHA1(272ad312f8694acf9190ebf48d1c8a5fec73c130) )\
	ROM_LOAD( "263_hacks05.v2", 0x400000, 0x400000, CRC(c9572c14) SHA1(4338aaceada29ea1cd2baf0d6ada4911246f5b09) )\
	ROM_LOAD( "263_hacks05.v3", 0x800000, 0x400000, CRC(e7c14624) SHA1(1d17f7e7cfe9bb03f1a374b31427c4e5203433f2) )\
	ROM_LOAD( "263_hacks05.v4", 0xc00000, 0x400000, CRC(b1e5ac70) SHA1(0d6eea68085c0e6cd7c7e29edb06f05a712f6d62) )\
	ROM_IGNORE(0x400000)

#define MSLUG4DDH_SPRITES \
	ROM_LOAD16_BYTE( "263d.c1", 0x0000000, 0x800000, CRC(a75ffcde) SHA1(97f405a95a56615ae49f79e1a69f98cc2f2434ef) )\
	ROM_LOAD16_BYTE( "263d.c2", 0x0000001, 0x800000, CRC(5ab0d12b) SHA1(8a3d95dd2e9cc1b6dcf6a957fed43ee390248307) )\
	ROM_LOAD16_BYTE( "263d.c3", 0x1000000, 0x800000, CRC(61af560c) SHA1(aa7bc45e03a6bbd18eb56d118d4932102ccb196a) )\
	ROM_LOAD16_BYTE( "263d.c4", 0x1000001, 0x800000, CRC(f2c544fd) SHA1(179b064f81b49f5808d7a7a5bce28e95b09e5abe) )\
	ROM_LOAD16_BYTE( "263_hacks20.c5", 0x2000000, 0x800000, CRC(1d340215) SHA1(38f1380c286c6452c9d513b7761e9142126dcea4) )\
	ROM_LOAD16_BYTE( "263_hacks20.c6", 0x2000001, 0x800000, CRC(231d54ea) SHA1(d9a7fd64661add60d8e4ff961f00e1aae9458a74) )

#define MSLUG4LA_SPRITES \
	ROM_LOAD16_BYTE( "263_hacks02.c1", 0x0000000, 0x800000, CRC(6e53b397) SHA1(5ace71164b860a9a7fcf87395cb200a2baf06b6e) )\
	ROM_LOAD16_BYTE( "263_hacks02.c2", 0x0000001, 0x800000, CRC(D1a7d312) SHA1(81180989e40b79f2be61115560154a2b1ece0304) )\
	ROM_LOAD16_BYTE( "263_hacks02.c3", 0x1000000, 0x800000, CRC(325d475e) SHA1(E6acdc51af211a856745391c2b7098eef4ddcc8f) )\
	ROM_LOAD16_BYTE( "263_hacks02.c4", 0x1000001, 0x800000, CRC(5c908d0f) SHA1(5092d8d215e85e7faa7f989e8ee16644dc16fe84) )\
	ROM_LOAD16_BYTE( "263_la01.c5", 0x2000000, 0x800000, CRC(acf94433) SHA1(d71faba62bf0f72ca68307e9020ac6c47f17327d) )\
	ROM_LOAD16_BYTE( "263_la01.c6", 0x2000001, 0x800000, CRC(b0fe29be) SHA1(405a274dc7dbc928026478fd02abbef647c3ccc7) )


#define MSLUG5_YMSND \
	ROM_LOAD( "268.v1", 0x000000, 0x800000, CRC(ae31d60c) SHA1(c42285cf4e52fea74247860813e826df5aa7600a) )\
	ROM_LOAD( "268.v2", 0x800000, 0x800000, CRC(c40613ed) SHA1(af889570304e2867d7dfea1e94e388c06249fb67) )

#define MSLUG5_SPRITES \
	ROM_LOAD16_BYTE( "268.c1", 0x0000000, 0x800000, CRC(ab7c389a) SHA1(025a188de589500bf7637fa8e7a37ab24bf4312e) )\
	ROM_LOAD16_BYTE( "268.c2", 0x0000001, 0x800000, CRC(3560881b) SHA1(493d218c92290b4770024d6ee2917c4022753b07) )\
	ROM_LOAD16_BYTE( "268.c3", 0x1000000, 0x800000, CRC(3af955ea) SHA1(cf36b6ae9b0d12744b17cb7a928399214de894be) )\
	ROM_LOAD16_BYTE( "268.c4", 0x1000001, 0x800000, CRC(c329c373) SHA1(5073d4079958a0ef5426885af2c9e3178f37d5e0) )\
	ROM_LOAD16_BYTE( "268.c5", 0x2000000, 0x800000, CRC(959c8177) SHA1(889bda7c65d71172e7d89194d1269561888fe789) )\
	ROM_LOAD16_BYTE( "268.c6", 0x2000001, 0x800000, CRC(010a831b) SHA1(aec140661e3ae35d264df416478ba15188544d91) )\
	ROM_LOAD16_BYTE( "268.c7", 0x3000000, 0x800000, CRC(6d72a969) SHA1(968dd9a4d1209b770b9b85ea6532fa24d262a262) )\
	ROM_LOAD16_BYTE( "268.c8", 0x3000001, 0x800000, CRC(551d720e) SHA1(ebf69e334fcaba0fda6fd432fd0970283a365d12) )

#define MS5BOOT_YMSND \
	ROM_LOAD( "268boot.v1", 0x000000, 0x400000, CRC(c3540e0d) SHA1(bf7ca3abe291b28a4cfaef791f0c556cc98ad8d8) )\
	ROM_LOAD( "268boot.v2", 0x400000, 0x400000, CRC(077bd2f4) SHA1(1699959d17f8c7113cebdb9da2e1cd18ce48486c) )\
	ROM_LOAD( "268boot.v3", 0x800000, 0x400000, CRC(39b14567) SHA1(1658612a93ba30130f9260bc41d3f18f6b90c1e7) )\
	ROM_LOAD( "268boot.v4", 0xc00000, 0x400000, CRC(969ff3b2) SHA1(50feceb741a1c08b000b077a33151ab1352eb798) )

#define MSLUG5ND_YMSND \
	ROM_LOAD( "268nd.v1", 0x000000, 0x400000, CRC(dc7f03f0) SHA1(e1e31dcaff1f8d2c4b2d657aa856a60771d82c26) )\
	ROM_LOAD( "268nd.v2", 0x400000, 0x400000, CRC(eeb7c926) SHA1(4d71f59948bdc5ed2e6daf101f16fd979e1ab193) )\
	ROM_LOAD( "268nd.v3", 0x800000, 0x400000, CRC(02fd519e) SHA1(6fbda5bf8dbf52bc110aba7b9d20b24bdc2fe0d7) )\
	ROM_LOAD( "268nd.v4", 0xc00000, 0x400000, CRC(d7c67794) SHA1(345dcbf76b25b2fbd10bcaf08ee395da0a52c5ce) )

#define MSLUG5ND_SPRITES \
	ROM_LOAD16_BYTE( "268d.c1", 0x0000000, 0x800000, CRC(969c0d62) SHA1(DE3C5CFA980CCA2FAB0416AC0D292948B5D4C9C3) )\
	ROM_LOAD16_BYTE( "268d.c2", 0x0000001, 0x800000, CRC(c69ae867) SHA1(3198EE5C7C2C7563B49EBD9F7BA95D9B0B303F6C) )\
	ROM_LOAD16_BYTE( "268d.c3", 0x1000000, 0x800000, CRC(d7beaeaf) SHA1(99443EA4C1BAB45F1977A390EB7E1A0163915110) )\
	ROM_LOAD16_BYTE( "268nd.c4", 0x1000001, 0x800000, CRC(e1b1131b) SHA1(68a36d336582069e79ad481638d92f57c4cd6523) )\
	ROM_LOAD16_BYTE( "268d.c5", 0x2000000, 0x800000, CRC(2fa1a5ad) SHA1(4AE15D29BA979601598EDDF8905072FE1D9E0A98) )\
	ROM_LOAD16_BYTE( "268d.c6", 0x2000001, 0x800000, CRC(6de89589) SHA1(86A6C036BF51AF516FEA83A30874026EC1586A83) )\
	ROM_LOAD16_BYTE( "268d.c7", 0x3000000, 0x800000, CRC(97bd0c0a) SHA1(30F3280FE527098ECF46541CC645A59B366105EA) )\
	ROM_LOAD16_BYTE( "268d.c8", 0x3000001, 0x800000, CRC(c0d5bc20) SHA1(b5d0d81d5cc624538b0651c568295e578a1330d1) )

#define MSLUG5D_SPRITES \
	ROM_LOAD16_BYTE( "268d.c1", 0x0000000, 0x800000, CRC(969c0d62) SHA1(DE3C5CFA980CCA2FAB0416AC0D292948B5D4C9C3) )\
	ROM_LOAD16_BYTE( "268d.c2", 0x0000001, 0x800000, CRC(c69ae867) SHA1(3198EE5C7C2C7563B49EBD9F7BA95D9B0B303F6C) )\
	ROM_LOAD16_BYTE( "268d.c3", 0x1000000, 0x800000, CRC(d7beaeaf) SHA1(99443EA4C1BAB45F1977A390EB7E1A0163915110) )\
	ROM_LOAD16_BYTE( "268d.c4", 0x1000001, 0x800000, CRC(899fb2af) SHA1(A06A424C813C1B4F922C5404405779F605C90A06) )\
	ROM_LOAD16_BYTE( "268d.c5", 0x2000000, 0x800000, CRC(2fa1a5ad) SHA1(4AE15D29BA979601598EDDF8905072FE1D9E0A98) )\
	ROM_LOAD16_BYTE( "268d.c6", 0x2000001, 0x800000, CRC(6de89589) SHA1(86A6C036BF51AF516FEA83A30874026EC1586A83) )\
	ROM_LOAD16_BYTE( "268d.c7", 0x3000000, 0x800000, CRC(97bd0c0a) SHA1(30F3280FE527098ECF46541CC645A59B366105EA) )\
	ROM_LOAD16_BYTE( "268d.c8", 0x3000001, 0x800000, CRC(c0d5bc20) SHA1(b5d0d81d5cc624538b0651c568295e578a1330d1) )

#define MSLUG5D1_SPRITES \
    ROM_LOAD16_BYTE( "268hd.c1", 0x0000000, 0x800000, CRC(e8239365) SHA1(E0A75902A783110049730B66AD3CBCCDD804BF62) )\
	ROM_LOAD16_BYTE( "268hd.c2", 0x0000001, 0x800000, CRC(89b21d4c) SHA1(862AC31CE9570529B33E6F58ADA0AC867A442679) )\
	ROM_LOAD16_BYTE( "268hd.c3", 0x1000000, 0x800000, CRC(3cda13a0) SHA1(5D029C92688384EAFD65AD711A2954D0B86A3163) )\
	ROM_LOAD16_BYTE( "268hd.c4", 0x1000001, 0x800000, CRC(9c00160d) SHA1(C612102F9CD288BBA4C245B5855D795E4362B217) )\
	ROM_LOAD16_BYTE( "268hd.c5", 0x2000000, 0x800000, CRC(38754256) SHA1(744D91D041998CA27EE2781188746797C65377BD) )\
	ROM_LOAD16_BYTE( "268hd.c6", 0x2000001, 0x800000, CRC(59d33e9c) SHA1(ED7F5C7DB4C35EC5A7DB0AD5436110B3FE80B89A) )\
	ROM_LOAD16_BYTE( "268hd.c7", 0x3000000, 0x800000, CRC(c9f8c357) SHA1(7064224AFA887F57245F5479B0B46228539A131B) )\
	ROM_LOAD16_BYTE( "268hd.c8", 0x3000001, 0x800000, CRC(fafc3eb9) SHA1(F4F73194CC7FD151BE3CA2561F993B5BE7628369) )

#define MS5PLUSD_YMSND \
	ROM_LOAD( "268_psd.v1", 0x000000, 0x800000, CRC(7ff6ca47) SHA1(e36c4e6a349dd9fd38d1a36b6760bbd943c2a42f) )\
	ROM_LOAD( "268_psd.v2", 0x800000, 0x800000, CRC(696cce3b) SHA1(538d4a25befa4ffd25a48df97380469c13980f7a) )

#define MSLUG5FR_SPRITES \
    ROM_LOAD16_BYTE( "268_hacks03.c1", 0x0000000, 0x800000, CRC(c115bcbd) SHA1(241b71cb442844df0f1c7005955c5ae85b95a0b9) )\
	ROM_LOAD16_BYTE( "268_hacks03.c2", 0x0000001, 0x800000, CRC(a95f284f) SHA1(db6b18b15f871ac17e88d2eec72b95c8a63be959) )\
	ROM_LOAD16_BYTE( "268hd.c3", 0x1000000, 0x800000, CRC(3cda13a0) SHA1(5d029c92688384eafd65ad711a2954d0b86a3163) )\
	ROM_LOAD16_BYTE( "268hd.c4", 0x1000001, 0x800000, CRC(9c00160d) SHA1(c612102f9cd288bba4c245b5855d795e4362b217) )\
	ROM_LOAD16_BYTE( "268_hacks03.c5", 0x2000000, 0x800000, CRC(dae4bdba) SHA1(c7da602cc59296fd264fd2f97b29bc242340760f) )\
	ROM_LOAD16_BYTE( "268_hacks03.c6", 0x2000001, 0x800000, CRC(731a14ce) SHA1(2363d3f0f2e2a621051de0db6e465f7b1a387ef9) )\
	ROM_LOAD16_BYTE( "268_hacks03.c7", 0x3000000, 0x800000, CRC(bc8296af) SHA1(4309d093c09219fb90104d30211c204a2abcf82e) )\
	ROM_LOAD16_BYTE( "268_hacks03.c8", 0x3000001, 0x800000, CRC(6fabe6d5) SHA1(12c23b85de985b4daba7def1d9d9d9b4c050ab63) )

#define MSLUG5W_YMSND \
	ROM_LOAD( "268_hacks04.v1", 0x000000, 0x400000, CRC(a8e12a92) SHA1(b23d80d131649c1d243bb30cf5b2141354a7ac55) )\
	ROM_LOAD( "268_hacks04.v2", 0x400000, 0x400000, CRC(0608cba3) SHA1(ad7c985a25abd68c58e502c008b45e0b14422829) )\
	ROM_LOAD( "268nd.v3", 0x800000, 0x400000, CRC(02fd519e) SHA1(6fbda5bf8dbf52bc110aba7b9d20b24bdc2fe0d7) )\
	ROM_LOAD( "268_hacks04.v4", 0xc00000, 0x400000, CRC(179cbca3) SHA1(fdcb0df0c2f73d11cd32a8625ad6fa24cedb9dc9) )\
	ROM_IGNORE(0x2f0)

#define MSLUG5W_SPRITES \
	ROM_LOAD16_BYTE( "268hd.c1", 0x0000000, 0x800000, CRC(e8239365) SHA1(E0A75902A783110049730B66AD3CBCCDD804BF62) )\
	ROM_LOAD16_BYTE( "268hd.c2", 0x0000001, 0x800000, CRC(89b21d4c) SHA1(862AC31CE9570529B33E6F58ADA0AC867A442679) )\
	ROM_LOAD16_BYTE( "268hd.c3", 0x1000000, 0x800000, CRC(3cda13a0) SHA1(5D029C92688384EAFD65AD711A2954D0B86A3163) )\
	ROM_LOAD16_BYTE( "268hd.c4", 0x1000001, 0x800000, CRC(9c00160d) SHA1(C612102F9CD288BBA4C245B5855D795E4362B217) )\
	ROM_LOAD16_BYTE( "268_hacks04.c5", 0x2000000, 0x800000, CRC(483a986c) SHA1(b6b1a08a340e4528575eff6fcf34e5965e4b6cda) )\
	ROM_LOAD16_BYTE( "268_hacks04.c6", 0x2000001, 0x800000, CRC(d918f796) SHA1(fa81b7930f3f82571bc6f1f70768a613f1573325) )\
	ROM_LOAD16_BYTE( "268_hacks04.c7", 0x3000000, 0x800000, CRC(bdb9a887) SHA1(f9984dbf5694afd42c2dde1ab0510c5d53719a1f) )\
	ROM_LOAD16_BYTE( "268_hacks04.c8", 0x3000001, 0x800000, CRC(6f8ac6fb) SHA1(49244f0fddde4d11b3f99f52b0e0e74d73414c71) )

#define MSLUG5F_YMSND \
	ROM_LOAD( "268_hacks06.v1", 0x000000, 0x400000, CRC(f61daa9e) SHA1(c9a64c48a37d9fcbd83f34580f44330aab0a0601) )\
	ROM_LOAD( "268_hacks06.v2", 0x400000, 0x400010, CRC(7fed45cb) SHA1(13ba8955f4b3703b9d44bab526263d552e7e1916) )\
	ROM_LOAD( "268_hacks06.v3", 0x800000, 0x400000, CRC(dc737e12) SHA1(f940ea767404f6a15dea526eda56ad66517ba79e) )\
	ROM_LOAD( "268_hacks06.v4", 0xc00000, 0x400000, CRC(e3a76022) SHA1(58a7cd31728f8a5dcdf12fd1cac5f97f558b9f50) )\
	ROM_IGNORE(0x37510)

#define MSLUG5F_SPRITES \
	ROM_LOAD16_BYTE( "268hd.c1", 0x0000000, 0x800000, CRC(e8239365) SHA1(E0A75902A783110049730B66AD3CBCCDD804BF62) )\
	ROM_LOAD16_BYTE( "268hd.c2", 0x0000001, 0x800000, CRC(89b21d4c) SHA1(862AC31CE9570529B33E6F58ADA0AC867A442679) )\
	ROM_LOAD16_BYTE( "268hd.c3", 0x1000000, 0x800000, CRC(3cda13a0) SHA1(5D029C92688384EAFD65AD711A2954D0B86A3163) )\
	ROM_LOAD16_BYTE( "268hd.c4", 0x1000001, 0x800000, CRC(9c00160d) SHA1(C612102F9CD288BBA4C245B5855D795E4362B217) )\
	ROM_LOAD16_BYTE( "268_hacks06.c5", 0x2000000, 0x800000, CRC(3bc09dbf) SHA1(a47954a3295dc21e7db7dedc03ab789f8005eed5) )\
	ROM_LOAD16_BYTE( "268_hacks06.c6", 0x2000001, 0x800000, CRC(0db5ce14) SHA1(aa2c4bb143e3bba18523df645c5be249eb9fc147) )\
	ROM_LOAD16_BYTE( "268_hacks06.c7", 0x3000000, 0x800000, CRC(aa4f3214) SHA1(dc3a8fac2cc81e60abafc209eb4bcccdaafde380) )\
	ROM_LOAD16_BYTE( "268_hacks06.c8", 0x3000001, 0x800000, CRC(c86d0d81) SHA1(1a00aa65481410c0b64fe971e547638c05c5a0ce) )

#define MSLUG5EXT_YMSND \
	ROM_LOAD( "268_hacks24.v1", 0x000000, 0x800000, CRC(14848c5c) SHA1(8f67daa54738f8b217cce1f1d0a7872fd06d91b6) )\
	ROM_LOAD( "268_psd.v2",     0x800000, 0x800000, CRC(696cce3b) SHA1(538d4a25befa4ffd25a48df97380469c13980f7a) )

#define MSLUG5LA_YMSND \
	ROM_LOAD( "268_hacks04.v1", 0x000000, 0x400000, CRC(a8e12a92) SHA1(b23d80d131649c1d243bb30cf5b2141354a7ac55) )\
	ROM_LOAD( "268_hacks04.v2", 0x400000, 0x400000, CRC(0608cba3) SHA1(ad7c985a25abd68c58e502c008b45e0b14422829) )\
	ROM_LOAD( "268_la01.v3", 0x800000, 0x400000, CRC(aeff1982) SHA1(ac777a12639e21f0f227be84a59c7bb543c6ddd9) )\
	ROM_LOAD( "268_hacks04.v4", 0xc00000, 0x400000, CRC(179cbca3) SHA1(fdcb0df0c2f73d11cd32a8625ad6fa24cedb9dc9) )\
	ROM_IGNORE(0x2f0)

#define MSLUG5LA_SPRITES \
	ROM_LOAD16_BYTE( "268_la01.c1", 0x0000000, 0x800000, CRC(aad8ff17) SHA1(f8c5f7bd9aa4b930ceb38b25934650ac810563df) )\
	ROM_LOAD16_BYTE( "268_la01.c2", 0x0000001, 0x800000, CRC(b0404dd4) SHA1(0ab1f81072dac777c3269811dfb69cb40bd7a245) )\
	ROM_LOAD16_BYTE( "268_la01.c3", 0x1000000, 0x800000, CRC(95dda158) SHA1(7ec4625a17f815e8d7e690cb01f3c9dbce75e39b) )\
	ROM_LOAD16_BYTE( "268_la01.c4", 0x1000001, 0x800000, CRC(c052cf1f) SHA1(198041e813914570b943fc1927e986e5553c6bd4) )\
	ROM_LOAD16_BYTE( "268_la01.c5", 0x2000000, 0x800000, CRC(8b9f7e21) SHA1(0dcefc5efc67d520a880b3da6122c342ebdc5e2f) )\
	ROM_LOAD16_BYTE( "268_la01.c6", 0x2000001, 0x800000, CRC(4a0693cf) SHA1(c4bdf72754f85250dfccdd4444941931a6596569) )\
	ROM_LOAD16_BYTE( "268_la01.c7", 0x3000000, 0x800000, CRC(3ad5eaf4) SHA1(b4c4cf0425d42bd47bc85acfab93006c003af341) )\
	ROM_LOAD16_BYTE( "268_la01.c8", 0x3000001, 0x800000, CRC(3e7c670d) SHA1(166eda40c01d01b1d8ec82f2482e8cb4e5ca0e79) )

#define MSLUG5RMXE_YMSND \
	ROM_LOAD( "268_rmxe0s01.v1", 0x000000, 0x400000, CRC(fcc8ba01) SHA1(c187c318c65f6aefae05e6cef9f17492edf2e4db) )\
	ROM_LOAD( "268_rmxe0s01.v2", 0x400000, 0x400000, CRC(386f32bd) SHA1(d49fc812c442563b9e82da43bf5c6b9deea0f687) )\
	ROM_LOAD( "268_hacks06.v3",  0x800000, 0x400000, CRC(dc737e12) SHA1(f940ea767404f6a15dea526eda56ad66517ba79e) )\
	ROM_LOAD( "268_rmxe0s01.v4", 0xc00000, 0x400000, CRC(2b310542) SHA1(5f0050d36cd1f722d78d9ae98db537eacc4d8548) )\
	ROM_IGNORE(0x37510)

#define MSLUG5RMXE_SPRITES \
	ROM_LOAD16_BYTE( "268_rmxe0s01.c1", 0x0000000, 0x800000, CRC(5e2e9087) SHA1(2366df8b0ee03e8938dcae571dccf32cfcfd685b) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c2", 0x0000001, 0x800000, CRC(d15770e3) SHA1(162c166c3fdc2a29af88dfea8a10138284029e36) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c3", 0x1000000, 0x800000, CRC(3e104b8f) SHA1(ee8467e07f27fbcad35a239b45e1e7e5aed79a25) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c4", 0x1000001, 0x800000, CRC(03f32dda) SHA1(64bc08dce4a8188878d683a6290e370e4d3488a1) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c5", 0x2000000, 0x800000, CRC(fd3e0f24) SHA1(5143c521c79a011465c93717171bb24b550bdc1f) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c6", 0x2000001, 0x800000, CRC(3c873aba) SHA1(b1c882f7c228046929f82a24d9f635c1b6443a94) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c7", 0x3000000, 0x800000, CRC(0d070c3b) SHA1(2d785bd095726b4e48aa65578b9694372f4c46c3) )\
	ROM_LOAD16_BYTE( "268_rmxe0s01.c8", 0x3000001, 0x800000, CRC(09751dd8) SHA1(5a1378faa3bd3ebb6573d7c171639df153b5202b) )

#define MS5PCBD_YMSND \
    ROM_LOAD( "268pcbd.v1", 0x000000, 0x1000000, CRC(4182838b) SHA1(03e58b6709db467e92397b26b84658f5adb0ff92) )

#define MS5PCBD_SPRITES \
    ROM_LOAD16_BYTE( "268pcbd.c1", 0x0000000, 0x1000000, CRC(996debed) SHA1(ec6965a24862541f337a1b7e259038b9b40ad1d2) )\
	ROM_LOAD16_BYTE( "268pcbd.c2", 0x0000001, 0x1000000, CRC(960a9764) SHA1(687cf957d91ef9da219b573f444e668209dcea41) )\
	ROM_LOAD16_BYTE( "268pcbd.c3", 0x2000000, 0x1000000, CRC(f699994f) SHA1(ff079d3c126e9611760956d51c5df66d8013dd78) )\
	ROM_LOAD16_BYTE( "268pcbd.c4", 0x2000001, 0x1000000, CRC(49e902e8) SHA1(289a560ffe3d50abd773c61fa2df3c1f18d7e803) )

#define MSLUGX_YMSND \
	ROM_LOAD( "250.v1", 0x000000, 0x400000, CRC(c79ede73) SHA1(ebfcc67204ff9677cf7972fd5b6b7faabf07280c) ) \
	ROM_LOAD( "250.v2", 0x400000, 0x400000, CRC(ea9aabe1) SHA1(526c42ca9a388f7435569400e2f132e2724c71ff) ) \
	ROM_LOAD( "250.v3", 0x800000, 0x200000, CRC(2ca65102) SHA1(45979d1edb1fc774a415d9386f98d7cb252a2043) )

#define MSLUGX_SPRITES \
	ROM_LOAD16_BYTE( "250.c1", 0x0000000, 0x800000, CRC(09a52c6f) SHA1(c3e8a8ccdac0f8bddc4c3413277626532405fae2) )\
	ROM_LOAD16_BYTE( "250.c2", 0x0000001, 0x800000, CRC(31679821) SHA1(554f600a3aa09c16c13c625299b087a79d0d15c5) )\
	ROM_LOAD16_BYTE( "250.c3", 0x1000000, 0x800000, CRC(fd602019) SHA1(c56646c62387bc1439d46610258c755beb8d7dd8) )\
	ROM_LOAD16_BYTE( "250.c4", 0x1000001, 0x800000, CRC(31354513) SHA1(31be8ea2498001f68ce4b06b8b90acbf2dcab6af) )\
	ROM_LOAD16_BYTE( "250.c5", 0x2000000, 0x800000, CRC(a4b56124) SHA1(d41069856df990a1a99d39fb263c8303389d5475) )\
	ROM_LOAD16_BYTE( "250.c6", 0x2000001, 0x800000, CRC(83e3e69d) SHA1(39be66287696829d243fb71b3fb8b7dc2bc3298f) )

#define MSLUGXFR_SPRITES \
	ROM_LOAD16_BYTE( "250_hacks06.c1", 0x0000000, 0x800000, CRC(eb7ce1ff) SHA1(e519b53c6938451f9bfb4d15c10baa7f152ecfcd) )\
	ROM_LOAD16_BYTE( "250_hacks06.c2", 0x0000001, 0x800000, CRC(45c7e401) SHA1(b71754cc746631f1a70c09ef564c7be11aa577e9) )\
	ROM_LOAD16_BYTE( "250.c3", 0x1000000, 0x800000, CRC(fd602019) SHA1(c56646c62387bc1439d46610258c755beb8d7dd8) )\
	ROM_LOAD16_BYTE( "250.c4", 0x1000001, 0x800000, CRC(31354513) SHA1(31be8ea2498001f68ce4b06b8b90acbf2dcab6af) )\
	ROM_LOAD16_BYTE( "250.c5", 0x2000000, 0x800000, CRC(a4b56124) SHA1(d41069856df990a1a99d39fb263c8303389d5475) )\
	ROM_LOAD16_BYTE( "250.c6", 0x2000001, 0x800000, CRC(83e3e69d) SHA1(39be66287696829d243fb71b3fb8b7dc2bc3298f) )

#define MSLUGXDD_SPRITES \
	ROM_LOAD16_BYTE( "250.c1", 0x0000000, 0x800000, CRC(09a52c6f) SHA1(c3e8a8ccdac0f8bddc4c3413277626532405fae2) )\
	ROM_LOAD16_BYTE( "250.c2", 0x0000001, 0x800000, CRC(31679821) SHA1(554f600a3aa09c16c13c625299b087a79d0d15c5) )\
	ROM_LOAD16_BYTE( "250.c3", 0x1000000, 0x800000, CRC(fd602019) SHA1(c56646c62387bc1439d46610258c755beb8d7dd8) )\
	ROM_LOAD16_BYTE( "250.c4", 0x1000001, 0x800000, CRC(31354513) SHA1(31be8ea2498001f68ce4b06b8b90acbf2dcab6af) )\
	ROM_LOAD16_BYTE( "250_hacks20.c5", 0x2000000, 0x800000, CRC(b2d7bf98) SHA1(1526564518aaf19a2eebc90175abbcc47c3e1147) )\
	ROM_LOAD16_BYTE( "250_hacks20.c6", 0x2000001, 0x800000, CRC(e0f7a059) SHA1(571d67ed24512bffc81554599c29e585dd533cf4) )

#define MSLUGXLB_SPRITES \
	ROM_LOAD16_BYTE( "250_hacks25.c1", 0x0000000, 0x800000, CRC(d52e8702) SHA1(e332e9c95679dbdbcdfdf868b3a0233ad6bd47c4) )\
	ROM_LOAD16_BYTE( "250_hacks25.c2", 0x0000001, 0x800000, CRC(edec334c) SHA1(56c6d6d19dd891e1dc66b1089abb2dacd1ad2611) )\
	ROM_LOAD16_BYTE( "250.c3", 0x1000000, 0x800000, CRC(fd602019) SHA1(c56646c62387bc1439d46610258c755beb8d7dd8) )\
	ROM_LOAD16_BYTE( "250.c4", 0x1000001, 0x800000, CRC(31354513) SHA1(31be8ea2498001f68ce4b06b8b90acbf2dcab6af) )\
	ROM_LOAD16_BYTE( "250.c5", 0x2000000, 0x800000, CRC(a4b56124) SHA1(d41069856df990a1a99d39fb263c8303389d5475) )\
	ROM_LOAD16_BYTE( "250.c6", 0x2000001, 0x800000, CRC(83e3e69d) SHA1(39be66287696829d243fb71b3fb8b7dc2bc3298f) )

#define MSLUGXSC_SPRITES \
	ROM_LOAD16_BYTE( "250.c1", 0x0000000, 0x800000, CRC(09a52c6f) SHA1(c3e8a8ccdac0f8bddc4c3413277626532405fae2) )\
	ROM_LOAD16_BYTE( "250.c2", 0x0000001, 0x800000, CRC(31679821) SHA1(554f600a3aa09c16c13c625299b087a79d0d15c5) )\
	ROM_LOAD16_BYTE( "250.c3", 0x1000000, 0x800000, CRC(fd602019) SHA1(c56646c62387bc1439d46610258c755beb8d7dd8) )\
	ROM_LOAD16_BYTE( "250.c4", 0x1000001, 0x800000, CRC(31354513) SHA1(31be8ea2498001f68ce4b06b8b90acbf2dcab6af) )\
	ROM_LOAD16_BYTE( "250_hacks31.c5", 0x2000000, 0x800000, CRC(9a9422e4) SHA1(453b897a98574c7be881123398642cf5ec143896) )\
	ROM_LOAD16_BYTE( "250_hacks31.c6", 0x2000001, 0x800000, CRC(ef116b50) SHA1(de725caad51cc787fba5d21a709844fa96362488) )

 /***********
 Andro Dunos
**************/

ROM_START( androdunhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "049_hack01.p1", 0x000000, 0x080000, CRC(5f63193e) SHA1(7ba7bb8f9382ddd7b35601b8171a0e532c9dfe55) )
	ROM_LOAD16_WORD_SWAP( "049.p2", 0x080000, 0x080000, CRC(2f062209) SHA1(991cf3e3677929b2cc0b2787b0c7b6ad3700f618) )

	NEO_SFIX_128K( "049.s1", CRC(6349de5d) SHA1(bcc44b9576d7bedd9a39294530bb66f707690c72) )

	NEO_BIOS_AUDIO_128K( "049.m1", CRC(edd2acf4) SHA1(c4ee6ba834d54b9fc5a854dbc41a05877e090371) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "049.v1", 0x000000, 0x100000, CRC(ce43cb89) SHA1(47f82e077abb6efc6b1b0490412ae147d5d2acef) )

	ROM_REGION( 0x200000, "sprites", 0 )
	ROM_LOAD16_BYTE( "049.c1", 0x000000, 0x100000, CRC(7ace6db3) SHA1(c41cc9de8c0788dcc49ca494fd3bb3124062d9dd) )
	ROM_LOAD16_BYTE( "049.c2", 0x000001, 0x100000, CRC(b17024f7) SHA1(fcf7efae48fcdccaf5255c145de414fb246128f0) )
ROM_END

  /****************
 Art of Fighting 1
********************/

ROM_START( aofhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "044_hack01.p1", 0x000000, 0x080000, CRC(202a60d8) SHA1(f0caa8b6a18dff1b9568aaae552cf6d10a9db722) )

	NEO_SFIX_128K( "044.s1", CRC(89903f39) SHA1(a04a0c244a5d5c7a595fcf649107969635a6a8b6) )

	NEO_BIOS_AUDIO_128K( "044.m1", CRC(0987e4bb) SHA1(8fae4b7fac09d46d4727928e609ed9d3711dbded) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "044.v2", 0x000000, 0x200000, CRC(3ec632ea) SHA1(e3f413f580b57f70d2dae16dbdacb797884d3fce) )
	ROM_LOAD( "044.v4", 0x200000, 0x200000, CRC(4b0f8e23) SHA1(105da0cc5ba19869c7147fba8b177500758c232b) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "044.c1", 0x000000, 0x100000, CRC(ddab98a7) SHA1(f20eb81ec431268798c142c482146c1545af1c24) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "044.c2", 0x000001, 0x100000, CRC(d8ccd575) SHA1(f697263fe92164e274bf34c55327b3d4a158b332) )
	ROM_CONTINUE( 0x400001, 0x100000 )
	ROM_LOAD16_BYTE( "044.c3", 0x200000, 0x100000, CRC(403e898a) SHA1(dd5888f8b24a33b2c1f483316fe80c17849ccfc4) )
	ROM_CONTINUE( 0x600000, 0x100000 )
	ROM_LOAD16_BYTE( "044.c4", 0x200001, 0x100000, CRC(6235fbaa) SHA1(9090e337d7beed25ba81ae0708d0aeb57e6cf405) )
	ROM_CONTINUE( 0x600001, 0x100000 )
ROM_END

  /****************
 Art of Fighting 2
********************/

ROM_START( aof2hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "056_hack01.p1", 0x000000, 0x100000, CRC(3af1e484) SHA1(57b33dbb93d066092acddf163dbd67d15b5159e2) )

	NEO_SFIX_128K( "056.s1", CRC(8b02638e) SHA1(aa4d28804ca602da776948b5f223ea89e427906b) )

	NEO_BIOS_AUDIO_128K( "056.m1", CRC(f27e9d52) SHA1(dddae733d87ce7c88ad2580a8f64cb6ff9572e67) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "056.v1", 0x000000, 0x200000, CRC(4628fde0) SHA1(ece2a50f5270d844d58401b1447d1d856d78ea45) )
	ROM_LOAD( "056.v2", 0x200000, 0x200000, CRC(b710e2f2) SHA1(df4da585203eea7554d3ce718eb107e9cb6a0254) )
	ROM_LOAD( "056.v3", 0x400000, 0x100000, CRC(d168c301) SHA1(969273d1d11943e81560959359a2c4e69522af0e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "056.c1", 0x000000, 0x200000, CRC(17b9cbd2) SHA1(1eee81e02763d384bd1c10a6012473ca931e4093) )
	ROM_LOAD16_BYTE( "056.c2", 0x000001, 0x200000, CRC(5fd76b67) SHA1(11925a41a53b53c6df4a5ebd28f98300950f743b) )
	ROM_LOAD16_BYTE( "056.c3", 0x400000, 0x200000, CRC(d2c88768) SHA1(22e2d84aa0c095944190e249ce87ef50d3f7b8ce) )
	ROM_LOAD16_BYTE( "056.c4", 0x400001, 0x200000, CRC(db39b883) SHA1(59de86c513dc4e230ae25d9e3b7e84621b657b54) )
	ROM_LOAD16_BYTE( "056.c5", 0x800000, 0x200000, CRC(c3074137) SHA1(9a75e3d63cb98d54f900dcfb3a03e21f3148d32f) )
	ROM_LOAD16_BYTE( "056.c6", 0x800001, 0x200000, CRC(31de68d3) SHA1(13ba7046cdd6863125f8284e60f102d4720af5a4) )
	ROM_LOAD16_BYTE( "056.c7", 0xc00000, 0x200000, CRC(3f36df57) SHA1(79ee97e9ae811a51141b535633f90e1491209d54) )
	ROM_LOAD16_BYTE( "056.c8", 0xc00001, 0x200000, CRC(e546d7a8) SHA1(74a2fca994a5a93a5784a46c0f68193122456a09) )
ROM_END

ROM_START( aof2hack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "056_hack02.p1", 0x000000, 0x100000, CRC(89eb2c94) SHA1(8981f82190853cea181a39c57ab83f10a3dec05d) )

	NEO_SFIX_128K( "056.s1", CRC(8b02638e) SHA1(aa4d28804ca602da776948b5f223ea89e427906b) )

	NEO_BIOS_AUDIO_128K( "056.m1", CRC(f27e9d52) SHA1(dddae733d87ce7c88ad2580a8f64cb6ff9572e67) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "056.v1", 0x000000, 0x200000, CRC(4628fde0) SHA1(ece2a50f5270d844d58401b1447d1d856d78ea45) )
	ROM_LOAD( "056.v2", 0x200000, 0x200000, CRC(b710e2f2) SHA1(df4da585203eea7554d3ce718eb107e9cb6a0254) )
	ROM_LOAD( "056.v3", 0x400000, 0x100000, CRC(d168c301) SHA1(969273d1d11943e81560959359a2c4e69522af0e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "056.c1", 0x000000, 0x200000, CRC(17b9cbd2) SHA1(1eee81e02763d384bd1c10a6012473ca931e4093) )
	ROM_LOAD16_BYTE( "056.c2", 0x000001, 0x200000, CRC(5fd76b67) SHA1(11925a41a53b53c6df4a5ebd28f98300950f743b) )
	ROM_LOAD16_BYTE( "056.c3", 0x400000, 0x200000, CRC(d2c88768) SHA1(22e2d84aa0c095944190e249ce87ef50d3f7b8ce) )
	ROM_LOAD16_BYTE( "056.c4", 0x400001, 0x200000, CRC(db39b883) SHA1(59de86c513dc4e230ae25d9e3b7e84621b657b54) )
	ROM_LOAD16_BYTE( "056.c5", 0x800000, 0x200000, CRC(c3074137) SHA1(9a75e3d63cb98d54f900dcfb3a03e21f3148d32f) )
	ROM_LOAD16_BYTE( "056.c6", 0x800001, 0x200000, CRC(31de68d3) SHA1(13ba7046cdd6863125f8284e60f102d4720af5a4) )
	ROM_LOAD16_BYTE( "056.c7", 0xc00000, 0x200000, CRC(3f36df57) SHA1(79ee97e9ae811a51141b535633f90e1491209d54) )
	ROM_LOAD16_BYTE( "056.c8", 0xc00001, 0x200000, CRC(e546d7a8) SHA1(74a2fca994a5a93a5784a46c0f68193122456a09) )
ROM_END

  /****************
 Art of Fighting 3
********************/

ROM_START( aof3hack01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "096_hack01.p1", 0x000000, 0x100000, CRC(70969ff1) SHA1(a17d17c451655e01db8d42c6edd31fd2917bd486) )
	ROM_LOAD16_WORD_SWAP( "096.p2", 0x100000, 0x200000, CRC(4d5a2602) SHA1(4c26d6135d2877d9c38169662033e9d0cc24d943) )

	NEO_SFIX_128K( "096.s1", CRC(cc7fd344) SHA1(2c6846cf8ea61fb192ba181dbccb63594d572c0e) )

	NEO_BIOS_AUDIO_128K( "096.m1", CRC(cb07b659) SHA1(940b379957c2987d7ab0443cb80c3ff58f6ba559) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "096.v1", 0x000000, 0x200000, CRC(e2c32074) SHA1(69426e7e63fc31a73d1cd056cc9ae6a2c4499407) )
	ROM_LOAD( "096.v2", 0x200000, 0x200000, CRC(a290eee7) SHA1(e66a98cd9740188bf999992b417f8feef941cede) )
	ROM_LOAD( "096.v3", 0x400000, 0x200000, CRC(199d12ea) SHA1(a883bf34e685487705a8dafdd0b8db15eb360e80) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "096.c1", 0x0000000, 0x400000, CRC(f17b8d89) SHA1(7180df23f7c7a964b0835fda76970b12f0aa9ea8) )
	ROM_LOAD16_BYTE( "096.c2", 0x0000001, 0x400000, CRC(3840c508) SHA1(55adc7cd26fec3e4dbd779df6701bc6eaba41b84) )
	ROM_LOAD16_BYTE( "096.c3", 0x0800000, 0x400000, CRC(55f9ee1e) SHA1(fbe1b7891beae66c5fcbc7e36168dc1b460ede91) )
	ROM_LOAD16_BYTE( "096.c4", 0x0800001, 0x400000, CRC(585b7e47) SHA1(d50ea91397fc53d86470ff5b493a44d57c010306) )
	ROM_LOAD16_BYTE( "096.c5", 0x1000000, 0x400000, CRC(c75a753c) SHA1(fc977f8710816a369a5d0d49ee84059380e93fb7) )
	ROM_LOAD16_BYTE( "096.c6", 0x1000001, 0x400000, CRC(9a9d2f7a) SHA1(a89a713bfcd93974c9acb21ce699d365b08e7e39) )
	ROM_LOAD16_BYTE( "096.c7", 0x1800000, 0x200000, CRC(51bd8ab2) SHA1(c8def9c64de64571492b5b7e14b794e3c18f1393) )
	ROM_LOAD16_BYTE( "096.c8", 0x1800001, 0x200000, CRC(9a34f99c) SHA1(fca72d95ec42790a7f1e771a1e25dbc5bec5fc19) )
ROM_END

  /********
 Bang Bead
************/

ROM_START( bangbeadd )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "259.p1", 0x100000, 0x100000, CRC(88a37f8b) SHA1(566db84850fad5e8fe822e8bba910a33e083b550) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "259p.s1", CRC(bb50fb2d) SHA1(7372939f328fb5e7d09c16985e09ae8c34702b0c) )

	NEO_BIOS_AUDIO_128K( "259.m1", CRC(85668ee9) SHA1(7d3f51710cf90c097cd3faaeeef10ceb85cbb3e8) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "259.v1", 0x000000, 0x400000, CRC(088eb8ab) SHA1(608306e35501dd7d382d9f96b28e7550aa896a03) )
	ROM_LOAD( "259.v2", 0x400000, 0x100000, CRC(97528fe9) SHA1(8f5eddbb3a9a225492479d1a44801f3916c8e791) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "259d.c1", 0x0000000, 0x800000, CRC(c50a91c3) SHA1(47ef41369b057b07bf143196e929ac5ab3679d33) )
	ROM_LOAD16_BYTE( "259d.c2", 0x0000001, 0x800000, CRC(820ab36e) SHA1(acad44f815989d5908585cfeff5658fe85a50ad8) )
ROM_END

ROM_START( bangbeadp )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "259.p1", 0x100000, 0x100000, CRC(88a37f8b) SHA1(566db84850fad5e8fe822e8bba910a33e083b550) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "259p.s1", CRC(bb50fb2d) SHA1(7372939f328fb5e7d09c16985e09ae8c34702b0c) )

	NEO_BIOS_AUDIO_128K( "259.m1", CRC(85668ee9) SHA1(7d3f51710cf90c097cd3faaeeef10ceb85cbb3e8) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "259p.v1",     0x000000, 0x200000, CRC(e97b9385) SHA1(d213cae6cf5732b8ab1f8a8cf04afee5dfd9a260) )
	ROM_LOAD( "259p.v2",     0x200000, 0x200000, CRC(b0cbd70a) SHA1(fc7c8183f8ff9800e8aae7c8d44d962300058cfb) )
	ROM_LOAD( "259.v2",   0x400000, 0x100000, CRC(97528fe9) SHA1(8f5eddbb3a9a225492479d1a44801f3916c8e791) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "259p.c1", 0x000000, 0x200000, CRC(e3919e44) SHA1(54c722414b5a7ad311dc8ddf6fdda88535e829d1) )
	ROM_LOAD16_BYTE( "259p.c2", 0x000001, 0x200000, CRC(baf5a320) SHA1(ead3d81d9b4aeb45af4f9cb5c38157f2236b506c) )
	ROM_LOAD16_BYTE( "259p.c3", 0x400000, 0x100000, CRC(c8e52157) SHA1(f10f58e905c4cbaf182b20e63abe5364462133c5) )
	ROM_LOAD16_BYTE( "259p.c4", 0x400001, 0x100000, CRC(69fa8e60) SHA1(29c1fbdb79dedf1470683202e2cb3435732d9275) )
ROM_END

 /*****************************
 Crouching Tiger Hidden Dragon
********************************/

ROM_START( ct2k3hack01 ) // all confirmed
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "5003-p1_hack01.bin",   0x000000, 0x454f4e, CRC(f3010a50) SHA1(b95f322c0682999f2c378fdf2db14fc804f71153) )
	ROM_LOAD16_WORD_SWAP( "5003-p2.bin", 0x100000, 0x400000, CRC(adc1c22b) SHA1(271e0629989257a0d21d280c05df53df259414b1) )

	//NEO_SFIX_256K( "360spi.s1", CRC(6c355ab4) SHA1(71ac2bcd3dbda8402baecc56dabc2297b148a900) ) // not working
	NEO_SFIX_128K( "5003-s1.bin", CRC(5ba29aab) SHA1(e7ea67268a10243693bff722e6fd2276ca540acf) )

	NEO_BIOS_AUDIO_128K( "5003-m1.bin", CRC(1a8c274b) SHA1(5f6f9c533f4a296a18c741ce59a69cf6f5c836b9) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "5003-c1.bin", 0x0000000, 0x800000, CRC(68f54b67) SHA1(e2869709b11ea2846799fe431211c83e928e103e) )
	ROM_LOAD16_BYTE( "5003-c2.bin", 0x0000001, 0x800000, CRC(2f8849d5) SHA1(7ef74981aa056f5acab4ddabffd3e98b4cb970be) )
	ROM_LOAD16_BYTE( "5003-c3.bin", 0x1000000, 0x800000, CRC(ac4aff71) SHA1(c983f642e68deaa40fee3e208f2dd55f3bacbdc1) )
	ROM_LOAD16_BYTE( "5003-c4.bin", 0x1000001, 0x800000, CRC(afef5d66) SHA1(39fe785563fbea54bba88de60dcc62e2458bd74a) )
	ROM_LOAD16_BYTE( "5003-c5.bin", 0x2000000, 0x800000, CRC(c7c1ae50) SHA1(f54f5be7513a5ce2f01ab107a2b26f6a9ee1f2a9) )
	ROM_LOAD16_BYTE( "5003-c6.bin", 0x2000001, 0x800000, CRC(613197f9) SHA1(6d1fefa1be81b79e251e55a1352544c0298e4674) )
	ROM_LOAD16_BYTE( "5003-c7.bin", 0x3000000, 0x800000, CRC(64ddfe0f) SHA1(361f3f4618009bf6419961266eb9ab5002bef53c) )
	ROM_LOAD16_BYTE( "5003-c8.bin", 0x3000001, 0x800000, CRC(917a1439) SHA1(6f28d1d7c6edee1283f25e632c69204dbebe40af) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( ct2k3sps )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "5003-p1s.bin",  0x000000, 0x100000, CRC(ab5c4de0) SHA1(ca9a6bfd4c32d791ecabb34ccbf2cbf0e84f97d5) )
	ROM_LOAD16_WORD_SWAP( "5003-p2.bin",   0x100000, 0x400000, CRC(adc1c22b) SHA1(271e0629989257a0d21d280c05df53df259414b1) )

	NEO_SFIX_128K( "5003s03.bin", CRC(7c1759b2) SHA1(41a1a1f2b22d9f4b0d500128511dbfc3a91c7ff8) )

	NEO_BIOS_AUDIO_128K( "5003-m1.bin", CRC(1a8c274b) SHA1(5f6f9c533f4a296a18c741ce59a69cf6f5c836b9) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "5003-c1.bin", 0x0000000, 0x800000, CRC(68f54b67) SHA1(e2869709b11ea2846799fe431211c83e928e103e) )
	ROM_LOAD16_BYTE( "5003-c2.bin", 0x0000001, 0x800000, CRC(2f8849d5) SHA1(7ef74981aa056f5acab4ddabffd3e98b4cb970be) )
	ROM_LOAD16_BYTE( "5003-c3.bin", 0x1000000, 0x800000, CRC(ac4aff71) SHA1(c983f642e68deaa40fee3e208f2dd55f3bacbdc1) )
	ROM_LOAD16_BYTE( "5003-c4.bin", 0x1000001, 0x800000, CRC(afef5d66) SHA1(39fe785563fbea54bba88de60dcc62e2458bd74a) )
	ROM_LOAD16_BYTE( "5003-c5.bin", 0x2000000, 0x800000, CRC(c7c1ae50) SHA1(f54f5be7513a5ce2f01ab107a2b26f6a9ee1f2a9) )
	ROM_LOAD16_BYTE( "5003-c6.bin", 0x2000001, 0x800000, CRC(613197f9) SHA1(6d1fefa1be81b79e251e55a1352544c0298e4674) )
	ROM_LOAD16_BYTE( "5003-c7.bin", 0x3000000, 0x800000, CRC(64ddfe0f) SHA1(361f3f4618009bf6419961266eb9ab5002bef53c) )
	ROM_LOAD16_BYTE( "5003-c8.bin", 0x3000001, 0x800000, CRC(917a1439) SHA1(6f28d1d7c6edee1283f25e632c69204dbebe40af) )
ROM_END

ROM_START( ct2k3spd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "360spd.p1", 0x000000, 0x100000, CRC(013a509d) SHA1(c61c9b777e6e062b5f4ad87cdb78e9ca05e9bfb9) )
	ROM_LOAD16_WORD_SWAP( "360d.p2", 0x100000, 0x400000, CRC(eba65bda) SHA1(e66755cb11e8b16f1af68c1439dd0ec485573c10) )

	NEO_SFIX_128K( "360spd.s1", CRC(b86c8ba0) SHA1(cfb8f317c061899343f2c80ea16da131fd50a6e7) )

	NEO_BIOS_AUDIO_128K( "360d.m1", CRC(526cccab) SHA1(f7931f42e6f27c3da1902a552a983ca588e2418b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "360d.c1", 0x0000000, 0x800000, CRC(29fd9108) SHA1(97e0c01692bffddf5c157c6a9e56573861029335) )
	ROM_LOAD16_BYTE( "360d.c2", 0x0000001, 0x800000, CRC(f58d4d3e) SHA1(d4276d4c9424fad2966cb3d49ac8f330b12ef7b6) )
	ROM_LOAD16_BYTE( "360d.c3", 0x1000000, 0x800000, CRC(71b3172d) SHA1(1b742b63e726f537df437df36b1bce35568c092a) )
	ROM_LOAD16_BYTE( "360d.c4", 0x1000001, 0x800000, CRC(564c70c1) SHA1(c67977f5be5a1cc9fbce4450a844967c019142c0) )
	ROM_LOAD16_BYTE( "360d.c5", 0x2000000, 0x800000, CRC(8ef8aef9) SHA1(ef464293c4fd720d6f59d243af9b064bc680c9f9) )
	ROM_LOAD16_BYTE( "360d.c6", 0x2000001, 0x800000, CRC(8a0fd440) SHA1(74428817d08d331a0bdd4a749ea578dff982f028) )
	ROM_LOAD16_BYTE( "360d.c7", 0x3000000, 0x800000, CRC(6f1effab) SHA1(a131bece8510f403ff1af4a52a03258a9714de4a) )
	ROM_LOAD16_BYTE( "360d.c8", 0x3000001, 0x800000, CRC(39550d3a) SHA1(8dbf219da2b39642c316164b8c28bcb350346250) )
ROM_END

ROM_START( cthd2k3d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "360d.p1", 0x000000, 0x100000, CRC(f75508d8) SHA1(c6611061f19c7b7eeafd0a36c084e6d437607781) )
	ROM_LOAD16_WORD_SWAP( "360d.p2", 0x100000, 0x400000, CRC(eba65bda) SHA1(e66755cb11e8b16f1af68c1439dd0ec485573c10) )

	NEO_SFIX_128K( "360d.s1", CRC(fc1f3809) SHA1(238bcb33cb1a1cb695f2532b04b4a7c639aad895) )

	NEO_BIOS_AUDIO_128K( "360d.m1", CRC(526cccab) SHA1(f7931f42e6f27c3da1902a552a983ca588e2418b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "360d.c1", 0x0000000, 0x800000, CRC(29fd9108) SHA1(97e0c01692bffddf5c157c6a9e56573861029335) )
	ROM_LOAD16_BYTE( "360d.c2", 0x0000001, 0x800000, CRC(f58d4d3e) SHA1(d4276d4c9424fad2966cb3d49ac8f330b12ef7b6) )
	ROM_LOAD16_BYTE( "360d.c3", 0x1000000, 0x800000, CRC(71b3172d) SHA1(1b742b63e726f537df437df36b1bce35568c092a) )
	ROM_LOAD16_BYTE( "360d.c4", 0x1000001, 0x800000, CRC(564c70c1) SHA1(c67977f5be5a1cc9fbce4450a844967c019142c0) )
	ROM_LOAD16_BYTE( "360d.c5", 0x2000000, 0x800000, CRC(8ef8aef9) SHA1(ef464293c4fd720d6f59d243af9b064bc680c9f9) )
	ROM_LOAD16_BYTE( "360d.c6", 0x2000001, 0x800000, CRC(8a0fd440) SHA1(74428817d08d331a0bdd4a749ea578dff982f028) )
	ROM_LOAD16_BYTE( "360d.c7", 0x3000000, 0x800000, CRC(6f1effab) SHA1(a131bece8510f403ff1af4a52a03258a9714de4a) )
	ROM_LOAD16_BYTE( "360d.c8", 0x3000001, 0x800000, CRC(39550d3a) SHA1(8dbf219da2b39642c316164b8c28bcb350346250) )
ROM_END

ROM_START( ct2k3ad )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "360k3ad.p1", 0x000000, 0x100000, CRC(83783954) SHA1(963a352bdc755b7c9cdf6b5f1c6b4f790061d6c3) )
	ROM_LOAD16_WORD_SWAP( "360k3ad.p2", 0x100000, 0x400000, CRC(84b0b164) SHA1(1d7de20d8fc71c2e74a4fda17a2f41e2a6f027ba) )

	NEO_SFIX_128K( "360k3ad.s1", CRC(956d8273) SHA1(15a0b51a194d6bddfa87d772837a4569fad59760) )

	NEO_BIOS_AUDIO_128K( "360k3ad.m1", CRC(3ee21b7e) SHA1(6d74b2154f2b955c0f355306427b5b308acf6bff) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "360a.v2",         0x400000, 0x400000, CRC(2b498449) SHA1(8e2b01dd17dbf4498a197bda8ce3e08cdf785f12) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "360k3ad.c1", 0x0000000, 0x800000, CRC(eddd5e2f) SHA1(2af647a57ec12ed60c37a3469adf3e58cb6728c6) )
	ROM_LOAD16_BYTE( "360k3ad.c2", 0x0000001, 0x800000, CRC(23d8d1d6) SHA1(b831e2d506bf8c1436b48d34fc6e2d321602434d) )
	ROM_LOAD16_BYTE( "360d.c3", 0x1000000, 0x800000, CRC(71b3172d) SHA1(1b742b63e726f537df437df36b1bce35568c092a) )
	ROM_LOAD16_BYTE( "360d.c4", 0x1000001, 0x800000, CRC(564c70c1) SHA1(c67977f5be5a1cc9fbce4450a844967c019142c0) )
	ROM_LOAD16_BYTE( "360d.c5", 0x2000000, 0x800000, CRC(8ef8aef9) SHA1(ef464293c4fd720d6f59d243af9b064bc680c9f9) )
	ROM_LOAD16_BYTE( "360d.c6", 0x2000001, 0x800000, CRC(8a0fd440) SHA1(74428817d08d331a0bdd4a749ea578dff982f028) )
	ROM_LOAD16_BYTE( "360k3ad.c7", 0x3000000, 0x800000, CRC(4f50e1ae) SHA1(676ecba2b3c5f1def2822559c1c1299d4eae503f) )
	ROM_LOAD16_BYTE( "360k3ad.c8", 0x3000001, 0x800000, CRC(4f5f09bf) SHA1(1c2d66e793f7943c2c3bbc8390d455001f07baa4) )
ROM_END

ROM_START( cthd2k3a )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "360a.p1", 0x000000, 0x100000, CRC(1185fe39) SHA1(f7ce0878180858c359f125990fd750ec846f42dd) )
	ROM_LOAD16_WORD_SWAP( "360a.p2", 0x100000, 0x400000, CRC(ea71faf7) SHA1(5d1bb12d04a5e2db6f48b59cae5f9b02acaeb976) )

	NEO_SFIX_128K( "360a.s1", CRC(174ccffd) SHA1(8067e4d79ac91f5c18855793840f41c30825cbb4) )

	NEO_BIOS_AUDIO_128K( "360a.m1", CRC(a37cc447) SHA1(969375ca70cf9140afaf833378716cc43e25c5c6) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "360a.v2",         0x400000, 0x400000, CRC(2b498449) SHA1(8e2b01dd17dbf4498a197bda8ce3e08cdf785f12) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "360a.c1",     0x0000000, 0x800000, CRC(dc90c563) SHA1(a79f54f754a1bd0d603f70af6b335e839b5e20d1) )
	ROM_LOAD16_BYTE( "360a.c2",     0x0000001, 0x800000, CRC(7b08d331) SHA1(40e259cbc888184aa0f8102d00098f37f3005ce3) )
	ROM_LOAD16_BYTE( "5003-c3.bin", 0x1000000, 0x800000, CRC(ac4aff71) SHA1(c983f642e68deaa40fee3e208f2dd55f3bacbdc1) )
	ROM_LOAD16_BYTE( "5003-c4.bin", 0x1000001, 0x800000, CRC(afef5d66) SHA1(39fe785563fbea54bba88de60dcc62e2458bd74a) )
	ROM_LOAD16_BYTE( "5003-c5.bin", 0x2000000, 0x800000, CRC(c7c1ae50) SHA1(f54f5be7513a5ce2f01ab107a2b26f6a9ee1f2a9) )
	ROM_LOAD16_BYTE( "5003-c6.bin", 0x2000001, 0x800000, CRC(613197f9) SHA1(6d1fefa1be81b79e251e55a1352544c0298e4674) )
	ROM_LOAD16_BYTE( "360a.c7",     0x3000000, 0x800000, CRC(ad2d72b3) SHA1(6396ba51f863019059ce3e37dab20a60d3a8c549) )
	ROM_LOAD16_BYTE( "360a.c8",     0x3000001, 0x800000, CRC(8c3fc1b5) SHA1(81b6df7c937915c5b971bde20786aece02404a05) )
ROM_END

  /**********************
 Blue's Journey / Raguy
**************************/

ROM_START( bjourneyhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "022_hack01.p1", 0x000000, 0x100000, CRC(1e8b5847) SHA1(59bfff5d8459b24bd3d403a73440ced5c440fd2b) )

	NEO_SFIX_128K( "022.s1", CRC(843c3624) SHA1(dbdf86c193b7c1d795f8c21f2c103c1d3e18abbe) )

	NEO_BIOS_AUDIO_128K( "022.m1", CRC(8e1d4ab6) SHA1(deabc11ab81e7e68a3e041c03a127ae28d0d7264) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "022.v11", 0x000000, 0x100000, CRC(2cb4ad91) SHA1(169ec7303c4275155a66a88cc08270c24132bb36) )
	ROM_LOAD( "022.v22", 0x100000, 0x100000, CRC(65a54d13) SHA1(a591fbcedca8f679dacbebcd554e3aa3fd163e92) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "022.c1", 0x000000, 0x100000, CRC(4d47a48c) SHA1(6e282285be72583d828e7765b1c1695ecdc44777) )
	ROM_LOAD16_BYTE( "022.c2", 0x000001, 0x100000, CRC(e8c1491a) SHA1(c468d2556b3de095aaa05edd1bc16d71303e9478) )
	ROM_LOAD16_BYTE( "022.c3", 0x200000, 0x080000, CRC(66e69753) SHA1(974b823fc62236fbc23e727f25b61a805a707a9e) )
	ROM_LOAD16_BYTE( "022.c4", 0x200001, 0x080000, CRC(71bfd48a) SHA1(47288be69e6992d09ebef108b4de9ffab6293dc8) )
ROM_END

 /************
 Blazing Star
***************/

ROM_START( blazstarhack01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "239_hack01.p1",  0x000000, 0x100000, CRC(077ba687) SHA1(0b62fafdf86b1d1d277c965ef79e2b1a234f4cc9) )
	ROM_LOAD16_WORD_SWAP( "239.p2", 0x100000, 0x200000, CRC(9a9f4154) SHA1(f8805453d0995c8fa16cd9accfb7a990071ca630) )

	NEO_SFIX_128K( "239.s1", CRC(d56cb498) SHA1(420ce56431dc7f3f7de84fcbc8c0a17b5eab205e) )

	NEO_BIOS_AUDIO_128K( "239.m1", CRC(d31a3aea) SHA1(e23abfeb23052f0358edcf2c83401025fe632511) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "239.v1", 0x000000, 0x400000, CRC(1b8d5bf7) SHA1(67fc1f7e36e92a89cd1d415eb31a2892f57b0d04) )
	ROM_LOAD( "239.v2", 0x400000, 0x400000, CRC(74cf0a70) SHA1(b00451a2a30de2517ae3eca35eb1fe985b950eb8) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "239.c1", 0x0000000, 0x400000, CRC(84f6d584) SHA1(ff36db8504611b0d8d942d1e24823ff71e4aeb37) )
	ROM_LOAD16_BYTE( "239.c2", 0x0000001, 0x400000, CRC(05a0cb22) SHA1(4abe03e7f3a86f277131d413a3151c7b9c3646c8) )
	ROM_LOAD16_BYTE( "239.c3", 0x0800000, 0x400000, CRC(5fb69c9e) SHA1(77b96518d8ad8ad120537e0f8ba65d69d1c33566) )
	ROM_LOAD16_BYTE( "239.c4", 0x0800001, 0x400000, CRC(0be028c4) SHA1(d3f8b37786ca7838c3525895a7f2b49afc8530d4) )
	ROM_LOAD16_BYTE( "239.c5", 0x1000000, 0x400000, CRC(74bae5f8) SHA1(812c9a31f0721c2971a316b084ce69337dbe3747) )
	ROM_LOAD16_BYTE( "239.c6", 0x1000001, 0x400000, CRC(4e0700d2) SHA1(cd059fb713c403208923c17e1e8ef02fcfd2fe8d) )
	ROM_LOAD16_BYTE( "239.c7", 0x1800000, 0x400000, CRC(010ff4fd) SHA1(2571d406442f007a7458d8ccb0939a9201c9c9bf) )
	ROM_LOAD16_BYTE( "239.c8", 0x1800001, 0x400000, CRC(db60460e) SHA1(a5cb27c0983c8b400d96fd0828ef0639a66d4dba) )
ROM_END

 /********
 Breakers
***********/

ROM_START( breakershack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "230_hack01.p1", 0x100000, 0x100000, CRC(1d708f96) SHA1(92604d61ed4f6101bdbf6e4352ab28a292e9d0cf) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "230.s1", CRC(076fb64c) SHA1(c166038128d7004f69932141f83b320a35c2b4ca) )

	NEO_BIOS_AUDIO_128K( "230.m1", CRC(3951a1c1) SHA1(1e6442a7ea82ada9503d71045dd93e12bd05254f) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "230.v1", 0x000000, 0x400000, CRC(7f9ed279) SHA1(acd0558960ec29bfc3e3ee99d00e503bebff8513) )
	ROM_LOAD( "230.v2", 0x400000, 0x400000, CRC(1d43e420) SHA1(26d09b8b18b4b802dbda4d6f06626c24d0b7c512) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "230.c1", 0x000000, 0x400000, CRC(68d4ae76) SHA1(2e820067f6963669f104bebf19e865fe4127b4dd) )
	ROM_LOAD16_BYTE( "230.c2", 0x000001, 0x400000, CRC(fdee05cd) SHA1(efc4ffd790953ac7c25d5f045c64a9b49d24b096) )
	ROM_LOAD16_BYTE( "230.c3", 0x800000, 0x400000, CRC(645077f3) SHA1(0ae74f3b4b3b88f128c6d8c0f35ffa53f5d67ef2) )
	ROM_LOAD16_BYTE( "230.c4", 0x800001, 0x400000, CRC(63aeb74c) SHA1(9ff6930c0c3d79b46b86356e8565ce4fcd69ac38) )
ROM_END

 /****************
 Breakers Revenge
*******************/

ROM_START( breakrevhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "245_hack01.p1", 0x100000, 0x100000, CRC(52c978b5) SHA1(892712457ba8c789b136b35ba423f5260c20b478) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "245.s1", CRC(e7660a5d) SHA1(1cd54964ba60b245ea57d9daf0e27b572b815d21) )

	NEO_BIOS_AUDIO_128K( "245.m1", CRC(00f31c66) SHA1(8488598415c9b74bce00e05b31d96e3d1625c20d) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "245.v1", 0x000000, 0x400000, CRC(e255446c) SHA1(b3933340d49d4ba581f3bf1af7ad69d786205790) )
	ROM_LOAD( "245.v2", 0x400000, 0x400000, CRC(9068198a) SHA1(71819b0475a5e173a2f9a6e4ff19a94655141c3c) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "245.c1", 0x0000000, 0x400000, CRC(68d4ae76) SHA1(2e820067f6963669f104bebf19e865fe4127b4dd) )
	ROM_LOAD16_BYTE( "245.c2", 0x0000001, 0x400000, CRC(fdee05cd) SHA1(efc4ffd790953ac7c25d5f045c64a9b49d24b096) )
	ROM_LOAD16_BYTE( "245.c3", 0x0800000, 0x400000, CRC(645077f3) SHA1(0ae74f3b4b3b88f128c6d8c0f35ffa53f5d67ef2) )
	ROM_LOAD16_BYTE( "245.c4", 0x0800001, 0x400000, CRC(63aeb74c) SHA1(9ff6930c0c3d79b46b86356e8565ce4fcd69ac38) )
	ROM_LOAD16_BYTE( "245.c5", 0x1000000, 0x400000, CRC(b5f40e7f) SHA1(b332bac64dbb9a9dd66c5315f47ea08634d36f45) )
	ROM_LOAD16_BYTE( "245.c6", 0x1000001, 0x400000, CRC(d0337328) SHA1(dff86b75dc283bd4512557a5c64f16e6be6c16e4) )
ROM_END

ROM_START( breakrevhack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "245_hack02.p1", 0x100000, 0x100000, CRC(db938d1e) SHA1(7e25134d344bf3816e37df320e4ff29d743cea37) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "245.s1", CRC(e7660a5d) SHA1(1cd54964ba60b245ea57d9daf0e27b572b815d21) )

	NEO_BIOS_AUDIO_128K( "245.m1", CRC(00f31c66) SHA1(8488598415c9b74bce00e05b31d96e3d1625c20d) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "245.v1", 0x000000, 0x400000, CRC(e255446c) SHA1(b3933340d49d4ba581f3bf1af7ad69d786205790) )
	ROM_LOAD( "245.v2", 0x400000, 0x400000, CRC(9068198a) SHA1(71819b0475a5e173a2f9a6e4ff19a94655141c3c) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "245.c1", 0x0000000, 0x400000, CRC(68d4ae76) SHA1(2e820067f6963669f104bebf19e865fe4127b4dd) )
	ROM_LOAD16_BYTE( "245.c2", 0x0000001, 0x400000, CRC(fdee05cd) SHA1(efc4ffd790953ac7c25d5f045c64a9b49d24b096) )
	ROM_LOAD16_BYTE( "245.c3", 0x0800000, 0x400000, CRC(645077f3) SHA1(0ae74f3b4b3b88f128c6d8c0f35ffa53f5d67ef2) )
	ROM_LOAD16_BYTE( "245.c4", 0x0800001, 0x400000, CRC(63aeb74c) SHA1(9ff6930c0c3d79b46b86356e8565ce4fcd69ac38) )
	ROM_LOAD16_BYTE( "245.c5", 0x1000000, 0x400000, CRC(b5f40e7f) SHA1(b332bac64dbb9a9dd66c5315f47ea08634d36f45) )
	ROM_LOAD16_BYTE( "245.c6", 0x1000001, 0x400000, CRC(d0337328) SHA1(dff86b75dc283bd4512557a5c64f16e6be6c16e4) )
ROM_END

ROM_START( breakrevhack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "245_hack03.p1", 0x100000, 0x100000, CRC(9985631a) SHA1(9d312fc72f6b399c4237cc0937ecf4c8856afff7) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "245.s1", CRC(e7660a5d) SHA1(1cd54964ba60b245ea57d9daf0e27b572b815d21) )

	NEO_BIOS_AUDIO_128K( "245.m1", CRC(00f31c66) SHA1(8488598415c9b74bce00e05b31d96e3d1625c20d) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "245.v1", 0x000000, 0x400000, CRC(e255446c) SHA1(b3933340d49d4ba581f3bf1af7ad69d786205790) )
	ROM_LOAD( "245.v2", 0x400000, 0x400000, CRC(9068198a) SHA1(71819b0475a5e173a2f9a6e4ff19a94655141c3c) )

	ROM_REGION( 0x1400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "245.c1", 0x0000000, 0x400000, CRC(68d4ae76) SHA1(2e820067f6963669f104bebf19e865fe4127b4dd) )
	ROM_LOAD16_BYTE( "245.c2", 0x0000001, 0x400000, CRC(fdee05cd) SHA1(efc4ffd790953ac7c25d5f045c64a9b49d24b096) )
	ROM_LOAD16_BYTE( "245.c3", 0x0800000, 0x400000, CRC(645077f3) SHA1(0ae74f3b4b3b88f128c6d8c0f35ffa53f5d67ef2) )
	ROM_LOAD16_BYTE( "245.c4", 0x0800001, 0x400000, CRC(63aeb74c) SHA1(9ff6930c0c3d79b46b86356e8565ce4fcd69ac38) )
	ROM_LOAD16_BYTE( "245s03.c5", 0x1000000, 0x200000, CRC(28ff1792) SHA1(0cc5c16ac42f52cee74f88235aef0671bee33b4c) )
	ROM_LOAD16_BYTE( "245s03.c6", 0x1000001, 0x200000, CRC(23c65644) SHA1(9dc74c3075cf0f738b6b41e5e8d89a74a6c9ef07) )
ROM_END


/**************
 Burning Fight
****************/

ROM_START( burningfhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "018_hack01.p1", 0x000000, 0x080000, CRC(da26031b) SHA1(44997a8c313356db48716bb007ab842473805d88) )

	NEO_SFIX_128K( "018.s1", CRC(6799ea0d) SHA1(ec75ef9dfdcb0b123574fc6d81ebaaadfba32fb5) )

	NEO_BIOS_AUDIO_128K( "018.m1", CRC(0c939ee2) SHA1(57d580d3279e66b9fe66bbcc68529d3384a926ff) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "018.v1", 0x000000, 0x100000, CRC(508c9ffc) SHA1(cd3a97a233a4585f8507116aba85884623cccdc4) )
	ROM_LOAD( "018.v2", 0x100000, 0x100000, CRC(854ef277) SHA1(4b3083b9c80620064cb44e812a787a700e32a6f3) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "018.c1", 0x000000, 0x100000, CRC(25a25e9b) SHA1(3cf02d0662e190678d0530d7b7d3f425209adf83) )
	ROM_LOAD16_BYTE( "018.c2", 0x000001, 0x100000, CRC(d4378876) SHA1(45659aa1755d96b992c977042186e47fff68bba9) )
	ROM_LOAD16_BYTE( "018.c3", 0x200000, 0x100000, CRC(862b60da) SHA1(e2303eb1609f1050f0b4f46693a15e37deb176fb) )
	ROM_LOAD16_BYTE( "018.c4", 0x200001, 0x100000, CRC(e2e0aff7) SHA1(1c691c092a6e2787de4f433b0eb9252bfdaa7e16) )
ROM_END

/****************
 Dragon's Heaven
******************/

ROM_START( ctomadayhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "249_hack01.p1", 0x100000, 0x100000, CRC(734b3c0b) SHA1(5f08a3f509ea1fa56bf8b66d996f259380fecdaa) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "249.s1", CRC(dc9eb372) SHA1(b8aa142243ba303799554479bfc88eb49260f3b1) )

	NEO_BIOS_AUDIO_128K( "249.m1", CRC(80328a47) SHA1(34b6b1a81eab1cf38834b2eea55454ce1b6100e2) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "249.v1", 0x000000, 0x400000, CRC(de7c8f27) SHA1(3681a68a702ab5da8f509b8301d6cada75959332) )
	ROM_LOAD( "249.v2", 0x400000, 0x100000, CRC(c8e40119) SHA1(738f525c381ed68c0b8a89318a3e4d0089473c45) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "249.c1", 0x000000, 0x400000, CRC(041fb8ee) SHA1(dacc84d713d76818d89a26358374afaa22fa82a2) )
	ROM_LOAD16_BYTE( "249.c2", 0x000001, 0x400000, CRC(74f3cdf4) SHA1(55ddabaf77f4d575f4deb24fe63e4bdc2c6f31e1) )
ROM_END

 /*************
 Double Dragon
****************/

ROM_START( doubledrhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "082_hack01.p1", 0x100000, 0x100000, CRC(92826c06) SHA1(4a6792d705a4be8fa1fc6d559d5d417e8cbf7d40) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "082.s1", CRC(bef995c5) SHA1(9c89adbdaa5c1f827632c701688563dac2e482a4) )

	NEO_BIOS_AUDIO_128K( "082.m1", CRC(10b144de) SHA1(cf1ed0a447da68240c62bcfd76b1569803f6bf76) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "082.v1", 0x000000, 0x200000, CRC(cc1128e4) SHA1(bfcfff24bc7fbde0b02b1bc0dffebd5270a0eb04) )
	ROM_LOAD( "082.v2", 0x200000, 0x200000, CRC(c3ff5554) SHA1(c685887ad64998e5572607a916b023f8b9efac49) )

	ROM_REGION( 0xe00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "082.c1", 0x000000, 0x200000, CRC(b478c725) SHA1(3a777c5906220f246a6dc06cb084e6ad650d67bb) )
	ROM_LOAD16_BYTE( "082.c2", 0x000001, 0x200000, CRC(2857da32) SHA1(9f13245965d23db86d46d7e73dfb6cc63e6f25a1) )
	ROM_LOAD16_BYTE( "082.c3", 0x400000, 0x200000, CRC(8b0d378e) SHA1(3a347215e414b738164f1fe4144102f07d4ffb80) )
	ROM_LOAD16_BYTE( "082.c4", 0x400001, 0x200000, CRC(c7d2f596) SHA1(e2d09d4d1b1fef9c0c53ecf3629e974b75e559f5) )
	ROM_LOAD16_BYTE( "082.c5", 0x800000, 0x200000, CRC(ec87bff6) SHA1(3fa86da93881158c2c23443855922a7b32e55135) )
	ROM_LOAD16_BYTE( "082.c6", 0x800001, 0x200000, CRC(844a8a11) SHA1(b2acbd4cacce66fb32c052b2fba9984904679bda) )
	ROM_LOAD16_BYTE( "082.c7", 0xc00000, 0x100000, CRC(727c4d02) SHA1(8204c7f037d46e0c58f269f9c7a535bc2589f526) )
	ROM_LOAD16_BYTE( "082.c8", 0xc00001, 0x100000, CRC(69a5fa37) SHA1(020e70e0e8b3c5d00a40fe97e418115a3187e50a) )
ROM_END

ROM_START( doubledrhack02 ) //doubledrcn
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "082.p1", 0x100000, 0x100000, CRC(34ab832a) SHA1(fbb1bd195f5653f7b9c89648649f838eaf83cbe4) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "082_hack02.s1", CRC(8d99684b) SHA1(4cea5bb537a2768f73d0b0c2ff9997ea4163c18b) )

	NEO_BIOS_AUDIO_128K( "082.m1", CRC(10b144de) SHA1(cf1ed0a447da68240c62bcfd76b1569803f6bf76) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "082.v1", 0x000000, 0x200000, CRC(cc1128e4) SHA1(bfcfff24bc7fbde0b02b1bc0dffebd5270a0eb04) )
	ROM_LOAD( "082.v2", 0x200000, 0x200000, CRC(c3ff5554) SHA1(c685887ad64998e5572607a916b023f8b9efac49) )

	ROM_REGION( 0xe00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "082_hack02.c1", 0x000000, 0x200000, CRC(3318463d) SHA1(34f65349cad2931988d39f2de9f3b955effa306a) )
	ROM_LOAD16_BYTE( "082_hack02.c2", 0x000001, 0x200000, CRC(73d73a11) SHA1(01a328dc01cd761cfa7fb371fe9c711d1c4a0845) )
	ROM_LOAD16_BYTE( "082_hack02.c3", 0x400000, 0x200000, CRC(eb8d33f1) SHA1(4eeadfecc59ca9ca7f4006da025b6a9e6478914f) )
	ROM_LOAD16_BYTE( "082_hack02.c4", 0x400001, 0x200000, CRC(d823f351) SHA1(6b8e1c7dc78da04e72c3be063800bb5dede4e3a9) )
	ROM_LOAD16_BYTE( "082_hack02.c5", 0x800000, 0x200000, CRC(ce6756e9) SHA1(1d85dadfedd73e1569085cfc711773804e107eae) )
	ROM_LOAD16_BYTE( "082_hack02.c6", 0x800001, 0x200000, CRC(2357d930) SHA1(89c8a3eb8e2a8ff928615d0c239c0b0d045185f2) )
	ROM_LOAD16_BYTE( "082_hack02.c7", 0xc00000, 0x100000, CRC(fbdf60a1) SHA1(1297b64cabf7a44968200e3d5a89c8efdb0e1625) )
	ROM_LOAD16_BYTE( "082_hack02.c8", 0xc00001, 0x100000, CRC(bca22b14) SHA1(399ab7689387878fb534ddff675540936149954d) )
ROM_END

ROM_START( doubledrhack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "082_hack03.p1", 0x100000, 0x100000, CRC(1ca0941f) SHA1(40de27c5019059ed97b3bd7d8178e64709513114) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "082.s1", CRC(bef995c5) SHA1(9c89adbdaa5c1f827632c701688563dac2e482a4) )

	NEO_BIOS_AUDIO_128K( "082.m1", CRC(10b144de) SHA1(cf1ed0a447da68240c62bcfd76b1569803f6bf76) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "082.v1", 0x000000, 0x200000, CRC(cc1128e4) SHA1(bfcfff24bc7fbde0b02b1bc0dffebd5270a0eb04) )
	ROM_LOAD( "082.v2", 0x200000, 0x200000, CRC(c3ff5554) SHA1(c685887ad64998e5572607a916b023f8b9efac49) )

	ROM_REGION( 0xe00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "082.c1", 0x000000, 0x200000, CRC(b478c725) SHA1(3a777c5906220f246a6dc06cb084e6ad650d67bb) )
	ROM_LOAD16_BYTE( "082.c2", 0x000001, 0x200000, CRC(2857da32) SHA1(9f13245965d23db86d46d7e73dfb6cc63e6f25a1) )
	ROM_LOAD16_BYTE( "082.c3", 0x400000, 0x200000, CRC(8b0d378e) SHA1(3a347215e414b738164f1fe4144102f07d4ffb80) )
	ROM_LOAD16_BYTE( "082.c4", 0x400001, 0x200000, CRC(c7d2f596) SHA1(e2d09d4d1b1fef9c0c53ecf3629e974b75e559f5) )
	ROM_LOAD16_BYTE( "082.c5", 0x800000, 0x200000, CRC(ec87bff6) SHA1(3fa86da93881158c2c23443855922a7b32e55135) )
	ROM_LOAD16_BYTE( "082.c6", 0x800001, 0x200000, CRC(844a8a11) SHA1(b2acbd4cacce66fb32c052b2fba9984904679bda) )
	ROM_LOAD16_BYTE( "082.c7", 0xc00000, 0x100000, CRC(727c4d02) SHA1(8204c7f037d46e0c58f269f9c7a535bc2589f526) )
	ROM_LOAD16_BYTE( "082.c8", 0xc00001, 0x100000, CRC(69a5fa37) SHA1(020e70e0e8b3c5d00a40fe97e418115a3187e50a) )
ROM_END

ROM_START( doubledrhack04 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "082_hack04.p1", 0x000000, 0x100000, CRC(8ea8ee3d) SHA1(4cc513f9021a5a6bfe29ebf2773847c674f7921b) )
	ROM_LOAD16_WORD_SWAP( "082_hack04.p2", 0x100000, 0x100000, CRC(0e2616ab) SHA1(cfe5ed1ec76e21dd833e8297a6dbb30ce407ab2d) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "082_hack04.p3", 0x000000, 0x020000, CRC(8b4839c4) SHA1(6c0357f8455bc4a100e1063a5be88c8be388672c) )

	NEO_SFIX_128K( "082.s1", CRC(bef995c5) SHA1(9c89adbdaa5c1f827632c701688563dac2e482a4) )

	NEO_BIOS_AUDIO_128K( "082.m1", CRC(10b144de) SHA1(cf1ed0a447da68240c62bcfd76b1569803f6bf76) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "082.v1", 0x000000, 0x200000, CRC(cc1128e4) SHA1(bfcfff24bc7fbde0b02b1bc0dffebd5270a0eb04) )
	ROM_LOAD( "082.v2", 0x200000, 0x200000, CRC(c3ff5554) SHA1(c685887ad64998e5572607a916b023f8b9efac49) )

	ROM_REGION( 0xe00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "082.c1", 0x000000, 0x200000, CRC(b478c725) SHA1(3a777c5906220f246a6dc06cb084e6ad650d67bb) )
	ROM_LOAD16_BYTE( "082.c2", 0x000001, 0x200000, CRC(2857da32) SHA1(9f13245965d23db86d46d7e73dfb6cc63e6f25a1) )
	ROM_LOAD16_BYTE( "082.c3", 0x400000, 0x200000, CRC(8b0d378e) SHA1(3a347215e414b738164f1fe4144102f07d4ffb80) )
	ROM_LOAD16_BYTE( "082.c4", 0x400001, 0x200000, CRC(c7d2f596) SHA1(e2d09d4d1b1fef9c0c53ecf3629e974b75e559f5) )
	ROM_LOAD16_BYTE( "082_hack04.c5", 0x800000, 0x200000, CRC(b9c799fe) SHA1(04d44f6fbee4bf6978031d1e148a536b012ecc8d) )
	ROM_LOAD16_BYTE( "082_hack04.c6", 0x800001, 0x200000, CRC(11569bc9) SHA1(ef937371e0f62ef8cc3d315aa944cacab798a173) )
	ROM_LOAD16_BYTE( "082.c7", 0xc00000, 0x100000, CRC(727c4d02) SHA1(8204c7f037d46e0c58f269f9c7a535bc2589f526) )
	ROM_LOAD16_BYTE( "082.c8", 0xc00001, 0x100000, CRC(69a5fa37) SHA1(020e70e0e8b3c5d00a40fe97e418115a3187e50a) )
ROM_END

/****************
 Dragon's Heaven
******************/

ROM_START( dragons1 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_BYTE( "336.p1", 0x000000, 0x080000, CRC(f25c71ad) SHA1(803fb6cd6a7ada59678ad901ff9788b1e54ddd0c) )
	ROM_LOAD16_BYTE( "336.p2", 0x000001, 0x080000, CRC(f353448c) SHA1(f0f966ca15d503e01b40e901765ff0888463b65d) )

	//NEO_SFIX_128K( "094.s1", CRC(2f8748a2) SHA1(5cc723c4284120473d63d8b0c1a3b3be74bdc324) )
	NEO_SFIX_128K( "336.s1", CRC(706477a7) SHA1(8cbee7f6832e7edd2dc792ca330420a6a984b879) )

	NEO_BIOS_AUDIO_128K( "094.m1", CRC(78c851cb) SHA1(a9923c002e4e2171a564af45cff0958c5d57b275) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "094.v1", 0x000000, 0x200000, CRC(6c31223c) SHA1(ede3a2806d7d872a0f737626a23ecce200b534e6) )
	ROM_LOAD( "094.v2", 0x200000, 0x200000, CRC(8edb776c) SHA1(a9eac5e24f83ccdcf303d63261747b1bad876a24) )
	ROM_LOAD( "094.v3", 0x400000, 0x100000, CRC(c63b9285) SHA1(6bbbacfe899e204e74657d6c3f3d05ce75e432f1) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "336.c1", 0x000000, 0x1000000, CRC(81821826) SHA1(b7c1a53e32633383675206a16c68f6f2ff984865) )
	ROM_LOAD16_BYTE( "336.c2", 0x000001, 0x1000000, CRC(3601d568) SHA1(800323e52f5d33b402f84d31850b42c688082d67) )
ROM_END

/**********
 Eight Man
************/

ROM_START( eightmanhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "025_hack01.p1", 0x000000, 0x080000, CRC(6356e4b9) SHA1(c02fe3fe7c59592c464cba05183fe98b1ac13f54) )

	NEO_SFIX_128K( "025.s1", CRC(a402202b) SHA1(75c44e1af459af155f5b892fd18706268dd5e602) )

	NEO_BIOS_AUDIO_128K( "025.m1", CRC(9927034c) SHA1(205665361c5b2ab4f01ec480dd3c9b69db858d09) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "025.v1", 0x000000, 0x100000, CRC(4558558a) SHA1(a4b277703ed67225c652be0d618daeca65a27b88) )
	ROM_LOAD( "025.v2", 0x100000, 0x100000, CRC(c5e052e9) SHA1(fa1119c90ce4c706a6aa0c17d7bc06aa3068d9b2) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "025.c1", 0x000000, 0x100000, CRC(555e16a4) SHA1(1c96f3d2fd0991680fbf627a6cdd26ad2cd60319) )
	ROM_LOAD16_BYTE( "025.c2", 0x000001, 0x100000, CRC(e1ee51c3) SHA1(da8d074bb4e923ed7b8a154fd31b42f2d65b8e96) )
	ROM_LOAD16_BYTE( "025.c3", 0x200000, 0x080000, CRC(0923d5b0) SHA1(ab72ba1e3ebf56dd356f9ad181f986b1360a1089) )
	ROM_LOAD16_BYTE( "025.c4", 0x200001, 0x080000, CRC(e3eca67b) SHA1(88154cbc1a261c2f425430119ebc08a30adc9675) )
ROM_END

 /**********************
 Fatal Fury Special
*************************/

ROM_START( fatfursphack01 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "058_hack01.p1",  0x000000, 0x100000, CRC(8cd18f7f) SHA1(1f7a550bb5c71495bd52c66ff289f64dfa778ee4) )
	ROM_LOAD16_WORD_SWAP( "058.p2", 0x100000, 0x080000, CRC(d7c71a6b) SHA1(b3428063031a2e5857da40a5d2ffa87fb550c1bb) )

	NEO_SFIX_128K( "058.s1", CRC(2df03197) SHA1(24083cfc97e720ac9e131c9fe37df57e27c49294) )

	NEO_BIOS_AUDIO_128K( "058.m1", CRC(ccc5186e) SHA1(cf9091c523c182aebfb928c91640b2d72fd70123) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "058.v1", 0x000000, 0x200000, CRC(55d7ce84) SHA1(05ac6a395d9bf9166925acca176a8d6129f533c8) )
	ROM_LOAD( "058.v2", 0x200000, 0x200000, CRC(ee080b10) SHA1(29814fc21bbe30d37745c8918fab00c83a309be4) )
	ROM_LOAD( "058.v3", 0x400000, 0x100000, CRC(f9eb3d4a) SHA1(d1747f9460b965f6daf4f881ed4ecd04c5253434) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "058.c1", 0x000000, 0x200000, CRC(044ab13c) SHA1(569d283638a132bc163faac2a9055497017ee0d2) )
	ROM_LOAD16_BYTE( "058.c2", 0x000001, 0x200000, CRC(11e6bf96) SHA1(c093a4f93f13e07b276e28b30c2a14dda9135d8f) )
	ROM_LOAD16_BYTE( "058.c3", 0x400000, 0x200000, CRC(6f7938d5) SHA1(be057b0a3faeb76d5fff161d3e6fea8a26e11d2c) )
	ROM_LOAD16_BYTE( "058.c4", 0x400001, 0x200000, CRC(4ad066ff) SHA1(4e304646d954d5f7bbabc5d068e85de31d38830f) )
	ROM_LOAD16_BYTE( "058.c5", 0x800000, 0x200000, CRC(49c5e0bf) SHA1(f3784178f90751990ea47a082a6aa869ee3566c9) )
	ROM_LOAD16_BYTE( "058.c6", 0x800001, 0x200000, CRC(8ff1f43d) SHA1(6180ceb5412a3e2e34e9513a3283b9f63087f747) )
ROM_END

ROM_START( fatfursphack02 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "058_hack02.p1",  0x000000, 0x100000, CRC(0593468f) SHA1(2a0cd20f53e6537047fd422d63f4c423e9736dac) )
	ROM_LOAD16_WORD_SWAP( "058.p2", 0x100000, 0x080000, CRC(d7c71a6b) SHA1(b3428063031a2e5857da40a5d2ffa87fb550c1bb) )
	ROM_LOAD16_WORD_SWAP( "058a.p1", 0x000000, 0x080000, CRC(9f0c1e1a) SHA1(02861b0f230541becccc3df6a2c85dbe8733e7ce) )

	NEO_SFIX_128K( "058.s1", CRC(2df03197) SHA1(24083cfc97e720ac9e131c9fe37df57e27c49294) )

	NEO_BIOS_AUDIO_128K( "058.m1", CRC(ccc5186e) SHA1(cf9091c523c182aebfb928c91640b2d72fd70123) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "058.v1", 0x000000, 0x200000, CRC(55d7ce84) SHA1(05ac6a395d9bf9166925acca176a8d6129f533c8) )
	ROM_LOAD( "058.v2", 0x200000, 0x200000, CRC(ee080b10) SHA1(29814fc21bbe30d37745c8918fab00c83a309be4) )
	ROM_LOAD( "058.v3", 0x400000, 0x100000, CRC(f9eb3d4a) SHA1(d1747f9460b965f6daf4f881ed4ecd04c5253434) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "058.c1", 0x000000, 0x200000, CRC(044ab13c) SHA1(569d283638a132bc163faac2a9055497017ee0d2) )
	ROM_LOAD16_BYTE( "058.c2", 0x000001, 0x200000, CRC(11e6bf96) SHA1(c093a4f93f13e07b276e28b30c2a14dda9135d8f) )
	ROM_LOAD16_BYTE( "058.c3", 0x400000, 0x200000, CRC(6f7938d5) SHA1(be057b0a3faeb76d5fff161d3e6fea8a26e11d2c) )
	ROM_LOAD16_BYTE( "058.c4", 0x400001, 0x200000, CRC(4ad066ff) SHA1(4e304646d954d5f7bbabc5d068e85de31d38830f) )
	ROM_LOAD16_BYTE( "058.c5", 0x800000, 0x200000, CRC(49c5e0bf) SHA1(f3784178f90751990ea47a082a6aa869ee3566c9) )
	ROM_LOAD16_BYTE( "058.c6", 0x800001, 0x200000, CRC(8ff1f43d) SHA1(6180ceb5412a3e2e34e9513a3283b9f63087f747) )
ROM_END

 /************
 Fatal Fury 1
***************/

ROM_START( fatfury1hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "033_hack01.p1", 0x000000, 0x080000, CRC(fc051e1d) SHA1(515f3f8eb080f794bbde09aa3959a1f01b1b01ef) )
	ROM_LOAD16_WORD_SWAP( "033.p2", 0x080000, 0x020000, CRC(c473af1c) SHA1(4919eeca20abe807493872ca7c79a5d1f496fe68) )

	NEO_SFIX_128K( "033.s1", CRC(3c3bdf8c) SHA1(2f3e5feed6c27850b2a0f6fae0b97041690e944c) )

	NEO_BIOS_AUDIO_128K( "033.m1", CRC(5be10ffd) SHA1(90a5e6cbbc58a7883cd2a3a597180d631a466882) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "033.v1", 0x000000, 0x100000, CRC(212fd20d) SHA1(120c040db8c01a6f140eea03725448bfa9ca98c2) )
	ROM_LOAD( "033.v2", 0x100000, 0x100000, CRC(fa2ae47f) SHA1(80d0ba4cd30aab59b6f0db8fa341387bd7388afc) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "033.c1", 0x000000, 0x100000, CRC(74317e54) SHA1(67b9c2814a12603b959612456f59de55f9bf6f57) )
	ROM_LOAD16_BYTE( "033.c2", 0x000001, 0x100000, CRC(5bb952f3) SHA1(ea964bbcc0408b6ae07cbb5043d003281b1aca15) )
	ROM_LOAD16_BYTE( "033.c3", 0x200000, 0x100000, CRC(9b714a7c) SHA1(b62bdcede3207d062a89e0a4a9adf706101bb681) )
	ROM_LOAD16_BYTE( "033.c4", 0x200001, 0x100000, CRC(9397476a) SHA1(a12dbb74020aeb6ebf24ec2abbfba5129cabcb7d) )
ROM_END

 /************
 Fatal Fury 2
***************/

ROM_START( fatfury2hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "047_hack01.p1", 0x000000, 0x100000, CRC(b594b7b3) SHA1(eca56d2a2f4b5b6a005576f95fb7e73c9be264c9) )

	NEO_SFIX_128K( "047.s1", CRC(d7dbbf39) SHA1(29253e596f475ebd41a6e3bb53952e3a0ccd2eed) )

	NEO_BIOS_AUDIO_128K( "047.m1", CRC(820b0ba7) SHA1(5708248d89446e49184eaadb52f7c61b2b6c13c5) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "047.v1", 0x000000, 0x200000, CRC(d9d00784) SHA1(f6a91eada8c23aa4518c4b82eeebca69f79d845c) )
	ROM_LOAD( "047.v2", 0x200000, 0x200000, CRC(2c9a4b33) SHA1(d4a1c0951c02c8919b3ec32ed96933634ff9e54c) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "047.c1", 0x000000, 0x100000, CRC(f72a939e) SHA1(67fc398ec28061adca0d3be82bbe7297015800da) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "047.c2", 0x000001, 0x100000, CRC(05119a0d) SHA1(c2f100b73eb04f65b6ba6089d49aceb51b470ec6) )
	ROM_CONTINUE( 0x400001, 0x100000 )
	ROM_LOAD16_BYTE( "047.c3", 0x200000, 0x100000, CRC(01e00738) SHA1(79654f24d777dd5eb68bafc3b8cb9db71d5822e2) )
	ROM_CONTINUE( 0x600000, 0x100000 )
	ROM_LOAD16_BYTE( "047.c4", 0x200001, 0x100000, CRC(9fe27432) SHA1(89d22d77ba8bc6d1f6c974195c34ad61b9010de7) )
	ROM_CONTINUE( 0x600001, 0x100000 )
ROM_END

ROM_START( fatfury2hack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "047_hack02.p1", 0x000000, 0x100000, CRC(7afbab91) SHA1(4571c5d033ff99b5dcf868c392cd7f4a5d03c8de) )

	NEO_SFIX_128K( "047.s1", CRC(d7dbbf39) SHA1(29253e596f475ebd41a6e3bb53952e3a0ccd2eed) )

	NEO_BIOS_AUDIO_128K( "047.m1", CRC(820b0ba7) SHA1(5708248d89446e49184eaadb52f7c61b2b6c13c5) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "047.v1", 0x000000, 0x200000, CRC(d9d00784) SHA1(f6a91eada8c23aa4518c4b82eeebca69f79d845c) )
	ROM_LOAD( "047.v2", 0x200000, 0x200000, CRC(2c9a4b33) SHA1(d4a1c0951c02c8919b3ec32ed96933634ff9e54c) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "047.c1", 0x000000, 0x100000, CRC(f72a939e) SHA1(67fc398ec28061adca0d3be82bbe7297015800da) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "047.c2", 0x000001, 0x100000, CRC(05119a0d) SHA1(c2f100b73eb04f65b6ba6089d49aceb51b470ec6) )
	ROM_CONTINUE( 0x400001, 0x100000 )
	ROM_LOAD16_BYTE( "047.c3", 0x200000, 0x100000, CRC(01e00738) SHA1(79654f24d777dd5eb68bafc3b8cb9db71d5822e2) )
	ROM_CONTINUE( 0x600000, 0x100000 )
	ROM_LOAD16_BYTE( "047.c4", 0x200001, 0x100000, CRC(9fe27432) SHA1(89d22d77ba8bc6d1f6c974195c34ad61b9010de7) )
	ROM_CONTINUE( 0x600001, 0x100000 )
ROM_END

 /************
 Fatal Fury 3
***************/

ROM_START( fatfury3hack01 ) 
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "069_hack01.p1",    0x000000, 0x100000, CRC(532c2930) SHA1(A65A39A79820138F275F75563F548240DD53F371) )
	ROM_LOAD16_WORD_SWAP( "069.p2", 0x100000, 0x200000, CRC(dbe963ed) SHA1(8ece7f663cfe8e563576a397e41161d392cee67e) )

	NEO_SFIX_128K( "069.s1", CRC(0b33a800) SHA1(b7d2cc97da4f30ddebc7b801f5e1d17d2306b2db) )

	NEO_BIOS_AUDIO_128K( "069.m1", CRC(fce72926) SHA1(a40c74f793900b8542f0b8383ce4bf46fca112d4) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "069.v1", 0x000000, 0x400000, CRC(2bdbd4db) SHA1(5f4fecf69c2329d699cbd45829c19303b1e2a80e) )
	ROM_LOAD( "069.v2", 0x400000, 0x400000, CRC(a698a487) SHA1(11b8bc53bc26a51f4a408e900e3769958625c4ed) )
	ROM_LOAD( "069.v3", 0x800000, 0x200000, CRC(581c5304) SHA1(e9550ec547b4f605afed996b22d711f49b48fa92) )

	ROM_REGION( 0x1400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "069.c1", 0x0000000, 0x400000, CRC(e302f93c) SHA1(d8610b14900b2b8fe691b67ca9b1abb335dbff74) )
	ROM_LOAD16_BYTE( "069.c2", 0x0000001, 0x400000, CRC(1053a455) SHA1(69501bfac68739e63d798045b812badd251d57b8) )
	ROM_LOAD16_BYTE( "069.c3", 0x0800000, 0x400000, CRC(1c0fde2f) SHA1(cf6c2ef56c03a861de3b0b6dc0d7c9204d947f9d) )
	ROM_LOAD16_BYTE( "069.c4", 0x0800001, 0x400000, CRC(a25fc3d0) SHA1(83cb349e2f1032652060b233e741fb893be5af16) )
	ROM_LOAD16_BYTE( "069.c5", 0x1000000, 0x200000, CRC(b3ec6fa6) SHA1(7e4c8ee9dd8d9a25ff183d9d8b05f38769348bc7) )
	ROM_LOAD16_BYTE( "069.c6", 0x1000001, 0x200000, CRC(69210441) SHA1(6d496c549dba65caabeaffe5b762e86f9d648a26) )
ROM_END

ROM_START( fatfury3hack02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "069_hack02.p1",   0x000000, 0x100000, CRC(b8362f59) SHA1(9ae1cd259d9667fefcba0452ea658cf72aba189e) )
	ROM_LOAD16_WORD_SWAP( "069.p2", 0x100000, 0x200000, CRC(dbe963ed) SHA1(8ece7f663cfe8e563576a397e41161d392cee67e) )

	NEO_SFIX_128K( "069.s1", CRC(0b33a800) SHA1(b7d2cc97da4f30ddebc7b801f5e1d17d2306b2db) )

	NEO_BIOS_AUDIO_128K( "069.m1", CRC(fce72926) SHA1(a40c74f793900b8542f0b8383ce4bf46fca112d4) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "069.v1", 0x000000, 0x400000, CRC(2bdbd4db) SHA1(5f4fecf69c2329d699cbd45829c19303b1e2a80e) )
	ROM_LOAD( "069.v2", 0x400000, 0x400000, CRC(a698a487) SHA1(11b8bc53bc26a51f4a408e900e3769958625c4ed) )
	ROM_LOAD( "069.v3", 0x800000, 0x200000, CRC(581c5304) SHA1(e9550ec547b4f605afed996b22d711f49b48fa92) )

	ROM_REGION( 0x1400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "069.c1", 0x0000000, 0x400000, CRC(e302f93c) SHA1(d8610b14900b2b8fe691b67ca9b1abb335dbff74) )
	ROM_LOAD16_BYTE( "069.c2", 0x0000001, 0x400000, CRC(1053a455) SHA1(69501bfac68739e63d798045b812badd251d57b8) )
	ROM_LOAD16_BYTE( "069.c3", 0x0800000, 0x400000, CRC(1c0fde2f) SHA1(cf6c2ef56c03a861de3b0b6dc0d7c9204d947f9d) )
	ROM_LOAD16_BYTE( "069.c4", 0x0800001, 0x400000, CRC(a25fc3d0) SHA1(83cb349e2f1032652060b233e741fb893be5af16) )
	ROM_LOAD16_BYTE( "069.c5", 0x1000000, 0x200000, CRC(b3ec6fa6) SHA1(7e4c8ee9dd8d9a25ff183d9d8b05f38769348bc7) )
	ROM_LOAD16_BYTE( "069.c6", 0x1000001, 0x200000, CRC(69210441) SHA1(6d496c549dba65caabeaffe5b762e86f9d648a26) )
ROM_END

/************
 Fight Fever
**************/

ROM_START( fightfevhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "060_hack01.p1", 0x0000000, 0x100000, CRC(0b9e106d) SHA1(1704c2b416747288cf37e52e927857819badcb62) )

	NEO_SFIX_128K( "060.s1", CRC(d62a72e9) SHA1(a23e4c4fd4ec11a7467ce41227c418b4dd1ef649) )

	NEO_BIOS_AUDIO_128K( "060.m1", CRC(0b7c4e65) SHA1(999a1e784de18db3f1332b30bc425836ea6970be) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "060.v1", 0x000000, 0x200000, CRC(f417c215) SHA1(0f53b8dd056f43b5d880628e8b74c2b27881ffac) )
	ROM_LOAD( "060.v2", 0x200000, 0x100000, CRC(efcff7cf) SHA1(e8372303724284a750b706dc6bf7641e4c52bb95) )

	ROM_REGION( 0x0800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "060.c1", 0x0000000, 0x200000, CRC(8908fff9) SHA1(f8c16ab0248b60f3a62e0d4d65c456e2f8e4da49) )
	ROM_LOAD16_BYTE( "060.c2", 0x0000001, 0x200000, CRC(c6649492) SHA1(5d39b077387ed6897ac075ede4a2aa94bb64545e) )
	ROM_LOAD16_BYTE( "060.c3", 0x0400000, 0x200000, CRC(0956b437) SHA1(c70be8b5cebf321afe4c3f5e9a12413c3077694a) )
	ROM_LOAD16_BYTE( "060.c4", 0x0400001, 0x200000, CRC(026f3b62) SHA1(d608483b70d60e7aa0e41f25a8b3fed508129eb7) )
ROM_END

/*****************
 Battle Flip Shot
*******************/

ROM_START( flipshothack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "247_hack01.p1", 0x000000, 0x080000, CRC(d2e7a7e3) SHA1(1ff4f070fcc658bbc7dc69e16c87f82d7392d100) )

	NEO_SFIX_128K( "247.s1", CRC(6300185c) SHA1(cb2f1de085fde214f96a962b1c2fa285eb387d44) )

	NEO_BIOS_AUDIO_128K( "247.m1", CRC(a9fe0144) SHA1(4cc076ecce9216a373f3dcd7ba28a03d6050e522) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "247.v1", 0x000000, 0x200000, CRC(42ec743d) SHA1(f45b5167ebcbd59300f4e5b05448cd421654102a) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "247_hack01.c1",  0x000000, 0x200000, CRC(1ccfeff9) SHA1(3575b84acca6ec0f26f4a0ad30a16c140e1c0b2e) )
	ROM_LOAD16_BYTE( "247_hack01.c2",  0x000001, 0x200000, CRC(648cdd5d) SHA1(8a0aaf13e2539e48f9b0b18f8154aa50bf4bed2a) )
ROM_END

/************
 Fight Fever
**************/

ROM_START( galaxyfghack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "078_hack01.p1", 0x100000, 0x100000, CRC(bbfffa22) SHA1(6bde48477f67241dce30107f84c38a8d0c34a81f) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "078.s1", CRC(72f8923e) SHA1(da908bffc2b5d8baa2002dbb5bfb3aa17d2472b7) )

	NEO_BIOS_AUDIO_128K( "078.m1", CRC(8e9e3b10) SHA1(7c44d1dbd4f8d337c99e90361d1dab837df85e31) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "078.v1", 0x000000, 0x200000, CRC(e3b735ac) SHA1(e16dfac09aef8115a20bae0bef8c86d4e7d0dc4a) )
	ROM_LOAD( "078.v2", 0x200000, 0x200000, CRC(6a8e78c2) SHA1(f60b1f8a3a945f279a582745e82f37278ce5d83b) )
	ROM_LOAD( "078.v3", 0x400000, 0x100000, CRC(70bca656) SHA1(218b7079c90898e7faa382b386e77f81f415e7ac) )

	ROM_REGION( 0xe00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "078.c1", 0x000000, 0x200000, CRC(c890c7c0) SHA1(b96c18a41c34070a4f24ca77cb7516fae8b0fd0c) )
	ROM_LOAD16_BYTE( "078.c2", 0x000001, 0x200000, CRC(b6d25419) SHA1(e089df9c9a9645f706e501108d634f4d222622a2) )
	ROM_LOAD16_BYTE( "078.c3", 0x400000, 0x200000, CRC(9d87e761) SHA1(ea1b6d7c9d5ef3a9b48968bde5a52d5699d591cc) )
	ROM_LOAD16_BYTE( "078.c4", 0x400001, 0x200000, CRC(765d7cb8) SHA1(7b9c86714d688602064d928c9d2b49d70bb7541e) )
	ROM_LOAD16_BYTE( "078.c5", 0x800000, 0x200000, CRC(e6b77e6a) SHA1(db3b8fc62a6f21c6653621c0665450d5d9a9913d) )
	ROM_LOAD16_BYTE( "078.c6", 0x800001, 0x200000, CRC(d779a181) SHA1(2761026abd9698a7b56114b76631563abd41fd12) )
	ROM_LOAD16_BYTE( "078.c7", 0xc00000, 0x100000, CRC(4f27d580) SHA1(c0f12496b45b2fe6e94aa8ac52b0157063127e0a) )
	ROM_LOAD16_BYTE( "078.c8", 0xc00001, 0x100000, CRC(0a7cc0d8) SHA1(68aaee6341c87e56ce11acc1c4ec8047839fe70d) )
ROM_END

 /******
 Ganryu
********/

ROM_START( ganryud )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "252.p1", 0x100000, 0x100000, CRC(4b8ac4fb) SHA1(93d90271bff281862b03beba3809cf95a47a1e44) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "252.m1", CRC(30cc4099) SHA1(46453b7aac41855a92724a785372f8daf931d8d4) )

	ROM_REGION( 0x0400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "252.v1", 0x000000, 0x400000, CRC(e5946733) SHA1(d5904a50465af03d6ff33399a98f3259721ca0b2) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "252d.c1", 0x0000000, 0x800000, CRC(6bf7605b) SHA1(6710cbc27d6ec35648f98f0ca541aaaeeffd0980) )
	ROM_LOAD16_BYTE( "252d.c2", 0x0000001, 0x800000, CRC(596792ce) SHA1(ad9c8f8b0e95396fade2b485d11a1b3d4cb65f37) )
ROM_END

 /*****
 Garou
********/

ROM_START( garouhack01 )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "kf.neo-sma", 0x0c0000, 0x040000, CRC(98bc93dc) SHA1(01fe3d18b50f770e131e8d8eeff4c630ba8c9551) )
	ROM_LOAD16_WORD_SWAP( "253.p1", 0x100000, 0x200000, CRC(ea3171a4) SHA1(bbda40f652baa0dc5fc6a006c001a1bdb0df43f6) )
	ROM_LOAD16_WORD_SWAP( "253.p2", 0x300000, 0x200000, CRC(382f704b) SHA1(0ace9c84a8b8a0524fd9a503e7d872de1bf1bd52) )
	ROM_LOAD16_WORD_SWAP( "253.p3", 0x500000, 0x200000, CRC(e395bfdd) SHA1(6b50f5ac15bf66b7e4e9bff57594fd3d7530c831) )
	ROM_LOAD16_WORD_SWAP( "253_hack01.p4", 0x700000, 0x200000, CRC(c1fb6ff4) SHA1(5a907c2c0e8d1fe40f3eecd3b79d97ac8e393940) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "253.m1", CRC(36a806be) SHA1(90fb44dc0c3fb57946a0f35716056abb84a0f191) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "253.v1", 0x000000, 0x400000, CRC(263e388c) SHA1(11f05feee170370c4bfc5053af79246a6e3de5dc) )
	ROM_LOAD( "253.v2", 0x400000, 0x400000, CRC(2c6bc7be) SHA1(c9c61054ce1a47bf1bf77a31117726b499df24a4) )
	ROM_LOAD( "253.v3", 0x800000, 0x400000, CRC(0425b27d) SHA1(986863c98fc3445487242dcf2ea75b075e7f33ee) )
	ROM_LOAD( "253.v4", 0xc00000, 0x400000, CRC(a54be8a9) SHA1(d7123e79b43e8adfaa5ecadbfcbeb6be890ec311) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "253.c1", 0x0000000, 0x800000, CRC(0603e046) SHA1(5ef4557ce90ba65d36129de97be1fdc049c4a3d0) )
	ROM_LOAD16_BYTE( "253.c2", 0x0000001, 0x800000, CRC(0917d2a4) SHA1(d4ed3a13ae22f880fb399671c1752f1a0283f316) )
	ROM_LOAD16_BYTE( "253.c3", 0x1000000, 0x800000, CRC(6737c92d) SHA1(678f0c9cc1267bd131546981b9989bfb7289d8ba) )
	ROM_LOAD16_BYTE( "253.c4", 0x1000001, 0x800000, CRC(5ba92ec6) SHA1(aae36b050a3a0321026a96eba06dd184c0e2acca) )
	ROM_LOAD16_BYTE( "253.c5", 0x2000000, 0x800000, CRC(3eab5557) SHA1(47c433015aa81a0b0a1d3ee51382c4948b80c023) )
	ROM_LOAD16_BYTE( "253.c6", 0x2000001, 0x800000, CRC(308d098b) SHA1(b052f1fa9fbc69606004c250e2505360eaa24949) )
	ROM_LOAD16_BYTE( "253.c7", 0x3000000, 0x800000, CRC(c0e995ae) SHA1(8675ca787d28246174c313167f82557f021366fc) )
	ROM_LOAD16_BYTE( "253.c8", 0x3000001, 0x800000, CRC(21a11303) SHA1(fd61221ad257c185ef5c1f9694bd6b840b591af3) )
ROM_END

ROM_START( garouhack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "253_hack02.p1", 0x000000, 0x100000, CRC(246d6c8c) SHA1(8486445bcef313b10758d965d5f1ffc610204db7) )
	ROM_LOAD16_WORD_SWAP( "253_hack02.p2", 0x100000, 0x400000, CRC(b8884d6a) SHA1(7c33d90521489d68c4afb589d4e4b43cc74366c8) )

	NEO_SFIX_128K( "proto_253.s1", CRC(779989de) SHA1(8bd550857b60f8a907f6d39a4225ceffdd330307) )

	NEO_BIOS_AUDIO_256K( "proto_253.m1", CRC(bbe464f7) SHA1(f5f8f3e48f5d453f45107085d6f4023bcd24c053) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "proto_253.v1", 0x000000, 0x400000, CRC(274f3923) SHA1(4c7a8ad1cd0e3afc1f78de3c2929120ed434f104) )
	ROM_LOAD( "proto_253.v2", 0x400000, 0x400000, CRC(8f86dabe) SHA1(b3d2d9f5c1d97a6e7aee2c674fb6627f41bbb240) )
	ROM_LOAD( "proto_253.v3", 0x800000, 0x400000, CRC(05fd06cd) SHA1(6cd699719614bb87547632ea3d61d92d81fdf563) )
	ROM_LOAD( "proto_253.v4", 0xc00000, 0x400000, CRC(14984063) SHA1(170d5638327ec0eb3590b80dc11590897367250c) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "proto_253.c1", 0x0000000, 0x800000, CRC(5bb5d137) SHA1(d648febd8e6a0bdd9bdbb6ce1f1f8b08567ec05a) )
	ROM_LOAD16_BYTE( "proto_253.c2", 0x0000001, 0x800000, CRC(5c8d2960) SHA1(f7503502be0332adf408ee0ea5ee5161c8939fd8) )
	ROM_LOAD16_BYTE( "proto_253.c3", 0x1000000, 0x800000, CRC(234d16fc) SHA1(7b9221f7ecc438150c8a10be72390329854ed21b) )
	ROM_LOAD16_BYTE( "proto_253.c4", 0x1000001, 0x800000, CRC(b9b5b993) SHA1(6059793eaf6e58c172235fe64aa9d25a40c38ed6) )
	ROM_LOAD16_BYTE( "proto_253.c5", 0x2000000, 0x800000, CRC(722615d2) SHA1(798832c535869f0e247c3db0d8253779b103e213) )
	ROM_LOAD16_BYTE( "proto_253.c6", 0x2000001, 0x800000, CRC(0a6fab38) SHA1(eaee6f2f18af91f7959d84d4b991b3fc182d07c4) )
	ROM_LOAD16_BYTE( "proto_253.c7", 0x3000000, 0x800000, CRC(d68e806f) SHA1(92bfd9839115bd590972ae4ecc45ad35dce22387) )
	ROM_LOAD16_BYTE( "proto_253.c8", 0x3000001, 0x800000, CRC(f778fe99) SHA1(c963f6ba90a36d02991728b44ffcf174ca18268a) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( garoud )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "kf.neo-sma", 0x0c0000, 0x040000, CRC(98bc93dc) SHA1(01fe3d18b50f770e131e8d8eeff4c630ba8c9551) )
	ROM_LOAD16_WORD_SWAP( "253.p1", 0x100000, 0x200000, CRC(ea3171a4) SHA1(bbda40f652baa0dc5fc6a006c001a1bdb0df43f6) )
	ROM_LOAD16_WORD_SWAP( "253.p2", 0x300000, 0x200000, CRC(382f704b) SHA1(0ace9c84a8b8a0524fd9a503e7d872de1bf1bd52) )
	ROM_LOAD16_WORD_SWAP( "253.p3", 0x500000, 0x200000, CRC(e395bfdd) SHA1(6b50f5ac15bf66b7e4e9bff57594fd3d7530c831) )
	ROM_LOAD16_WORD_SWAP( "253.p4", 0x700000, 0x200000, CRC(da92c08e) SHA1(5556f983ebcebc33160e90a6a6cf589d54c8cedc) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "253.m1", CRC(36a806be) SHA1(90fb44dc0c3fb57946a0f35716056abb84a0f191) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "253.v1", 0x000000, 0x400000, CRC(263e388c) SHA1(11f05feee170370c4bfc5053af79246a6e3de5dc) )
	ROM_LOAD( "253.v2", 0x400000, 0x400000, CRC(2c6bc7be) SHA1(c9c61054ce1a47bf1bf77a31117726b499df24a4) )
	ROM_LOAD( "253.v3", 0x800000, 0x400000, CRC(0425b27d) SHA1(986863c98fc3445487242dcf2ea75b075e7f33ee) )
	ROM_LOAD( "253.v4", 0xc00000, 0x400000, CRC(a54be8a9) SHA1(d7123e79b43e8adfaa5ecadbfcbeb6be890ec311) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "253d.c1", 0x0000000, 0x800000, CRC(497be3f2) SHA1(3b4e524ff185db326cdefe115851ce282272fb58) )
	ROM_LOAD16_BYTE( "253d.c2", 0x0000001, 0x800000, CRC(6a9e95ca) SHA1(d9730d2db58edfa660edab5bf0f7fc3ed058bccf) )
	ROM_LOAD16_BYTE( "253d.c3", 0x1000000, 0x800000, CRC(39373d2f) SHA1(8c0307150bc31001605efbe0e38ae74e1e466d5d) )
	ROM_LOAD16_BYTE( "253d.c4", 0x1000001, 0x800000, CRC(4de23f6c) SHA1(b40bce471fb1815037e4c3705f8238e659a13a82) )
	ROM_LOAD16_BYTE( "253d.c5", 0x2000000, 0x800000, CRC(16634ba5) SHA1(e51649efd039270139aa7292a28925e547b2be59) )
	ROM_LOAD16_BYTE( "253d.c6", 0x2000001, 0x800000, CRC(95671ffd) SHA1(803ebeb74fefe88b9aba53cbca103c0e9145abcb) )
	ROM_LOAD16_BYTE( "253d.c7", 0x3000000, 0x800000, CRC(e36ce77f) SHA1(71931559293182749ded13c52d8bac988a3a062f) )
	ROM_LOAD16_BYTE( "253d.c8", 0x3000001, 0x800000, CRC(ddbd1096) SHA1(634cfb61398b3f1b983f7f3966b53f4cc3a78671) )
ROM_END

 /***************
 Voltage Fighter
******************/

ROM_START( gowcaizrhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "094_hack01.p1", 0x100000, 0x100000, CRC(d4f37cc4) SHA1(deec8a0516ae2057551713098e81be53c751eff7) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "094.s1", CRC(2f8748a2) SHA1(5cc723c4284120473d63d8b0c1a3b3be74bdc324) )

	NEO_BIOS_AUDIO_128K( "094.m1", CRC(78c851cb) SHA1(a9923c002e4e2171a564af45cff0958c5d57b275) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "094.v1", 0x000000, 0x200000, CRC(6c31223c) SHA1(ede3a2806d7d872a0f737626a23ecce200b534e6) )
	ROM_LOAD( "094.v2", 0x200000, 0x200000, CRC(8edb776c) SHA1(a9eac5e24f83ccdcf303d63261747b1bad876a24) )
	ROM_LOAD( "094.v3", 0x400000, 0x100000, CRC(c63b9285) SHA1(6bbbacfe899e204e74657d6c3f3d05ce75e432f1) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "094.c1", 0x000000, 0x200000, CRC(042f6af5) SHA1(1c50df6a1a53ffb3079ea0a19c746f5c9536a3ed) )
	ROM_LOAD16_BYTE( "094.c2", 0x000001, 0x200000, CRC(0fbcd046) SHA1(9a6dc920a877f27424477c3478907b23afbaa5ea) )
	ROM_LOAD16_BYTE( "094.c3", 0x400000, 0x200000, CRC(58bfbaa1) SHA1(4c6f9cf138c5e6dfe89a45e2a690a986c75f5bfc) )
	ROM_LOAD16_BYTE( "094.c4", 0x400001, 0x200000, CRC(9451ee73) SHA1(7befee4a886b1d7493c06cefb7abf4ec01c14a8b) )
	ROM_LOAD16_BYTE( "094.c5", 0x800000, 0x200000, CRC(ff9cf48c) SHA1(5f46fb5d0812275b0006919d8540f22be7c16492) )
	ROM_LOAD16_BYTE( "094.c6", 0x800001, 0x200000, CRC(31bbd918) SHA1(7ff8c5e3f17d40e7a8a189ad8f8026de55368810) )
	ROM_LOAD16_BYTE( "094.c7", 0xc00000, 0x200000, CRC(2091ec04) SHA1(a81d4bdbef1ac6ea49845dc30e31bf9745694100) )
	ROM_LOAD16_BYTE( "094.c8", 0xc00001, 0x200000, CRC(d80dd241) SHA1(1356a64e4d4e271f62cd0d83f79ee9c906440810) )
ROM_END

ROM_START( gowcaizrhack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "094_hack02.p1", 0x100000, 0x100000, CRC(c8f1f4ae) SHA1(95f9fec0e90380a93423f5291eafb2b5b837e397) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "094.s1", CRC(2f8748a2) SHA1(5cc723c4284120473d63d8b0c1a3b3be74bdc324) )

	NEO_BIOS_AUDIO_128K( "094.m1", CRC(78c851cb) SHA1(a9923c002e4e2171a564af45cff0958c5d57b275) )

	ROM_REGION( 0x500000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "094.v1", 0x000000, 0x200000, CRC(6c31223c) SHA1(ede3a2806d7d872a0f737626a23ecce200b534e6) )
	ROM_LOAD( "094.v2", 0x200000, 0x200000, CRC(8edb776c) SHA1(a9eac5e24f83ccdcf303d63261747b1bad876a24) )
	ROM_LOAD( "094.v3", 0x400000, 0x100000, CRC(c63b9285) SHA1(6bbbacfe899e204e74657d6c3f3d05ce75e432f1) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "094.c1", 0x000000, 0x200000, CRC(042f6af5) SHA1(1c50df6a1a53ffb3079ea0a19c746f5c9536a3ed) )
	ROM_LOAD16_BYTE( "094.c2", 0x000001, 0x200000, CRC(0fbcd046) SHA1(9a6dc920a877f27424477c3478907b23afbaa5ea) )
	ROM_LOAD16_BYTE( "094.c3", 0x400000, 0x200000, CRC(58bfbaa1) SHA1(4c6f9cf138c5e6dfe89a45e2a690a986c75f5bfc) )
	ROM_LOAD16_BYTE( "094.c4", 0x400001, 0x200000, CRC(9451ee73) SHA1(7befee4a886b1d7493c06cefb7abf4ec01c14a8b) )
	ROM_LOAD16_BYTE( "094.c5", 0x800000, 0x200000, CRC(ff9cf48c) SHA1(5f46fb5d0812275b0006919d8540f22be7c16492) )
	ROM_LOAD16_BYTE( "094.c6", 0x800001, 0x200000, CRC(31bbd918) SHA1(7ff8c5e3f17d40e7a8a189ad8f8026de55368810) )
	ROM_LOAD16_BYTE( "094.c7", 0xc00000, 0x200000, CRC(2091ec04) SHA1(a81d4bdbef1ac6ea49845dc30e31bf9745694100) )
	ROM_LOAD16_BYTE( "094y.c8", 0xc00001, 0x200000, CRC(0d31dee6) SHA1(4979aa3daa7e490fbe39e7b3c70cbb2ef7551c5f) )
ROM_END

 /***********
 Ghost Pilots
**************/

ROM_START( gpilotshack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "020_hack01.p1", 0x000000, 0x080000, CRC(80d6950e) SHA1(c83e119c316b9c5d86e06fff598b1be7e2cc53a5) )
	ROM_LOAD16_WORD_SWAP( "020.p2", 0x080000, 0x020000, CRC(edcb22ac) SHA1(505d2db38ae999b7d436e8f2ff56b81796d62b54) )

	NEO_SFIX_128K( "020.s1", CRC(a6d83d53) SHA1(9a8c092f89521cc0b27a385aa72e29cbaca926c5) )

	NEO_BIOS_AUDIO_128K( "020.m1", CRC(48409377) SHA1(0e212d2c76856a90b2c2fdff675239525972ac43) )

	ROM_REGION( 0x180000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "020.v11", 0x000000, 0x100000, CRC(1b526c8b) SHA1(2801868d2badcf8aaf5d490e010e4049d81d7bc1) )
	ROM_LOAD( "020.v12", 0x100000, 0x080000, CRC(4a9e6f03) SHA1(d3ac11f333b03d8a318921bdaefb14598e289a14) )

	ROM_REGION( 0x080000, "ym.deltat", 0 )
	ROM_LOAD( "020.v21", 0x000000, 0x080000, CRC(7abf113d) SHA1(5b2a0e70f2eaf4638b44702dacd4cb17838fb1d5) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "020.c1", 0x000000, 0x100000, CRC(bd6fe78e) SHA1(50b704862cd79d64fa488e621b079f6e413c33bc) )
	ROM_LOAD16_BYTE( "020.c2", 0x000001, 0x100000, CRC(5f4a925c) SHA1(71c5ef8141234daaa7025427a6c65e79766973a5) )
	ROM_LOAD16_BYTE( "020.c3", 0x200000, 0x100000, CRC(d1e42fd0) SHA1(f0d476aebbdc2ce008f5f0783be86d295b24aa44) )
	ROM_LOAD16_BYTE( "020.c4", 0x200001, 0x100000, CRC(edde439b) SHA1(79be7b10ecdab54c2f77062b8f5fda0e299fa982) )
ROM_END

 /********
 Ironclad
***********/

ROM_START( ironcladhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "220_hack01.p1", 0x000000, 0x200000, CRC(65849961) SHA1(2846081bb1451a209412159991bfac95d394fe3a) )

	NEO_SFIX_128K( "proto_220.s1", CRC(372fe217) SHA1(493433e682f519bf647e1481c8bdd3a980830ffb) )

	NEO_BIOS_AUDIO_128K( "proto_220.m1", CRC(3a08bb63) SHA1(d8fbbf42a006ccafc3cd99808d28c82dbaac4590) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "proto_220.v1", 0x000000, 0x400000, CRC(8f30a215) SHA1(0ee866a468c4c3608d55df2b5cb9243c8016d77c) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "220_hack01.c1", 0x000000, 0x800000, CRC(621f4a09) SHA1(117a8e8bea5f880d8315b48bc5884eb805c5e678) )
	ROM_LOAD16_BYTE( "220_hack01.c2", 0x000001, 0x800000, CRC(7fec83e8) SHA1(769c2cb4ed4a9d7c29e3aad3f4601f1d24c96640) )
ROM_END

 /****************
 Jockey Grand Prix
*******************/

ROM_START( jockeygpd )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "008.p1", 0x000000, 0x100000, CRC(2fb7f388) SHA1(e3c9b03944b4c10cf5081caaf9c8be1f08c06493) )
	ROM_FILL( 0x100000, 0x100000, 0xff )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "008.m1", CRC(d163c690) SHA1(1dfd04d20c5985037f07cd01000d0b04f3a8f4f4) )

	ROM_REGION( 0x0200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "008.v1", 0x000000, 0x200000, CRC(443eadba) SHA1(3def3c22f0e276bc4c2fc7ff70ce473c08b0d2df) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "008d.c1", 0x0000000, 0x800000, CRC(ea259019) SHA1(c14dc59a43e255ef12d8d832b2860f1a84c48170) )
	ROM_LOAD16_BYTE( "008d.c2", 0x0000001, 0x800000, CRC(97f270be) SHA1(06e03b26bacb8568c69ccf22e58175426a69e01b) )
ROM_END

 /****************
 Far East of Eden
*******************/

ROM_START( kabukiklhack01 ) 
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "092_hack01.p1", 0x100000, 0x100000, CRC(bcf0f989) SHA1(91D493486A2BE79817D31E75A192079497A45360) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "092.s1", CRC(a3d68ee2) SHA1(386f6110a16967a72fbf788f9d968fddcdcd2889) )

	NEO_BIOS_AUDIO_128K( "092.m1", CRC(91957ef6) SHA1(7b6907532a0e02ceb643623cbd689cf228776ed1) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "092.v1", 0x000000, 0x200000, CRC(69e90596) SHA1(1a2007d7784b3ce90d115980c3353862f1664d45) )
	ROM_LOAD( "092.v2", 0x200000, 0x200000, CRC(7abdb75d) SHA1(0bff764889fe02f37877514c7fc450250839f632) )
	ROM_LOAD( "092.v3", 0x400000, 0x200000, CRC(eccc98d3) SHA1(b0dfbdb1ea045cb961323ac6906ab342256c3dc7) )
	ROM_LOAD( "092.v4", 0x600000, 0x100000, CRC(a7c9c949) SHA1(574bc55b45e81ce357b14f5992426115de25cd35) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "092.c1", 0x000000, 0x400000, CRC(2a9fab01) SHA1(aa9f037df33ae0575b328734c76c0918ae1917e9) )
	ROM_LOAD16_BYTE( "092.c2", 0x000001, 0x400000, CRC(6d2bac02) SHA1(dfe96b62883333872be432e8af1ae617c9e62698) )
	ROM_LOAD16_BYTE( "092.c3", 0x800000, 0x400000, CRC(5da735d6) SHA1(f1c05a73794ece15576a0a30c81f4a44faac475a) )
	ROM_LOAD16_BYTE( "092.c4", 0x800001, 0x400000, CRC(de07f997) SHA1(c27a4d4bef868eed38dc152ff37d4135b16cc991) )
ROM_END

/*****************
 Karnov's Revenge
*******************/

ROM_START( karnovrhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "066_hack01.p1", 0x000000, 0x100000, CRC(7b776bb5) SHA1(a5457239ad2af9b341878bfd94baa818683a21d1) )

	NEO_SFIX_128K( "066.s1", CRC(bae5d5e5) SHA1(aa69d9b235b781ec51f72a528fada9cb12e72cbc) )

	NEO_BIOS_AUDIO_128K( "066.m1", CRC(030beae4) SHA1(ceb6ee6c09514504efacdbca7b280901e4c97084) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "066.v1", 0x000000, 0x200000, CRC(0b7ea37a) SHA1(34e7d4f6db053674a7e8c8b2e3e398777d5b02e6) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "066.c1", 0x000000, 0x200000, CRC(09dfe061) SHA1(ca4c0f0ce80967b4be2f18b72435c468bbfbac4c) )
	ROM_LOAD16_BYTE( "066.c2", 0x000001, 0x200000, CRC(e0f6682a) SHA1(addb4fbc30da2b8ffc86819d92a874eb232f67dd) )
	ROM_LOAD16_BYTE( "066.c3", 0x400000, 0x200000, CRC(a673b4f7) SHA1(d138f5b38fd65c61549ce36f5c4983f7c8a3e7f6) )
	ROM_LOAD16_BYTE( "066.c4", 0x400001, 0x200000, CRC(cb3dc5f4) SHA1(865d9ccfc3df517c341d6aac16120f6b6aa759fe) )
	ROM_LOAD16_BYTE( "066.c5", 0x800000, 0x200000, CRC(9a28785d) SHA1(19723e1f7ff429e8a038d89488b279f830dfaf6e) )
	ROM_LOAD16_BYTE( "066.c6", 0x800001, 0x200000, CRC(c15c01ed) SHA1(7cf5583e6610bcdc3b332896cefc71df84fb3f19) )
ROM_END

/*****************
 Kizuna Encounter
*******************/

ROM_START( kizunahack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "216_hack01.p1", 0x100000, 0x100000, CRC(a4ccd402) SHA1(038539BF889AF6186BB81DB330E6DBDF06F64FA8) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "216.s1", CRC(efdc72d7) SHA1(be37cbf1852e2e4c907cc799b754b538544b6703) )

	NEO_BIOS_AUDIO_128K( "216.m1", CRC(1b096820) SHA1(72852e78c620038f8dafde5e54e02e418c31be9c) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "059.v1", 0x000000, 0x200000, CRC(530c50fd) SHA1(29401cee7f7d2c199c7cb58092e86b28205e81ad) )
	ROM_LOAD( "216.v2", 0x200000, 0x200000, CRC(03667a8d) SHA1(3b0475e553a49f8788f32b0c84f82645cc6b4273) )
	ROM_LOAD( "059.v3", 0x400000, 0x200000, CRC(7038c2f9) SHA1(c1d6f86b24feba03fe009b58199d2eeabe572f4e) )
	ROM_LOAD( "216.v4", 0x600000, 0x200000, CRC(31b99bd6) SHA1(5871751f8e9e6b98337472c22b5e1c7ede0a9311) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "059.c1", 0x0000000, 0x200000, CRC(763ba611) SHA1(d3262e0332c894ee149c5963f882cc5e5562ee57) )
	ROM_LOAD16_BYTE( "059.c2", 0x0000001, 0x200000, CRC(e05e8ca6) SHA1(986a9b16ff92bc101ab567d2d01348e093abea9a) )
	ROM_LOAD16_BYTE( "216.c3", 0x0800000, 0x400000, CRC(665c9f16) SHA1(7ec781a49a462f395b450460b29493f55134eac2) )
	ROM_LOAD16_BYTE( "216.c4", 0x0800001, 0x400000, CRC(7f5d03db) SHA1(365ed266c121f4df0bb76898955a8ae0e668a216) )
	ROM_LOAD16_BYTE( "059.c5", 0x1000000, 0x200000, CRC(59013f9e) SHA1(5bf48fcc450da72a8c4685f6e3887e67eae49988) )
	ROM_LOAD16_BYTE( "059.c6", 0x1000001, 0x200000, CRC(1c8d5def) SHA1(475d89a5c4922a9f6bd756d23c2624d57b6e9d62) )
	ROM_LOAD16_BYTE( "059.c7", 0x1800000, 0x200000, CRC(c88f7035) SHA1(c29a428b741f4fe7b71a3bc23c87925b6bc1ca8f) )
	ROM_LOAD16_BYTE( "059.c8", 0x1800001, 0x200000, CRC(484ce3ba) SHA1(4f21ed20ce6e2b67e2b079404599310c94f591ff) )
ROM_END

ROM_START( kizunahack02 ) //kizunal
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "216_hack02.p1", 0x100000, 0x100000, CRC(d2ca0cbe) SHA1(84dc3678a1be2357b1b02eedb1dd0088a94e0ca2) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "216.s1", CRC(efdc72d7) SHA1(be37cbf1852e2e4c907cc799b754b538544b6703) )

	NEO_BIOS_AUDIO_128K( "216.m1", CRC(1b096820) SHA1(72852e78c620038f8dafde5e54e02e418c31be9c) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "059.v1", 0x000000, 0x200000, CRC(530c50fd) SHA1(29401cee7f7d2c199c7cb58092e86b28205e81ad) )
	ROM_LOAD( "216.v2", 0x200000, 0x200000, CRC(03667a8d) SHA1(3b0475e553a49f8788f32b0c84f82645cc6b4273) )
	ROM_LOAD( "059.v3", 0x400000, 0x200000, CRC(7038c2f9) SHA1(c1d6f86b24feba03fe009b58199d2eeabe572f4e) )
	ROM_LOAD( "216.v4", 0x600000, 0x200000, CRC(31b99bd6) SHA1(5871751f8e9e6b98337472c22b5e1c7ede0a9311) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "059.c1", 0x0000000, 0x200000, CRC(763ba611) SHA1(d3262e0332c894ee149c5963f882cc5e5562ee57) )
	ROM_LOAD16_BYTE( "059.c2", 0x0000001, 0x200000, CRC(e05e8ca6) SHA1(986a9b16ff92bc101ab567d2d01348e093abea9a) )
	ROM_LOAD16_BYTE( "216.c3", 0x0800000, 0x400000, CRC(665c9f16) SHA1(7ec781a49a462f395b450460b29493f55134eac2) )
	ROM_LOAD16_BYTE( "216.c4", 0x0800001, 0x400000, CRC(7f5d03db) SHA1(365ed266c121f4df0bb76898955a8ae0e668a216) )
	ROM_LOAD16_BYTE( "059.c5", 0x1000000, 0x200000, CRC(59013f9e) SHA1(5bf48fcc450da72a8c4685f6e3887e67eae49988) )
	ROM_LOAD16_BYTE( "059.c6", 0x1000001, 0x200000, CRC(1c8d5def) SHA1(475d89a5c4922a9f6bd756d23c2624d57b6e9d62) )
	ROM_LOAD16_BYTE( "059.c7", 0x1800000, 0x200000, CRC(c88f7035) SHA1(c29a428b741f4fe7b71a3bc23c87925b6bc1ca8f) )
	ROM_LOAD16_BYTE( "059.c8", 0x1800001, 0x200000, CRC(484ce3ba) SHA1(4f21ed20ce6e2b67e2b079404599310c94f591ff) )
ROM_END

 /***************************
   The King of Fighters '94
*****************************/

ROM_START( kof94hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "055_hack01.p1", 0x100000, 0x100000, CRC(1b4528e8) SHA1(241248df8cfbb2979ec55849018c509bf3355628) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "055.s1", CRC(825976c1) SHA1(cb6a70bdd95d449d25196ca269b621c362db6743) )

	NEO_BIOS_AUDIO_128K( "055.m1", CRC(f6e77cf5) SHA1(292a3e3a4918ffe72bd1c41acb927b91844e035e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "055.v1", 0x000000, 0x200000, CRC(8889596d) SHA1(c9ce713b720511438dbd3fe3bcc7c246f475c6a2) )
	ROM_LOAD( "055.v2", 0x200000, 0x200000, CRC(25022b27) SHA1(2b040a831c3c92ac6e4719de38009a0d55b64f6b) )
	ROM_LOAD( "055.v3", 0x400000, 0x200000, CRC(83cf32c0) SHA1(34a31a37eb10945b5169e96321bcea06eec33a00) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "055.c1", 0x000000, 0x200000, CRC(b96ef460) SHA1(e52f5303c17b50ce165c008be2837336369c110b) )
	ROM_LOAD16_BYTE( "055.c2", 0x000001, 0x200000, CRC(15e096a7) SHA1(237c2a3d059de00bfca66e0016ed325d7a32bfec) )
	ROM_LOAD16_BYTE( "055.c3", 0x400000, 0x200000, CRC(54f66254) SHA1(c594384bcd8b03beb8c595591505fecc44b185ac) )
	ROM_LOAD16_BYTE( "055.c4", 0x400001, 0x200000, CRC(0b01765f) SHA1(ec1fdcc944611408367bf5023d4ebe7edd9dfa88) )
	ROM_LOAD16_BYTE( "055.c5", 0x800000, 0x200000, CRC(ee759363) SHA1(8a5621c1b1f8267b9b9b6a14ab4944de542e1945) )
	ROM_LOAD16_BYTE( "055.c6", 0x800001, 0x200000, CRC(498da52c) SHA1(1e6e6202ee053a5261db889177ce3a087e078bda) )
	ROM_LOAD16_BYTE( "055.c7", 0xc00000, 0x200000, CRC(62f66888) SHA1(ac91a0eab0753bee175ad40213a4ae5d38ed5b87) )
	ROM_LOAD16_BYTE( "055.c8", 0xc00001, 0x200000, CRC(fe0a235d) SHA1(a45c66836e4e3c77dfef9d4c6cc422cb59169149) )
ROM_END

ROM_START( kof94hack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "055_hack02.p1", 0x100000, 0x100000, CRC(0138a992) SHA1(48c857ed371de236861d172552e3b5bf38def776) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "055.s1", CRC(825976c1) SHA1(cb6a70bdd95d449d25196ca269b621c362db6743) )

	NEO_BIOS_AUDIO_128K( "055.m1", CRC(f6e77cf5) SHA1(292a3e3a4918ffe72bd1c41acb927b91844e035e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "055.v1", 0x000000, 0x200000, CRC(8889596d) SHA1(c9ce713b720511438dbd3fe3bcc7c246f475c6a2) )
	ROM_LOAD( "055.v2", 0x200000, 0x200000, CRC(25022b27) SHA1(2b040a831c3c92ac6e4719de38009a0d55b64f6b) )
	ROM_LOAD( "055.v3", 0x400000, 0x200000, CRC(83cf32c0) SHA1(34a31a37eb10945b5169e96321bcea06eec33a00) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "055_hack02.c1",  0x000000, 0x200000, CRC(6fb14154) SHA1(bdc0e1933b9e903a69a2303c2e0e4243d3e1c95e) )
	ROM_LOAD16_BYTE( "055.c2", 0x000001, 0x200000, CRC(15e096a7) SHA1(237c2a3d059de00bfca66e0016ed325d7a32bfec) )
	ROM_LOAD16_BYTE( "055.c3", 0x400000, 0x200000, CRC(54f66254) SHA1(c594384bcd8b03beb8c595591505fecc44b185ac) )
	ROM_LOAD16_BYTE( "055.c4", 0x400001, 0x200000, CRC(0b01765f) SHA1(ec1fdcc944611408367bf5023d4ebe7edd9dfa88) )
	ROM_LOAD16_BYTE( "055.c5", 0x800000, 0x200000, CRC(ee759363) SHA1(8a5621c1b1f8267b9b9b6a14ab4944de542e1945) )
	ROM_LOAD16_BYTE( "055.c6", 0x800001, 0x200000, CRC(498da52c) SHA1(1e6e6202ee053a5261db889177ce3a087e078bda) )
	ROM_LOAD16_BYTE( "055.c7", 0xc00000, 0x200000, CRC(62f66888) SHA1(ac91a0eab0753bee175ad40213a4ae5d38ed5b87) )
	ROM_LOAD16_BYTE( "055.c8", 0xc00001, 0x200000, CRC(fe0a235d) SHA1(a45c66836e4e3c77dfef9d4c6cc422cb59169149) )
ROM_END

ROM_START( kof94hack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "055.p1", 0x100000, 0x100000, CRC(f10a2042) SHA1(d08a3f3c28be4b1793de7d362456281329fe1828) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "055_hack03.s1", CRC(80eeeaf6) SHA1(cfeb8cce91798f80f022c490cc983b67310da454) )

	NEO_BIOS_AUDIO_128K( "055.m1", CRC(f6e77cf5) SHA1(292a3e3a4918ffe72bd1c41acb927b91844e035e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "055.v1", 0x000000, 0x200000, CRC(8889596d) SHA1(c9ce713b720511438dbd3fe3bcc7c246f475c6a2) )
	ROM_LOAD( "055.v2", 0x200000, 0x200000, CRC(25022b27) SHA1(2b040a831c3c92ac6e4719de38009a0d55b64f6b) )
	ROM_LOAD( "055.v3", 0x400000, 0x200000, CRC(83cf32c0) SHA1(34a31a37eb10945b5169e96321bcea06eec33a00) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "055.c1", 0x000000, 0x200000, CRC(b96ef460) SHA1(e52f5303c17b50ce165c008be2837336369c110b) )
	ROM_LOAD16_BYTE( "055.c2", 0x000001, 0x200000, CRC(15e096a7) SHA1(237c2a3d059de00bfca66e0016ed325d7a32bfec) )
	ROM_LOAD16_BYTE( "055.c3", 0x400000, 0x200000, CRC(54f66254) SHA1(c594384bcd8b03beb8c595591505fecc44b185ac) )
	ROM_LOAD16_BYTE( "055.c4", 0x400001, 0x200000, CRC(0b01765f) SHA1(ec1fdcc944611408367bf5023d4ebe7edd9dfa88) )
	ROM_LOAD16_BYTE( "055.c5", 0x800000, 0x200000, CRC(ee759363) SHA1(8a5621c1b1f8267b9b9b6a14ab4944de542e1945) )
	ROM_LOAD16_BYTE( "055.c6", 0x800001, 0x200000, CRC(498da52c) SHA1(1e6e6202ee053a5261db889177ce3a087e078bda) )
	ROM_LOAD16_BYTE( "055.c7", 0xc00000, 0x200000, CRC(62f66888) SHA1(ac91a0eab0753bee175ad40213a4ae5d38ed5b87) )
	ROM_LOAD16_BYTE( "055.c8", 0xc00001, 0x200000, CRC(fe0a235d) SHA1(a45c66836e4e3c77dfef9d4c6cc422cb59169149) )
ROM_END

ROM_START( kof94hack04 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "055_hack04.p1", 0x100000, 0x100000, CRC(38410fdf) SHA1(e4053b41afd8c8934b7b2977df6d64d8b6338370) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "055_hack04.s1", CRC(286ab67d) SHA1(c414c1543d13097c49090e46c474c5dd3c524c9c) )

	NEO_BIOS_AUDIO_128K( "055.m1", CRC(f6e77cf5) SHA1(292a3e3a4918ffe72bd1c41acb927b91844e035e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "055.v1", 0x000000, 0x200000, CRC(8889596d) SHA1(c9ce713b720511438dbd3fe3bcc7c246f475c6a2) )
	ROM_LOAD( "055.v2", 0x200000, 0x200000, CRC(25022b27) SHA1(2b040a831c3c92ac6e4719de38009a0d55b64f6b) )
	ROM_LOAD( "055.v3", 0x400000, 0x200000, CRC(83cf32c0) SHA1(34a31a37eb10945b5169e96321bcea06eec33a00) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "055.c1", 0x000000, 0x200000, CRC(b96ef460) SHA1(e52f5303c17b50ce165c008be2837336369c110b) )
	ROM_LOAD16_BYTE( "055.c2", 0x000001, 0x200000, CRC(15e096a7) SHA1(237c2a3d059de00bfca66e0016ed325d7a32bfec) )
	ROM_LOAD16_BYTE( "055.c3", 0x400000, 0x200000, CRC(54f66254) SHA1(c594384bcd8b03beb8c595591505fecc44b185ac) )
	ROM_LOAD16_BYTE( "055.c4", 0x400001, 0x200000, CRC(0b01765f) SHA1(ec1fdcc944611408367bf5023d4ebe7edd9dfa88) )
	ROM_LOAD16_BYTE( "055.c5", 0x800000, 0x200000, CRC(ee759363) SHA1(8a5621c1b1f8267b9b9b6a14ab4944de542e1945) )
	ROM_LOAD16_BYTE( "055.c6", 0x800001, 0x200000, CRC(498da52c) SHA1(1e6e6202ee053a5261db889177ce3a087e078bda) )
	ROM_LOAD16_BYTE( "055.c7", 0xc00000, 0x200000, CRC(62f66888) SHA1(ac91a0eab0753bee175ad40213a4ae5d38ed5b87) )
	ROM_LOAD16_BYTE( "055.c8", 0xc00001, 0x200000, CRC(fe0a235d) SHA1(a45c66836e4e3c77dfef9d4c6cc422cb59169149) )
ROM_END

 /************************
 The King of Fighters '95
****************************/

ROM_START( kof95hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "084_hack01.p1",0x100000, 0x100000, CRC(b6ad793e) SHA1(9e4d99311df208bec1cabc8e3d2337fbf33f669a) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "084.s1", CRC(de716f8a) SHA1(f7386454a943ed5caf625f67ee1d0197b1c6fa13) )

	NEO_BIOS_AUDIO_128K( "084.m1", CRC(6f2d7429) SHA1(6f8462e4f07af82a5ca3197895d5dcbb67bdaa61) )

	ROM_REGION( 0x900000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "084.v1", 0x000000, 0x400000, CRC(84861b56) SHA1(1b6c91ddaed01f45eb9b7e49d9c2b9b479d50da6) )
	ROM_LOAD( "084.v2", 0x400000, 0x200000, CRC(b38a2803) SHA1(dbc2c8606ca09ed7ff20906b022da3cf053b2f09) )
	ROM_LOAD( "084.v3", 0x800000, 0x100000, CRC(d683a338) SHA1(eb9866b4b286edc09963cb96c43ce0a8fb09adbb) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "084.c1", 0x0000000, 0x400000, CRC(fe087e32) SHA1(e8e89faa616027e4fb9b8a865c1a67f409c93bdf) )
	ROM_LOAD16_BYTE( "084.c2", 0x0000001, 0x400000, CRC(07864e09) SHA1(0817fcfd75d0735fd8ff27561eaec371e4ff5829) )
	ROM_LOAD16_BYTE( "084.c3", 0x0800000, 0x400000, CRC(a4e65d1b) SHA1(740a405b40b3a4b324697d2652cae29ffe0ac0bd) )
	ROM_LOAD16_BYTE( "084.c4", 0x0800001, 0x400000, CRC(c1ace468) SHA1(74ea2a3cfd7b744f0988a05baaff10016ca8f625) )
	ROM_LOAD16_BYTE( "084.c5", 0x1000000, 0x200000, CRC(8a2c1edc) SHA1(67866651bc0ce27122285a66b0aab108acf3d065) )
	ROM_LOAD16_BYTE( "084.c6", 0x1000001, 0x200000, CRC(f593ac35) SHA1(302c92c63f092a8d49429c3331e5e5678f0ea48d) )
	ROM_LOAD16_BYTE( "084.c7", 0x1800000, 0x100000, CRC(9904025f) SHA1(eec770746a0ad073f7d353ab16a2cc3a5278d307) )
	ROM_LOAD16_BYTE( "084.c8", 0x1800001, 0x100000, CRC(78eb0f9b) SHA1(2925ea21ed2ce167f08a25589e94f28643379034) )
ROM_END

ROM_START( kof95hack02 ) //kof95sp
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "084_hack02.p1",0x000000, 0x100000, CRC(b3c26333) SHA1(e81a4d7a7da9295154375fbb6f894e7b19e45331) )
	ROM_LOAD16_WORD_SWAP( "084_hack02.p2", 0x100000, 0x100000, CRC(5cb1af9e) SHA1(e4bea729569e7c1897182787d835188fdc6c2ef0) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "084_hack02.p3",0x000000, 0x020000, CRC(7eddc5d8) SHA1(47ad5a72fef93bb4097f1febc35a5782be07fe1a) )

	NEO_SFIX_128K( "084_hack02.s1", CRC(83cbae60) SHA1(9e529c8811fa69a201e103c62e723427d578098b) )

	NEO_BIOS_AUDIO_128K( "084.m1", CRC(6f2d7429) SHA1(6f8462e4f07af82a5ca3197895d5dcbb67bdaa61) )

	ROM_REGION( 0x900000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "084.v1", 0x000000, 0x400000, CRC(84861b56) SHA1(1b6c91ddaed01f45eb9b7e49d9c2b9b479d50da6) )
	ROM_LOAD( "084.v2", 0x400000, 0x200000, CRC(b38a2803) SHA1(dbc2c8606ca09ed7ff20906b022da3cf053b2f09) )
	ROM_LOAD( "084.v3", 0x800000, 0x100000, CRC(d683a338) SHA1(eb9866b4b286edc09963cb96c43ce0a8fb09adbb) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "084.c1", 0x0000000, 0x400000, CRC(fe087e32) SHA1(e8e89faa616027e4fb9b8a865c1a67f409c93bdf) )
	ROM_LOAD16_BYTE( "084.c2", 0x0000001, 0x400000, CRC(07864e09) SHA1(0817fcfd75d0735fd8ff27561eaec371e4ff5829) )
	ROM_LOAD16_BYTE( "084.c3", 0x0800000, 0x400000, CRC(a4e65d1b) SHA1(740a405b40b3a4b324697d2652cae29ffe0ac0bd) )
	ROM_LOAD16_BYTE( "084.c4", 0x0800001, 0x400000, CRC(c1ace468) SHA1(74ea2a3cfd7b744f0988a05baaff10016ca8f625) )
	ROM_LOAD16_BYTE( "084.c5", 0x1000000, 0x200000, CRC(8a2c1edc) SHA1(67866651bc0ce27122285a66b0aab108acf3d065) )
	ROM_LOAD16_BYTE( "084.c6", 0x1000001, 0x200000, CRC(f593ac35) SHA1(302c92c63f092a8d49429c3331e5e5678f0ea48d) )
	ROM_LOAD16_BYTE( "084.c7", 0x1800000, 0x100000, CRC(9904025f) SHA1(eec770746a0ad073f7d353ab16a2cc3a5278d307) )
	ROM_LOAD16_BYTE( "084.c8", 0x1800001, 0x100000, CRC(78eb0f9b) SHA1(2925ea21ed2ce167f08a25589e94f28643379034) )
ROM_END

ROM_START( kof96hack01 ) 
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214_hack01.p1", 0x000000, 0x100000, CRC(0b385fab) SHA1(ea544d95438b373e3f5f530528413ff583d2b329) )
	ROM_LOAD16_WORD_SWAP( "214.p2", 0x100000, 0x200000, CRC(002ccb73) SHA1(3ae8df682c75027ca82db25491021eeba00a267e) )

	NEO_SFIX_128K( "214.s1", CRC(1254cbdb) SHA1(fce5cf42588298711a3633e9c9c1d4dcb723ac76) )

	NEO_BIOS_AUDIO_128K( "214.m1", CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214.c1", 0x0000000, 0x400000, CRC(7ecf4aa2) SHA1(f773c4c1f05d58dd37e7bb2ac1d1e0ec43998a71) )
	ROM_LOAD16_BYTE( "214.c2", 0x0000001, 0x400000, CRC(05b54f37) SHA1(cc31653fe4cb05201fba234e080cb9c7a7592b1b) )
	ROM_LOAD16_BYTE( "214.c3", 0x0800000, 0x400000, CRC(64989a65) SHA1(e6f3749d43be0afa9dad7b085cb782ba694252ca) )
	ROM_LOAD16_BYTE( "214.c4", 0x0800001, 0x400000, CRC(afbea515) SHA1(ae875052728de33174827705646bd14cf3937b5c) )
	ROM_LOAD16_BYTE( "214.c5", 0x1000000, 0x400000, CRC(2a3bbd26) SHA1(7c1a7e50a10a1b082e0d0d515c34135ee9f995ac) )
	ROM_LOAD16_BYTE( "214.c6", 0x1000001, 0x400000, CRC(44d30dc7) SHA1(c8ae001e37224b55d9e4a4d99f6578b4f6eb055f) )
	ROM_LOAD16_BYTE( "214_hack01.c7",  0x1800000, 0x400000, CRC(3459297a) SHA1(cc839fdf327a7e3c107132d871d617d3cdef9f94) )
	ROM_LOAD16_BYTE( "214_hack01.c8",  0x1800001, 0x400000, CRC(8266c111) SHA1(3b3046e896b2720f6a34c2091ce1d68e581cc44e) )
ROM_END

ROM_START( kof96hack02 ) //kof96ae20x
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214_hack02.p1",  0x000000, 0x100000, CRC(c718ea76) SHA1(8745ee88bc40d1a05488d6d84d18be82ab63810a) )
	ROM_LOAD16_WORD_SWAP( "214_hack02.p2",  0x100000, 0x400000, CRC(2638be07) SHA1(3bac4851b69984dbc1bba5b5e1460ffed92af404) )

	NEO_SFIX_128K( "214_hack02.s1", CRC(b9626494) SHA1(34d913b93d8a389cdbf9fc1da3d743f099cc5352) )

	NEO_BIOS_AUDIO_128K( "214_hack02.m1", CRC(3a4a7c21) SHA1(f730540d97f5c24fefe42133bacf04d8c02a85dd) )

	ROM_REGION( 0xC00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214_hack02.v3", 0x800000, 0x400000, CRC(f85673b0) SHA1(6378931be248b52b15496c4583602ffa3fbd5d9b) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214_hack02.c1", 0x0000000, 0x800000, CRC(a7466eea) SHA1(f0513b048782b27aa49b9cbcc396ecb05348cd24) )
	ROM_LOAD16_BYTE( "214_hack02.c2", 0x0000001, 0x800000, CRC(6ff22256) SHA1(cc92838f83d01ff1bef5d354231c223043705170) )
	ROM_LOAD16_BYTE( "214_hack02.c3", 0x1000000, 0x800000, CRC(48d81318) SHA1(2c2f4149c613da45d8b2a8545ccef4ed3cfc0d74) )
	ROM_LOAD16_BYTE( "214_hack02.c4", 0x1000001, 0x800000, CRC(d69836e6) SHA1(30cdda1a72dfe536bced936c54145217fc6ae17f) )
	ROM_LOAD16_BYTE( "214_hack02.c5", 0x2000000, 0x800000, CRC(ed71e6bb) SHA1(6e8021912029b6d1a68f3f942648b95d3724bcd4) )
	ROM_LOAD16_BYTE( "214_hack02.c6", 0x2000001, 0x800000, CRC(059fa364) SHA1(0e701f66fbf14089943c03c2a2e0401ebc10a0f2) )
ROM_END

ROM_START( kof96hack03 ) //kof96ae20x
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214_hack03.p1",  0x000000, 0x100000, CRC(5602774e) SHA1(e10f8e4dd7bc065461eb6db236f33f23e06e549a) )
	ROM_LOAD16_WORD_SWAP( "214_hack02.p2", 0x100000, 0x400000, CRC(2638be07) SHA1(3bac4851b69984dbc1bba5b5e1460ffed92af404) )

	NEO_SFIX_128K( "214_hack02.s1", CRC(b9626494) SHA1(34d913b93d8a389cdbf9fc1da3d743f099cc5352) )

	NEO_BIOS_AUDIO_128K( "214_hack02.m1", CRC(3a4a7c21) SHA1(f730540d97f5c24fefe42133bacf04d8c02a85dd) )

	ROM_REGION( 0xC00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214_hack02.v3", 0x800000, 0x400000, CRC(f85673b0) SHA1(6378931be248b52b15496c4583602ffa3fbd5d9b) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214_hack02.c1", 0x0000000, 0x800000, CRC(a7466eea) SHA1(f0513b048782b27aa49b9cbcc396ecb05348cd24) )
	ROM_LOAD16_BYTE( "214_hack02.c2", 0x0000001, 0x800000, CRC(6ff22256) SHA1(cc92838f83d01ff1bef5d354231c223043705170) )
	ROM_LOAD16_BYTE( "214_hack02.c3", 0x1000000, 0x800000, CRC(48d81318) SHA1(2c2f4149c613da45d8b2a8545ccef4ed3cfc0d74) )
	ROM_LOAD16_BYTE( "214_hack02.c4", 0x1000001, 0x800000, CRC(d69836e6) SHA1(30cdda1a72dfe536bced936c54145217fc6ae17f) )
	ROM_LOAD16_BYTE( "214_hack02.c5", 0x2000000, 0x800000, CRC(ed71e6bb) SHA1(6e8021912029b6d1a68f3f942648b95d3724bcd4) )
	ROM_LOAD16_BYTE( "214_hack02.c6", 0x2000001, 0x800000, CRC(059fa364) SHA1(0e701f66fbf14089943c03c2a2e0401ebc10a0f2) )
ROM_END

ROM_START( kof96hack04 ) //kof96sp
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214_hack04.p1", 0x000000, 0x100000, CRC(59b84bbf) SHA1(512e1eec96147510410ae09360bfef9f2749172b) )
	ROM_LOAD16_WORD_SWAP( "214_hack04.p2", 0x100000, 0x200000, CRC(6bd3d75e) SHA1(c1e517f575d2dc4148360b27dee55fba2251e59e) )

	NEO_SFIX_128K( "214_hack04.s1", CRC(36a14c3b) SHA1(2ab1a75bc8ba6b69fd8f99ff31d8082ac3d50c34) )

	NEO_BIOS_AUDIO_128K( "214.m1", CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214.c1", 0x0000000, 0x400000, CRC(7ecf4aa2) SHA1(f773c4c1f05d58dd37e7bb2ac1d1e0ec43998a71) )
	ROM_LOAD16_BYTE( "214.c2", 0x0000001, 0x400000, CRC(05b54f37) SHA1(cc31653fe4cb05201fba234e080cb9c7a7592b1b) )
	ROM_LOAD16_BYTE( "214.c3", 0x0800000, 0x400000, CRC(64989a65) SHA1(e6f3749d43be0afa9dad7b085cb782ba694252ca) )
	ROM_LOAD16_BYTE( "214.c4", 0x0800001, 0x400000, CRC(afbea515) SHA1(ae875052728de33174827705646bd14cf3937b5c) )
	ROM_LOAD16_BYTE( "214.c5", 0x1000000, 0x400000, CRC(2a3bbd26) SHA1(7c1a7e50a10a1b082e0d0d515c34135ee9f995ac) )
	ROM_LOAD16_BYTE( "214.c6", 0x1000001, 0x400000, CRC(44d30dc7) SHA1(c8ae001e37224b55d9e4a4d99f6578b4f6eb055f) )
	ROM_LOAD16_BYTE( "214.c7", 0x1800000, 0x400000, CRC(3687331b) SHA1(2be95caab76d7af51674f93884330ba73a6053e4) )
	ROM_LOAD16_BYTE( "214.c8", 0x1800001, 0x400000, CRC(fa1461ad) SHA1(6c71a7f08e4044214223a6bf80984582ab5e0328) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof96ep )
	ROM_REGION( 0x380000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214.p1",  0x000000, 0x100000, CRC(52755d74) SHA1(4232d627f1d2e6ea9fc8cf01571d77d4d5b8a1bb) )
	ROM_LOAD16_WORD_SWAP( "214.p2", 0x100000, 0x200000, CRC(002ccb73) SHA1(3ae8df682c75027ca82db25491021eeba00a267e) )
	ROM_LOAD16_WORD_SWAP( "214ep.p1",   0x300000, 0x080000, CRC(e5f2fb43) SHA1(5efff9873b30679ea924b7770606c889a1076315) )

	NEO_SFIX_128K( "214.s1", CRC(1254cbdb) SHA1(fce5cf42588298711a3633e9c9c1d4dcb723ac76) )

	NEO_BIOS_AUDIO_128K( "214.m1", CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214.c1", 0x0000000, 0x400000, CRC(7ecf4aa2) SHA1(f773c4c1f05d58dd37e7bb2ac1d1e0ec43998a71) )
	ROM_LOAD16_BYTE( "214.c2", 0x0000001, 0x400000, CRC(05b54f37) SHA1(cc31653fe4cb05201fba234e080cb9c7a7592b1b) )
	ROM_LOAD16_BYTE( "214.c3", 0x0800000, 0x400000, CRC(64989a65) SHA1(e6f3749d43be0afa9dad7b085cb782ba694252ca) )
	ROM_LOAD16_BYTE( "214.c4", 0x0800001, 0x400000, CRC(afbea515) SHA1(ae875052728de33174827705646bd14cf3937b5c) )
	ROM_LOAD16_BYTE( "214.c5", 0x1000000, 0x400000, CRC(2a3bbd26) SHA1(7c1a7e50a10a1b082e0d0d515c34135ee9f995ac) )
	ROM_LOAD16_BYTE( "214.c6", 0x1000001, 0x400000, CRC(44d30dc7) SHA1(c8ae001e37224b55d9e4a4d99f6578b4f6eb055f) )
	ROM_LOAD16_BYTE( "214.c7", 0x1800000, 0x400000, CRC(3687331b) SHA1(2be95caab76d7af51674f93884330ba73a6053e4) )
	ROM_LOAD16_BYTE( "214.c8", 0x1800001, 0x400000, CRC(fa1461ad) SHA1(6c71a7f08e4044214223a6bf80984582ab5e0328) )
ROM_END

ROM_START( kof96cn )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "214cn.p1", 0x000000, 0x100000, CRC(a8c25f0a) SHA1(f6b0f82ec7baa235fa68713bb1b12e3e83db8be5) )
	ROM_LOAD16_WORD_SWAP( "214cn.p2", 0x100000, 0x200000, CRC(0edface5) SHA1(2fb7a3e84758a6ce3977b6a1f306867cb52fe0f5) )
	ROM_LOAD16_WORD_SWAP( "214cn.p3", 0x300000, 0x500000, CRC(8724a441) SHA1(409a93e3bd7f40dfd623c2ed1f7173fa55f597b8) )

	NEO_SFIX_128K( "214c.s1", CRC(f917527a) SHA1(8c4778f9dc5e0651042f8ff6f1a18e3f40d0bf7c) )

	NEO_BIOS_AUDIO_128K( "214.m1", CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "214.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "214.c1", 0x0000000, 0x400000, CRC(7ecf4aa2) SHA1(f773c4c1f05d58dd37e7bb2ac1d1e0ec43998a71) )
	ROM_LOAD16_BYTE( "214.c2", 0x0000001, 0x400000, CRC(05b54f37) SHA1(cc31653fe4cb05201fba234e080cb9c7a7592b1b) )
	ROM_LOAD16_BYTE( "214.c3", 0x0800000, 0x400000, CRC(64989a65) SHA1(e6f3749d43be0afa9dad7b085cb782ba694252ca) )
	ROM_LOAD16_BYTE( "214.c4", 0x0800001, 0x400000, CRC(afbea515) SHA1(ae875052728de33174827705646bd14cf3937b5c) )
	ROM_LOAD16_BYTE( "214.c5", 0x1000000, 0x400000, CRC(2a3bbd26) SHA1(7c1a7e50a10a1b082e0d0d515c34135ee9f995ac) )
	ROM_LOAD16_BYTE( "214.c6", 0x1000001, 0x400000, CRC(44d30dc7) SHA1(c8ae001e37224b55d9e4a4d99f6578b4f6eb055f) )
	ROM_LOAD16_BYTE( "214.c7", 0x1800000, 0x400000, CRC(3687331b) SHA1(2be95caab76d7af51674f93884330ba73a6053e4) )
	ROM_LOAD16_BYTE( "214.c8", 0x1800001, 0x400000, CRC(fa1461ad) SHA1(6c71a7f08e4044214223a6bf80984582ab5e0328) )
	ROM_LOAD16_BYTE( "214cn.c9",  0x2000000, 0x400000, CRC(a9f811d2) SHA1(abd1bd95d2f44fc4b604294c12c5509b41509ed7) )
	ROM_LOAD16_BYTE( "214cn.c10", 0x2000001, 0x400000, CRC(1147406a) SHA1(2bccbd2f38f15c13eb7d5a89fd9d85f595e23bc3) )
ROM_END

 /***************************
   The King of Fighters '97
*****************************/

ROM_START( kof97hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack01.p1",  0x000000, 0x100000, CRC(b76c6ad6) SHA1(b1871c5c04e6671dd16d7c5271cade0307921825) )
	ROM_LOAD16_WORD_SWAP( "232.p2", 0x100000, 0x400000, CRC(158b23f6) SHA1(9744620a70513490aaf9c5eda33e5ec31222be19) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack01.c5",  0x2000000, 0x400000, CRC(6fe02054) SHA1(47b3a6f0867e15900deb3ffd5f888cbcf6e2bbdd) )
	ROM_LOAD16_BYTE( "232_hack01.c6",  0x2000001, 0x400000, CRC(0f96c84a) SHA1(79a8e1ff71051ece0ca6c8f75b0bd1cd567f2e33) )
ROM_END

ROM_START( kof97hack02 ) //kof97t2020
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack02.p1", 0x000000, 0x100000, CRC(53d9851d) SHA1(5f718a0edbca935b4b8066c516f34e0de6f63609) )
	ROM_LOAD16_WORD_SWAP( "232_hack02.p2", 0x100000, 0x400000, CRC(6d448462) SHA1(7eb71bb8a0ceda96043025d5a5f07143a07283ea) )

	NEO_SFIX_128K( "232_hack02.s1", CRC(e338d57f) SHA1(4a914bcf0ffd011d4062e0bda9b47cb725324ff0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack02.c1", 0x0000000, 0x800000, CRC(848040cb) SHA1(99d3f992fa3b489d00b1fefe95b510c4156ed6fa) )
	ROM_LOAD16_BYTE( "232_hack02.c2", 0x0000001, 0x800000, CRC(d01ad7c9) SHA1(d9979a377ded2cce1edcb13938c071aad9f5936c) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack02.c5", 0x2000000, 0x800000, CRC(7bb13ff4) SHA1(4ee2c5a655edad1ea85f24b9f444dacd802c8f20) )
	ROM_LOAD16_BYTE( "232_hack02.c6", 0x2000001, 0x800000, CRC(6846a8a5) SHA1(7c507638ad731e641b688319da44453d8494cc1f) )
ROM_END

ROM_START( kof97hack03 ) // Playstation Hack
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack03.p1", 0x000000, 0x100000, CRC(fc25feec) SHA1(5e2677ce1011789bd74c208cd6b1286f4ca8f4bd) )
	ROM_LOAD16_WORD_SWAP( "232_hack03.p2", 0x100000, 0x400000, CRC(90723643) SHA1(62d95957ae51732de30af903dc8f07f5556d15f5) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack03.c1",  0x0000000, 0x800000, CRC(748ade86) SHA1(3b4d84e978b03f8cd8f2155da524870096e3734f) )
	ROM_LOAD16_BYTE( "232_hack03.c2",  0x0000001, 0x800000, CRC(3270fa6e) SHA1(16d6afdf0ed437303a069184bbbbcbc0719f3db6) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack03.c5",  0x2000000, 0x400000, CRC(47711a8c) SHA1(4105b1326a83d042dd998431c7bea18ee08f124c) )
	ROM_LOAD16_BYTE( "232_hack03.c6",  0x2000001, 0x400000, CRC(6703a48a) SHA1(ffdc3f8e65a0197d9fcb9bd0d1d945061bf9736e) )
ROM_END

ROM_START( kof97hack04 ) //kof97ae
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack04.p1", 0x000000, 0x100000, CRC(e453b221) SHA1(62dd2288fd061fcd7bf672dd32d7e7b2157a5e01) )
	ROM_LOAD16_WORD_SWAP( "232_hack04.p2", 0x100000, 0x400000, CRC(f00ba90d) SHA1(899062146c5d655d1e832c6fed70cdea1f121180) )

	NEO_SFIX_128K( "232_hack04.s1", CRC(30a6b3b8) SHA1(22c62108346dcb02f3aa6f46f4c0c8140c75115a) )

	NEO_BIOS_AUDIO_128K( "232_hack04.m1", CRC(f7b7ee13) SHA1(7afbe582569966c69737df0b5e2c3449fa955585) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )
	ROM_LOAD( "232_hack04.v4", 0xc00000, 0x200000, CRC(7d37caa0) SHA1(4c7cfc54cc8133a16ab88570fb390319af74de96) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack04.c1",  0x0000000, 0x800000, CRC(5f0133eb) SHA1(c28f196272775666b479d168f09fde9d90d06bf7) )
	ROM_LOAD16_BYTE( "232_hack04.c2",  0x0000001, 0x800000, CRC(136b8449) SHA1(3514a4d589fb1c0aadc9af1074422b5a52406de5) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack04.c5",  0x2000000, 0x800000, CRC(8167b444) SHA1(62fb1271e300345795253d65e0f3496713aa6a1f) )
	ROM_LOAD16_BYTE( "232_hack04.c6",  0x2000001, 0x800000, CRC(a410f3ca) SHA1(162b4a935a1a3b74bbf4bc7f8219494cf7ac73c1) )
	ROM_LOAD16_BYTE( "232_hack04.c7",  0x3000000, 0x800000, CRC(50a48a56) SHA1(7be16bd8fe0a97264357a60a01a52b43aea14037) )
	ROM_LOAD16_BYTE( "232_hack04.c8",  0x3000001, 0x800000, CRC(80d7ec00) SHA1(52208de912675dcac19502e690a9c5a5bccabc7e) )
ROM_END

ROM_START( kof97hack05 ) //kof97ae
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232h206.p1", 0x000000, 0x100000, CRC(3ea666b2) SHA1(24099dfb25b7d9072641fe08f619445006bcd094) )
	ROM_LOAD16_WORD_SWAP( "232_hack04.p2", 0x100000, 0x400000, CRC(f00ba90d) SHA1(899062146c5d655d1e832c6fed70cdea1f121180) )

	NEO_SFIX_128K( "232_hack04.s1", CRC(30a6b3b8) SHA1(22c62108346dcb02f3aa6f46f4c0c8140c75115a) )

	NEO_BIOS_AUDIO_128K( "232_hack04.m1", CRC(f7b7ee13) SHA1(7afbe582569966c69737df0b5e2c3449fa955585) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )
	ROM_LOAD( "232_hack04.v4", 0xc00000, 0x200000, CRC(7d37caa0) SHA1(4c7cfc54cc8133a16ab88570fb390319af74de96) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack04.c1",  0x0000000, 0x800000, CRC(5f0133eb) SHA1(c28f196272775666b479d168f09fde9d90d06bf7) )
	ROM_LOAD16_BYTE( "232_hack04.c2",  0x0000001, 0x800000, CRC(136b8449) SHA1(3514a4d589fb1c0aadc9af1074422b5a52406de5) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack04.c5",  0x2000000, 0x800000, CRC(8167b444) SHA1(62fb1271e300345795253d65e0f3496713aa6a1f) )
	ROM_LOAD16_BYTE( "232_hack04.c6",  0x2000001, 0x800000, CRC(a410f3ca) SHA1(162b4a935a1a3b74bbf4bc7f8219494cf7ac73c1) )
	ROM_LOAD16_BYTE( "232_hack04.c7",  0x3000000, 0x800000, CRC(50a48a56) SHA1(7be16bd8fe0a97264357a60a01a52b43aea14037) )
	ROM_LOAD16_BYTE( "232_hack04.c8",  0x3000001, 0x800000, CRC(80d7ec00) SHA1(52208de912675dcac19502e690a9c5a5bccabc7e) )
ROM_END

ROM_START( kof97hack06 ) //kof97cbt
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack06.p1", 0x000000, 0x100000, CRC(323e4263) SHA1(0a57f55578ae1e807c977394da95450cfcff5267) )
	ROM_LOAD16_WORD_SWAP( "232_hack06.p2", 0x100000, 0x400000, CRC(91c0cfdb) SHA1(13c815f91b271316c4650d322077808428003562) )

	NEO_SFIX_128K( "232_hack06.s1", CRC(d4957067) SHA1(b756c97b050296db9b00e0bc6cdd79e4d2e2273e) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack06.c1", 0x0000000, 0x800000, CRC(ea16b711) SHA1(e54d1a0b559f1ef326ee86bc89e9f1b399f08b32) )
	ROM_LOAD16_BYTE( "232_hack06.c2", 0x0000001, 0x800000, CRC(b7bef674) SHA1(378a67dfa7dba03900dd7c70a3473c12f9879af1) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack01.c5",  0x2000000, 0x400000, CRC(6FE02054) SHA1(47b3a6f0867e15900deb3ffd5f888cbcf6e2bbdd) )
	ROM_LOAD16_BYTE( "232_hack01.c6",  0x2000001, 0x400000, CRC(0F96C84A) SHA1(79a8e1ff71051ece0ca6c8f75b0bd1cd567f2e33) )
ROM_END

ROM_START( kof97hack07 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack07.p1", 0x000000, 0x100000, CRC(2e4f4e3b) SHA1(122b749fe68057fa030d4f0af270883e99246a31) )
	ROM_LOAD16_WORD_SWAP( "232_hack07.p2", 0x100000, 0x400000, CRC(6d4503ce) SHA1(95217d0b8f51a92cb98eb17d4a9c106a1a0a7920) )

	NEO_SFIX_128K( "232_hack07.s1", CRC(d6fe166f) SHA1(55e70395466ab68e7a170be3aa6f5978e704701b) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack07.c1", 0x0000000, 0x800000, CRC(d504bf4a) SHA1(a58f6c07080c666ae1bb3e2dbab4790feab5ccc7) )
	ROM_LOAD16_BYTE( "232_hack07.c2", 0x0000001, 0x800000, CRC(942ea708) SHA1(20389bdb253611745c5515206b1eb620a1574711) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack07.c5", 0x2000000, 0x800000, CRC(c1bd2375) SHA1(1fbd0e714d5e0e61b90a7a04c5eab3fa4652dfe5) )
	ROM_LOAD16_BYTE( "232_hack07.c6", 0x2000001, 0x800000, CRC(a7c8506a) SHA1(c545b5ca9a8559caa17593c5dc137f939f4f42e7) )
ROM_END

ROM_START( kof97hack08 ) //kof97rc
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack08.p1", 0x000000, 0x100000, CRC(1d476235) SHA1(eee17d407c97f68578abd8ce2e9d975c38c706cd) )
	ROM_LOAD16_WORD_SWAP( "232_hack08.p2", 0x100000, 0x400000, CRC(406e4a4b) SHA1(1959c4e7462eb63bc90f085c639e12a9c6429efa) )

	NEO_SFIX_128K( "232_hack06.s1", CRC(d4957067) SHA1(b756c97b050296db9b00e0bc6cdd79e4d2e2273e) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack08.c1", 0x0000000, 0x800000, CRC(e2fe7d7a) SHA1(55adbad77ebac9cd10280e17cb5eac6720424d30) )
	ROM_LOAD16_BYTE( "232_hack08.c2", 0x0000001, 0x800000, CRC(d0746893) SHA1(ec17600eabc1db3fffe87cc982e929847cc8cd51) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack01.c5",  0x2000000, 0x400000, CRC(6FE02054) SHA1(47b3a6f0867e15900deb3ffd5f888cbcf6e2bbdd) )
	ROM_LOAD16_BYTE( "232_hack01.c6",  0x2000001, 0x400000, CRC(0F96C84A) SHA1(79a8e1ff71051ece0ca6c8f75b0bd1cd567f2e33) )
ROM_END

ROM_START( kof97hack09 )  // kof97sw2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack09.p1",  0x000000, 0x100000, CRC(5d279e62) SHA1(0805f45494572f6743443e404c977ee11352c85a) )
	ROM_LOAD16_WORD_SWAP( "232_hack09.p2", 0x100000, 0x400000, CRC(b6dd9e89) SHA1(b4429521a7b4114a458c465d8a95147b84d7b9d5) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack09.c5", 0x2000000, 0x800000, CRC(c81698e5) SHA1(2d4bc8d1ccc8ca9e2d41776e1f66d7dfb8203664) )
	ROM_LOAD16_BYTE( "232_hack09.c6", 0x2000001, 0x800000, CRC(b88f5db7) SHA1(70737eb6e87efc7a93d7a7d57303333615bef4fb) )
ROM_END

ROM_START( kof97hack10 )  // kof97sw2s
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack10.p1", 0x000000, 0x100000, CRC(d9bfe3ba) SHA1(95260cc996b8d02f88cd4c8fa04d0b3aa73e7dd4) )
	ROM_LOAD16_WORD_SWAP( "232_hack09.p2", 0x100000, 0x400000, CRC(b6dd9e89) SHA1(b4429521a7b4114a458c465d8a95147b84d7b9d5) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack09.c5", 0x2000000, 0x800000, CRC(c81698e5) SHA1(2d4bc8d1ccc8ca9e2d41776e1f66d7dfb8203664) )
	ROM_LOAD16_BYTE( "232_hack09.c6", 0x2000001, 0x800000, CRC(b88f5db7) SHA1(70737eb6e87efc7a93d7a7d57303333615bef4fb) )
ROM_END

ROM_START( kof97hack11 ) //kof97yk83h
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack11.p1", 0x000000, 0x100000, CRC(bea12d47) SHA1(1debd99ec98d5c0d099595d04d574b655775ef34) )
	ROM_LOAD16_WORD_SWAP( "232_hack11.p2", 0x100000, 0x600000, CRC(cdf2c7b8) SHA1(630298c51cf190865a0ed960b96f018104c2e5ef) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack13.s1", CRC(ca1d4290) SHA1(658ad9d6d4a153af19f2e9d04e78c879bd4c1a77) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack11.c1", 0x0000000, 0x800000, CRC(f748da71) SHA1(5012e8537e7d16d365d9c088a70323b3b71cd6f5) )
	ROM_LOAD16_BYTE( "232_hack11.c2", 0x0000001, 0x800000, CRC(6b77e767) SHA1(7f625fa348754a248d9b58bd2f085c47142faf91) )
	ROM_LOAD16_BYTE( "232_hack11.c3", 0x1000000, 0x800000, CRC(ffb025af) SHA1(64913b51b496a13a92317008d2a15543e6c89ba9) )
	ROM_LOAD16_BYTE( "232_hack11.c4", 0x1000001, 0x800000, CRC(7182157c) SHA1(306a968ae325d0b7d647e7fdb36a3a07bacda923) )
	ROM_LOAD16_BYTE( "232_hack11.c5", 0x2000000, 0x800000, CRC(00a77a26) SHA1(a86054fd3870be49ad6055a2fab663cb5ef27de2) )
	ROM_LOAD16_BYTE( "232_hack11.c6", 0x2000001, 0x800000, CRC(0b504e35) SHA1(597cabd5077670827ad3e809d6cd52c9c7f3013a) )
	ROM_LOAD16_BYTE( "232_hack11.c7", 0x3000000, 0x800000, CRC(8024278a) SHA1(4e56535a9fc088b3646f050b534b075e581105a7) )
	ROM_LOAD16_BYTE( "232_hack11.c8", 0x3000001, 0x800000, CRC(75469b4c) SHA1(640ba75c739cade673bea2574d7b9350f9ce8493) )
ROM_END

ROM_START( kof97hack12 ) //kof97yk83h
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack12.p1", 0x000000, 0x100000, CRC(cab04096) SHA1(5b12a945f97c5767ddaf0d8419ade2159dea104f) )
	ROM_LOAD16_WORD_SWAP( "232_hack11.p2", 0x100000, 0x600000, CRC(cdf2c7b8) SHA1(630298c51cf190865a0ed960b96f018104c2e5ef) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack13.s1", CRC(ca1d4290) SHA1(658ad9d6d4a153af19f2e9d04e78c879bd4c1a77) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack11.c1", 0x0000000, 0x800000, CRC(f748da71) SHA1(5012e8537e7d16d365d9c088a70323b3b71cd6f5) )
	ROM_LOAD16_BYTE( "232_hack11.c2", 0x0000001, 0x800000, CRC(6b77e767) SHA1(7f625fa348754a248d9b58bd2f085c47142faf91) )
	ROM_LOAD16_BYTE( "232_hack11.c3", 0x1000000, 0x800000, CRC(ffb025af) SHA1(64913b51b496a13a92317008d2a15543e6c89ba9) )
	ROM_LOAD16_BYTE( "232_hack11.c4", 0x1000001, 0x800000, CRC(7182157c) SHA1(306a968ae325d0b7d647e7fdb36a3a07bacda923) )
	ROM_LOAD16_BYTE( "232_hack11.c5", 0x2000000, 0x800000, CRC(00a77a26) SHA1(a86054fd3870be49ad6055a2fab663cb5ef27de2) )
	ROM_LOAD16_BYTE( "232_hack11.c6", 0x2000001, 0x800000, CRC(0b504e35) SHA1(597cabd5077670827ad3e809d6cd52c9c7f3013a) )
	ROM_LOAD16_BYTE( "232_hack11.c7", 0x3000000, 0x800000, CRC(8024278a) SHA1(4e56535a9fc088b3646f050b534b075e581105a7) )
	ROM_LOAD16_BYTE( "232_hack11.c8", 0x3000001, 0x800000, CRC(75469b4c) SHA1(640ba75c739cade673bea2574d7b9350f9ce8493) )
ROM_END

ROM_START( kof97hack13 ) //kof97yk85h
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack13.p1", 0x000000, 0x100000, CRC(cb471dc6) SHA1(60e9a9f008d83cf3616c0885c503763478949e84) )
	ROM_LOAD16_WORD_SWAP( "232_hack13.p2", 0x100000, 0x600000, CRC(1e6c4c11) SHA1(8fa7864d35665bb76c0a1cbc8255338da6f35bdb) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack13.s1", CRC(ca1d4290) SHA1(658ad9d6d4a153af19f2e9d04e78c879bd4c1a77) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )
	ROM_LOAD( "265-nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) ) //in romset but m1 is not changed so i doubt it is used

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack13.c1",  0x0000000, 0x800000, CRC(e47371bb) SHA1(e0e107f4edfbac0a7638b240405745bf31dbadf9) )
	ROM_LOAD16_BYTE( "232_hack13.c2",  0x0000001, 0x800000, CRC(e09108b0) SHA1(b19e0101ed500dec65156a6c19334125a4012521) )
	ROM_LOAD16_BYTE( "232_hack11.c3",  0x1000000, 0x800000, CRC(ffb025af) SHA1(64913b51b496a13a92317008d2a15543e6c89ba9) )
	ROM_LOAD16_BYTE( "232_hack11.c4",  0x1000001, 0x800000, CRC(7182157c) SHA1(306a968ae325d0b7d647e7fdb36a3a07bacda923) )
	ROM_LOAD16_BYTE( "232_hack13.c5",  0x2000000, 0x800000, CRC(c61653cc) SHA1(05674fb5b0007f200b090e93863335fdf474fa64) )
	ROM_LOAD16_BYTE( "232_hack13.c6",  0x2000001, 0x800000, CRC(a01a762f) SHA1(68df7a1753745b9468c4f464779c2c5033f16458) )
	ROM_LOAD16_BYTE( "232_hack13.c7",  0x3000000, 0x800000, CRC(52ac1bcf) SHA1(51f026db5bac479b82297260b55575f8a60306c7) )
	ROM_LOAD16_BYTE( "232_hack13.c8",  0x3000001, 0x800000, CRC(76de561d) SHA1(8f27e746372daf77ff41c4ee431fa36ae22c7b8d) )
	ROM_LOAD16_BYTE( "232_hack15.c9",  0x4000000, 0x800000, CRC(46eb506a) SHA1(1b64fb31dfdc95b901b2e3d359bdee38f764358f) )
	ROM_LOAD16_BYTE( "232_hack15.c10", 0x4000001, 0x800000, CRC(33595746) SHA1(1532ce71b27beace35f359568d506eaf75ad578d) )
ROM_END

ROM_START( kof97hack14 ) //kof97yk85h
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack14.p1", 0x000000, 0x100000, CRC(bf567017) SHA1(9eaef2bb1b6172923b3e62adf574a56b3f6fc7c6) )
	ROM_LOAD16_WORD_SWAP( "232_hack13.p2", 0x100000, 0x600000, CRC(1e6c4c11) SHA1(8fa7864d35665bb76c0a1cbc8255338da6f35bdb) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack13.s1", CRC(ca1d4290) SHA1(658ad9d6d4a153af19f2e9d04e78c879bd4c1a77) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )
	ROM_LOAD( "265-nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) ) //in romset but m1 is not changed so i doubt it is used

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack13.c1",  0x0000000, 0x800000, CRC(e47371bb) SHA1(e0e107f4edfbac0a7638b240405745bf31dbadf9) )
	ROM_LOAD16_BYTE( "232_hack13.c2",  0x0000001, 0x800000, CRC(e09108b0) SHA1(b19e0101ed500dec65156a6c19334125a4012521) )
	ROM_LOAD16_BYTE( "232_hack11.c3",  0x1000000, 0x800000, CRC(ffb025af) SHA1(64913b51b496a13a92317008d2a15543e6c89ba9) )
	ROM_LOAD16_BYTE( "232_hack11.c4",  0x1000001, 0x800000, CRC(7182157c) SHA1(306a968ae325d0b7d647e7fdb36a3a07bacda923) )
	ROM_LOAD16_BYTE( "232_hack13.c5",  0x2000000, 0x800000, CRC(c61653cc) SHA1(05674fb5b0007f200b090e93863335fdf474fa64) )
	ROM_LOAD16_BYTE( "232_hack13.c6",  0x2000001, 0x800000, CRC(a01a762f) SHA1(68df7a1753745b9468c4f464779c2c5033f16458) )
	ROM_LOAD16_BYTE( "232_hack13.c7",  0x3000000, 0x800000, CRC(52ac1bcf) SHA1(51f026db5bac479b82297260b55575f8a60306c7) )
	ROM_LOAD16_BYTE( "232_hack13.c8",  0x3000001, 0x800000, CRC(76de561d) SHA1(8f27e746372daf77ff41c4ee431fa36ae22c7b8d) )
	ROM_LOAD16_BYTE( "232_hack15.c9",  0x4000000, 0x800000, CRC(46eb506a) SHA1(1b64fb31dfdc95b901b2e3d359bdee38f764358f) )
	ROM_LOAD16_BYTE( "232_hack15.c10", 0x4000001, 0x800000, CRC(33595746) SHA1(1532ce71b27beace35f359568d506eaf75ad578d) )
ROM_END

ROM_START( kof97hack15 ) //kof97ykzeroh
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack15.p1", 0x000000, 0x100000, CRC(33977e2c) SHA1(d37123f5c56f82882cddbe091810d73d76c4518e) )
	ROM_LOAD16_WORD_SWAP( "232_hack15.p2", 0x100000, 0x400000, CRC(ef89bede) SHA1(142fa2670b6b24bf268e1dd12244a5923b5b6208) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack15.s1", CRC(d8826314) SHA1(5d3ffdf709ee8f84e451464866d0ddcaf98bdb89) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack15.c1", 0x0000000, 0x800000, CRC(93415994) SHA1(b0473816e3ff82d68e66be3722ac39af19e05d7d) )
	ROM_LOAD16_BYTE( "232_hack15.c2", 0x0000001, 0x800000, CRC(65b3b5b0) SHA1(076d640b71103c6fe1cf3ffd001eaa78e3da178d) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack15.c5", 0x2000000, 0x800000, CRC(46fc8247) SHA1(c959868338d35fcc2c4ed7066040bfe00e9999e5) )
	ROM_LOAD16_BYTE( "232_hack15.c6", 0x2000001, 0x800000, CRC(0d39f4a8) SHA1(5f0119907d1991d9ccb41b44adf9b6b1cee05267) )
	ROM_LOAD16_BYTE( "232_hack15.c7", 0x3000000, 0x800000, CRC(9a0a273e) SHA1(0ff61b3e740d86e3c2870b8f4b9f3920bb680b5d) )
	ROM_LOAD16_BYTE( "232_hack15.c8", 0x3000001, 0x800000, CRC(e52514d5) SHA1(5c07fcfff411251088d76b429226b242355d7ede) )
	ROM_LOAD16_BYTE( "232_hack15.c9",  0x4000000, 0x800000, CRC(46eb506a) SHA1(1b64fb31dfdc95b901b2e3d359bdee38f764358f) )
	ROM_LOAD16_BYTE( "232_hack15.c10", 0x4000001, 0x800000, CRC(33595746) SHA1(1532ce71b27beace35f359568d506eaf75ad578d) )
ROM_END

ROM_START( kof97hack16 ) //kof97ykzeroh
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack16.p1", 0x000000, 0x100000, CRC(0dc3fde3) SHA1(e8eb19ff86940c175345b57e5e2389ea8d8eedb5) )
	ROM_LOAD16_WORD_SWAP( "232_hack15.p2", 0x100000, 0x400000, CRC(ef89bede) SHA1(142fa2670b6b24bf268e1dd12244a5923b5b6208) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack15.s1", CRC(d8826314) SHA1(5d3ffdf709ee8f84e451464866d0ddcaf98bdb89) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack15.c1", 0x0000000, 0x800000, CRC(93415994) SHA1(b0473816e3ff82d68e66be3722ac39af19e05d7d) )
	ROM_LOAD16_BYTE( "232_hack15.c2", 0x0000001, 0x800000, CRC(65b3b5b0) SHA1(076d640b71103c6fe1cf3ffd001eaa78e3da178d) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack15.c5", 0x2000000, 0x800000, CRC(46fc8247) SHA1(c959868338d35fcc2c4ed7066040bfe00e9999e5) )
	ROM_LOAD16_BYTE( "232_hack15.c6", 0x2000001, 0x800000, CRC(0d39f4a8) SHA1(5f0119907d1991d9ccb41b44adf9b6b1cee05267) )
	ROM_LOAD16_BYTE( "232_hack15.c7", 0x3000000, 0x800000, CRC(9a0a273e) SHA1(0ff61b3e740d86e3c2870b8f4b9f3920bb680b5d) )
	ROM_LOAD16_BYTE( "232_hack15.c8", 0x3000001, 0x800000, CRC(e52514d5) SHA1(5c07fcfff411251088d76b429226b242355d7ede) )
	ROM_LOAD16_BYTE( "232_hack15.c9", 0x4000000, 0x800000, CRC(46eb506a) SHA1(1b64fb31dfdc95b901b2e3d359bdee38f764358f) )
	ROM_LOAD16_BYTE( "232_hack15.c10",0x4000001, 0x800000, CRC(33595746) SHA1(1532ce71b27beace35f359568d506eaf75ad578d) )
ROM_END

ROM_START( kof97hack17 ) //kof97rugal
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack17.p1", 0x000000, 0x100000, CRC(082dd515) SHA1(544601c64898e7567b851c298eafc702bbbad8c4) )
	ROM_LOAD16_WORD_SWAP( "232_hack17.p2", 0x100000, 0x400000, CRC(4c4383cb) SHA1(f4aab08d96c882b30b7fa7b907547e7647415a51) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232.c5", 0x2000000, 0x400000, CRC(34fc4e51) SHA1(b39c65f27873f71a6f5a5d1d04e5435f874472ee) )
	ROM_LOAD16_BYTE( "232.c6", 0x2000001, 0x400000, CRC(4ff4d47b) SHA1(4d5689ede24a5fe4330bd85d4d3f4eb2795308bb) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof97hack18 ) //kof9714iori
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack18.p1",  0x000000, 0x100000, CRC(3a7a37a2) SHA1(aca92db675d1a7b1d0a2a117bed7174672ba954d) )
	ROM_LOAD16_WORD_SWAP( "232_hack18.p2",  0x100000, 0x400000, CRC(0c4ce900) SHA1(fcef835ae1327afae65f43f91bb88afdf5c60bb0) )

	NEO_SFIX_128K( "232.s1", CRC(8514ecf5) SHA1(18d8e7feb51ea88816f1c786932a53655b0de6a0) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack18.c5",  0x2000000, 0x800000, CRC(a262015e) SHA1(6ac99a1d560eeae00d91e61922d4b6ec429f9536) )
	ROM_LOAD16_BYTE( "232_hack18.c6",  0x2000001, 0x800000, CRC(5155b008) SHA1(a4f386142e39766f85d73088b692b8a58c679eb0) )
ROM_END

ROM_START( kof97hack19 ) //kof9714iori
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack19.p1",  0x000000, 0x100000, CRC(5e07f753) SHA1(60de45fe10563a77ca667d54648af179a1aa59a5) )
	ROM_LOAD16_WORD_SWAP( "232_hack19.p2",  0x100000, 0x400000, CRC(6c82a388) SHA1(79966a4ba618f5988d01e481b50d28da14dcff38) )

	NEO_SFIX_128K( "232_hack19.s1", CRC(ba445f53) SHA1(cda0fb620b90a82c1343b004a9085b37890afc58) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack18.c5",  0x2000000, 0x800000, CRC(a262015e) SHA1(6ac99a1d560eeae00d91e61922d4b6ec429f9536) )
	ROM_LOAD16_BYTE( "232_hack18.c6",  0x2000001, 0x800000, CRC(5155b008) SHA1(a4f386142e39766f85d73088b692b8a58c679eb0) )
ROM_END

ROM_START( kof97hack20 ) //kof97sp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack20.p1", 0x000000, 0x100000, CRC(a511714d) SHA1(27afea3a1753fe498610ec898920125fbabc82ef) )
	ROM_LOAD16_WORD_SWAP( "232_hack20.p2", 0x100000, 0x400000, CRC(d9e51750) SHA1(9dd00aa69361d6b9c4ed905d656c62b82c8f4f9a) )
	ROM_FILL(0x70bda,1,0x0c)
	ROM_FILL(0x70bdb,1,0x2d)
	ROM_FILL(0x70bdc,1,0x00)
	ROM_FILL(0x70bdd,1,0x05)
	ROM_FILL(0x70bde,1,0x28)
	ROM_FILL(0x70bdf,1,0x46)
	ROM_FILL(0x70be0,1,0x65)
	ROM_FILL(0x70be1,1,0x18)
	ROM_FILL(0x70be2,1,0x39)
	ROM_FILL(0x70be3,1,0x7c)
	ROM_FILL(0x70be4,1,0x00)
	ROM_FILL(0x70be5,1,0xb4)
	ROM_FILL(0x70be6,1,0x00)
	ROM_FILL(0x70be7,1,0xd2)
	ROM_FILL(0x70be8,1,0x28)
	ROM_FILL(0x70be9,1,0xbc)
	ROM_FILL(0x70bea,1,0x00)
	ROM_FILL(0x70beb,1,0x07)
	ROM_FILL(0x70bec,1,0x0b)
	ROM_FILL(0x70bed,1,0xee)

	NEO_SFIX_128K( "232_hack19.s1", CRC(ba445f53) SHA1(cda0fb620b90a82c1343b004a9085b37890afc58) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack20.c1", 0x0000000, 0x800000, CRC(b7f5a3b9) SHA1(a904503e1686ad6e3f0679370e5a0b100826f8ce) )
	ROM_LOAD16_BYTE( "232_hack20.c2", 0x0000001, 0x800000, CRC(959d6d78) SHA1(95031b35e5d6c5ed5c36503cfb6e6c907d9666d9) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232.c5", 0x2000000, 0x400000, CRC(34fc4e51) SHA1(b39c65f27873f71a6f5a5d1d04e5435f874472ee) )
	ROM_LOAD16_BYTE( "232.c6", 0x2000001, 0x400000, CRC(4ff4d47b) SHA1(4d5689ede24a5fe4330bd85d4d3f4eb2795308bb) )
ROM_END

ROM_START( kof97hack21 ) //kof97cto2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack21.p1", 0x000000, 0x100000, CRC(bd27ed3d) SHA1(c8a5a555e6ebf04264bdcdfe83aa9d25f771555a) )
	ROM_LOAD16_WORD_SWAP( "232_hack21.p2", 0x100000, 0x400000, CRC(c0c73a15) SHA1(d5303ce0b5556fdbc804e470d15e52b23500fe87) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack21.s1", CRC(0016c83a) SHA1(c95b44dc0976971b69c51d62a437cf6279f83ef9) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232_hack21.v2", 0x400000, 0x400000, CRC(92b14f89) SHA1(64809538c80e44512130d288b515dbed129bd687) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack21.c1",  0x0000000, 0x800000, CRC(a41dd2c0) SHA1(8f5eae03c4ed9ae51c7d9c069b41925b9fcf187e) )
	ROM_LOAD16_BYTE( "232_hack21.c2",  0x0000001, 0x800000, CRC(47eb128a) SHA1(dbf8ef9dd65388c42af716db609cdc32b7fbaf64) )
	ROM_LOAD16_BYTE( "232_hack21.c3",  0x1000000, 0x800000, CRC(b092e64f) SHA1(7d809432d068db8f9056f5210570191416f1a53b) )
	ROM_LOAD16_BYTE( "232_hack21.c4",  0x1000001, 0x800000, CRC(d25e8a04) SHA1(52227828e3fdd27399b4cd514551abf856293f47) )
	ROM_LOAD16_BYTE( "232_hack21.c5",  0x2000000, 0x800000, CRC(4922b325) SHA1(2c93d529bdbdd6864eb3f570f853b2e18e438558) )
	ROM_LOAD16_BYTE( "232_hack21.c6",  0x2000001, 0x800000, CRC(cebac145) SHA1(40e09ff8d08e9855ca33896b43b2703fd7a3b19c) )
ROM_END

ROM_START( kof97hack22 ) //kof97cto2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack22.p1", 0x000000, 0x100000, CRC(3e72a8b7) SHA1(71f954fd1eb1f31aa7f7acc2e7b2c7234d90e8e7) )
	ROM_LOAD16_WORD_SWAP( "232_hack21.p2", 0x100000, 0x400000, CRC(c0c73a15) SHA1(d5303ce0b5556fdbc804e470d15e52b23500fe87) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack21.s1", CRC(0016c83a) SHA1(c95b44dc0976971b69c51d62a437cf6279f83ef9) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232_hack21.v2", 0x400000, 0x400000, CRC(92b14f89) SHA1(64809538c80e44512130d288b515dbed129bd687) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack21.c1",  0x0000000, 0x800000, CRC(a41dd2c0) SHA1(8f5eae03c4ed9ae51c7d9c069b41925b9fcf187e) )
	ROM_LOAD16_BYTE( "232_hack21.c2",  0x0000001, 0x800000, CRC(47eb128a) SHA1(dbf8ef9dd65388c42af716db609cdc32b7fbaf64) )
	ROM_LOAD16_BYTE( "232_hack21.c3",  0x1000000, 0x800000, CRC(b092e64f) SHA1(7d809432d068db8f9056f5210570191416f1a53b) )
	ROM_LOAD16_BYTE( "232_hack21.c4",  0x1000001, 0x800000, CRC(d25e8a04) SHA1(52227828e3fdd27399b4cd514551abf856293f47) )
	ROM_LOAD16_BYTE( "232_hack21.c5",  0x2000000, 0x800000, CRC(4922b325) SHA1(2c93d529bdbdd6864eb3f570f853b2e18e438558) )
	ROM_LOAD16_BYTE( "232_hack21.c6",  0x2000001, 0x800000, CRC(cebac145) SHA1(40e09ff8d08e9855ca33896b43b2703fd7a3b19c) )
ROM_END

ROM_START( kof97hack23 ) //kof97athena
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack23.p1", 0x000000, 0x100000, CRC(1a5e06f4) SHA1(5b8bcd6feac79f51311d5ec81b32359454424b31) )
	ROM_LOAD16_WORD_SWAP( "232_hack22.p2", 0x100000, 0x400000, CRC(50d174ab) SHA1(ebf147a169370e0466dabc1cedb9f454ca65101e) )

	NEO_SFIX_128K( "232_hack22.s1", CRC(d0f8eda2) SHA1(589b31d3f2ba386c2ba2c2fbff56952ee86dc48b) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack22.c1", 0x0000000, 0x800000, CRC(aa665027) SHA1(a241063a1a699dde8a0bd71f2beabf013cc2d65f) )
	ROM_LOAD16_BYTE( "232_hack22.c2", 0x0000001, 0x800000, CRC(d89f601e) SHA1(31d2f6488aee5f93b50706eb529446e87538b20a) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack22.c5", 0x2000000, 0x800000, CRC(bbefeb6b) SHA1(f0e4f5d311abf27cfc77f78c0c60f8d0affaee36) )
	ROM_LOAD16_BYTE( "232_hack22.c6", 0x2000001, 0x800000, CRC(0185bda0) SHA1(1441b04eacd4a418259ffa27d0cea416e382af25) )
ROM_END

ROM_START( kof97hack24 ) //kof97athena
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232h211.p1", 0x000000, 0x100000, CRC(43c64fd0) SHA1(c82a42f8179670db04293970811486dda1fc3807) )
	ROM_LOAD16_WORD_SWAP( "232_hack22.p2", 0x100000, 0x400000, CRC(50d174ab) SHA1(ebf147a169370e0466dabc1cedb9f454ca65101e) )

	NEO_SFIX_128K( "232_hack22.s1", CRC(d0f8eda2) SHA1(589b31d3f2ba386c2ba2c2fbff56952ee86dc48b) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack22.c1", 0x0000000, 0x800000, CRC(aa665027) SHA1(a241063a1a699dde8a0bd71f2beabf013cc2d65f) )
	ROM_LOAD16_BYTE( "232_hack22.c2", 0x0000001, 0x800000, CRC(d89f601e) SHA1(31d2f6488aee5f93b50706eb529446e87538b20a) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack22.c5", 0x2000000, 0x800000, CRC(bbefeb6b) SHA1(f0e4f5d311abf27cfc77f78c0c60f8d0affaee36) )
	ROM_LOAD16_BYTE( "232_hack22.c6", 0x2000001, 0x800000, CRC(0185bda0) SHA1(1441b04eacd4a418259ffa27d0cea416e382af25) )
ROM_END

ROM_START( kof97hack25 ) //kof97cp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232_hack25.p1", 0x000000, 0x100000, CRC(07237af7) SHA1(718a5610ff559bff3e480cb8c0443293ffd6683a) )
	ROM_LOAD16_WORD_SWAP( "232_hack25.p2", 0x100000, 0x400000, CRC(d86d0095) SHA1(53d76659dacc6a7d3113fd51c29904e2ef0cf171) )
	ROM_FILL(0x263ba,1,0x65)

	NEO_SFIX_128K( "232_hack26.s1", CRC(c24e2cb7) SHA1(79f298997b773ed869a49a897aa38b74a43b051c) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232_hack07.c1", 0x0000000, 0x800000, CRC(d504bf4a) SHA1(a58f6c07080c666ae1bb3e2dbab4790feab5ccc7) )
	ROM_LOAD16_BYTE( "232_hack07.c2", 0x0000001, 0x800000, CRC(942ea708) SHA1(20389bdb253611745c5515206b1eb620a1574711) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232_hack25.c5", 0x2000000, 0x400000, CRC(e749d4d2) SHA1(8f4c54c0355decebfc47b8d11b863dcd84647688) )
	ROM_LOAD16_BYTE( "232_hack25.c6", 0x2000001, 0x400000, CRC(dc7c5cd7) SHA1(f33b62a1ded71edf0b2bd8e6f2495da34c7b1984) )
ROM_END

 /********
  bootleg
***********/

ROM_START( kof97pla )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "232pla.p1", 0x000000, 0x100000, CRC(035911c5) SHA1(3321862384ec91174f1822cfdbc2bf1d1407e775) )
	ROM_LOAD16_WORD_SWAP( "232.p2", 0x100000, 0x400000, CRC(158b23f6) SHA1(9744620a70513490aaf9c5eda33e5ec31222be19) )

	NEO_SFIX_128K( "232pla.s1", CRC(4ee2149a) SHA1(180a1a90021031eac1a643b769d9cdeda56518f5) ) // scrambled

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "232.c1", 0x0000000, 0x800000, CRC(5f8bf0a1) SHA1(e8b63bbc814de171fd18c5864a7fc639970c1ecf) )
	ROM_LOAD16_BYTE( "232.c2", 0x0000001, 0x800000, CRC(e4d45c81) SHA1(fdb2b9326362e27b1c7a5beb977e0bc537488186) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "232.c5", 0x2000000, 0x400000, CRC(34fc4e51) SHA1(b39c65f27873f71a6f5a5d1d04e5435f874472ee) )
	ROM_LOAD16_BYTE( "232.c6", 0x2000001, 0x400000, CRC(4ff4d47b) SHA1(4d5689ede24a5fe4330bd85d4d3f4eb2795308bb) )
ROM_END

 /***************************
   The King of Fighters '98
*****************************/

ROM_START( kof98hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack01.p1",  0x000000, 0x100000, CRC(9f8cb001) SHA1(0d159627815175a25e39b2d84aa0f9c7afb16b68) )
	ROM_LOAD16_WORD_SWAP( "242_hack01.p2",  0x100000, 0x400000, CRC(8a167905) SHA1(b6487e2df6450b8c192a7782d4514c1f740df41a) )

	NEO_SFIX_128K( "242_hack01.s1", CRC(c045e32a) SHA1(496f9123a50840064a1bf2d5dc8e768bc10aded1) )

	NEO_BIOS_AUDIO_256K( "242_hack01.m1", CRC(4e7a6b1b) SHA1(b54d08f88713ed0271aa06f9f7c9c572ef555b1a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack01.c1",  0x0000000, 0x800000, CRC(40aceac9) SHA1(c9b7aee462e47bb70dc3644aa20b0698e7841ac1) )
	ROM_LOAD16_BYTE( "242_hack01.c2",  0x0000001, 0x800000, CRC(3aa0b16b) SHA1(fbc063e85a6fa33ae189fe0e74ad53c6b29e3dd9) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof98hack02 ) 
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack02.p1", 0x000000, 0x100000, CRC(2f29984c) SHA1(6feb8412d335e8b38a0d7231f51af91251901dd4) )
	ROM_LOAD16_WORD_SWAP( "242_hack02.p2", 0x100000, 0x400000, CRC(c2fc3506) SHA1(ddcedbc30474d2e1db70d640a6a258171ecb900c) )

	NEO_SFIX_128K( "242.s1", CRC(7f7b4805) SHA1(80ee6e5d0ece9c34ebca54b043a7cb33f9ff6b92) )

	NEO_BIOS_AUDIO_256K( "242_hack01.m1", CRC(4e7a6b1b) SHA1(b54d08f88713ed0271aa06f9f7c9c572ef555b1a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack02.c1",  0x0000000, 0x800000, CRC(07bd2146) SHA1(124c580667887d9f027b3fb2dbb6db39b346359c) )
	ROM_LOAD16_BYTE( "242_hack02.c2",  0x0000001, 0x800000, CRC(c935e740) SHA1(375624da3f24ea3b752c0cc3d9b75457e27c31d0) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof98hack03 )
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack03.p1", 0x000000, 0x100000, CRC(d69ecaa9) SHA1(924741bfab19a43d14704aeb305970de616788e4) )
	ROM_LOAD16_WORD_SWAP( "242_hack03.p2", 0x100000, 0x600000, CRC(ac1e9e79) SHA1(95caf44bb771a7ce5663f60127cf5ae798f7dd4c) )

	NEO_SFIX( 0x40000, "242_hack03.s1", CRC(345c20b0) SHA1(ff844e47667297f09f8b49ae19987542e253095e) )

	NEO_BIOS_AUDIO_256K( "242_hack03.m1", CRC(9ade0528) SHA1(67d0c3b146d369416b84c081544fe51fc6c2a140) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )
	
	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "242_hack03.v5", 0x000000, 0x400000, CRC(afdd9660) SHA1(0d67fb61111256c0d74d4f2b473ab5a42d1909b9) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack03.c1",  0x0000000, 0x800000, CRC(086ec060) SHA1(dace47c251e5c9bea771e221bf550e1b4af5212d) )
	ROM_LOAD16_BYTE( "242_hack03.c2",  0x0000001, 0x800000, CRC(16b9a9a2) SHA1(c8b757e6a84bae33caab07de27ca6c3d84d1893a) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242_hack03.c5",  0x2000000, 0x800000, CRC(71641718) SHA1(b88a00aca2fc34230d2d2da0b235195a5eb1ecf0) )
	ROM_LOAD16_BYTE( "242_hack03.c6",  0x2000001, 0x800000, CRC(982ba2b3) SHA1(232ce3be7beaad13b35865da770157ef4b29a7a9) )
	ROM_LOAD16_BYTE( "242_hack03.c7",  0x3000000, 0x800000, CRC(8d495552) SHA1(20ff76b681b2e544c5a57060bb98ab6bb91bea3c) )
	ROM_LOAD16_BYTE( "242_hack03.c8",  0x3000001, 0x800000, CRC(8bfc3417) SHA1(38bb85563d0a7f008a64cf76d71d82b935cd98e5) )
	ROM_LOAD16_BYTE( "242_hack03.c9",  0x4000000, 0x800000, CRC(128256d1) SHA1(948af62545756165890e950fcd67bbd6304b3366) )
	ROM_LOAD16_BYTE( "242_hack03.c10", 0x4000001, 0x800000, CRC(4bd8412d) SHA1(72e9cc6e3df8000d0a7fc7a89257dd0ce56fc909) )
	ROM_LOAD16_BYTE( "242_hack03.c11", 0x5000000, 0x800000, CRC(56d361cb) SHA1(594826cf58b7cd3b0bee05cdab1265d03c057b58) )
	ROM_LOAD16_BYTE( "242_hack03.c12", 0x5000001, 0x800000, CRC(f8cb115b) SHA1(ba8a152a59acfcf72c73c6e29cd6e133630e100d) )
ROM_END

ROM_START( kof98hack04 )
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack04.p1", 0x000000, 0x100000, CRC(23a80b3e) SHA1(b1afaf382ffd5a1d72ae5ca18c11a751a11d7654) )
	ROM_LOAD16_WORD_SWAP( "242_hack04.p2", 0x100000, 0x600000, CRC(99b3e5cc) SHA1(ef2f44eff097e597bb96b187c4e69294cb5b5423) )

	NEO_SFIX_128K( "242_hack04.s1", CRC(f1fee5c0) SHA1(e0d7b6c3c4a1c24003f25de6a5238e0a8ad32269) )

	NEO_BIOS_AUDIO_256K( "242_hack03.m1", CRC(9ade0528) SHA1(67d0c3b146d369416b84c081544fe51fc6c2a140) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )
	
	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "242_hack03.v5", 0x000000, 0x400000, CRC(afdd9660) SHA1(0d67fb61111256c0d74d4f2b473ab5a42d1909b9) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack04.c1",  0x0000000, 0x800000, CRC(0319cfc9) SHA1(f275015d6bddf392936b35cd7399f929a6d63d29) )
	ROM_LOAD16_BYTE( "242_hack04.c2",  0x0000001, 0x800000, CRC(553f6714) SHA1(9c14963ce9ac0cfd125defe2fe80206deb1bc896) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242_hack03.c5",  0x2000000, 0x800000, CRC(71641718) SHA1(b88a00aca2fc34230d2d2da0b235195a5eb1ecf0) )
	ROM_LOAD16_BYTE( "242_hack03.c6",  0x2000001, 0x800000, CRC(982ba2b3) SHA1(232ce3be7beaad13b35865da770157ef4b29a7a9) )
	ROM_LOAD16_BYTE( "242_hack03.c7",  0x3000000, 0x800000, CRC(8d495552) SHA1(20ff76b681b2e544c5a57060bb98ab6bb91bea3c) )
	ROM_LOAD16_BYTE( "242_hack03.c8",  0x3000001, 0x800000, CRC(8bfc3417) SHA1(38bb85563d0a7f008a64cf76d71d82b935cd98e5) )
	ROM_LOAD16_BYTE( "242_hack03.c9",  0x4000000, 0x800000, CRC(128256d1) SHA1(948af62545756165890e950fcd67bbd6304b3366) )
	ROM_LOAD16_BYTE( "242_hack03.c10", 0x4000001, 0x800000, CRC(4bd8412d) SHA1(72e9cc6e3df8000d0a7fc7a89257dd0ce56fc909) )
	ROM_LOAD16_BYTE( "242_hack04.c11", 0x5000000, 0x800000, CRC(e893277f) SHA1(a87db547821c63d162aac3ad178c8f794ef6a060) )
	ROM_LOAD16_BYTE( "242_hack04.c12", 0x5000001, 0x800000, CRC(f165589e) SHA1(6bdcb1dd9ce507fcae40d0aba42a2c1300987479) )
ROM_END

ROM_START( kof98hack05 ) //kof98cps
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack05.p1", 0x000000, 0x100000, CRC(003d1128) SHA1(36e744427563283069baff3d2220ef937821713d) )
	ROM_LOAD16_WORD_SWAP( "242_hack05.p2", 0x100000, 0x400000, CRC(d4ecdf60) SHA1(a3beb6f4c96f284ede10929e784a7f4c9250d656) )
	ROM_LOAD16_WORD_SWAP( "242_hack05.p3", 0x500000, 0x400000, CRC(4846dde0) SHA1(ad3d69389f897fcbb11efde24e397916a6f32243) )

	ROM_REGION( 0x040000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "242_hack05.p4", 0x000000, 0x040000, CRC(c076b84e) SHA1(9a2fb8acb511b82f5904a59b098e8e16d065409c) )

	NEO_SFIX_128K( "242_hack05.s1", CRC(b4ed8fb0) SHA1(59dde17cf8a1d1001a8bbb5dc1fc47d9886c5fd4) )

	NEO_BIOS_AUDIO_256K( "242_hack05.m1", CRC(564b53f8) SHA1(b8f3842d03f19f7af3077373db6410619380b579) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "242_hack03.v5", 0x000000, 0x400000, CRC(afdd9660) SHA1(0d67fb61111256c0d74d4f2b473ab5a42d1909b9) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack05.c1", 0x0000000, 0x800000, CRC(9cb35021) SHA1(28f6743acfe4d4d52111fbb4000bd88b012a0e42) )
	ROM_LOAD16_BYTE( "242_hack05.c2", 0x0000001, 0x800000, CRC(432a83ac) SHA1(de5d3545e1e12c1b9dc8541d1c3734be4a992e57) )
	ROM_LOAD16_BYTE( "242_hack05.c3",  0x1000000, 0x800000, CRC(ea84bdae) SHA1(27108247ed7406060f0b149a9dfa64869a6bbcf2) )
	ROM_LOAD16_BYTE( "242_hack05.c4",  0x1000001, 0x800000, CRC(2c17ac8e) SHA1(fc704f5cf358df81a88d20932be3edffe202dea9) )
	ROM_LOAD16_BYTE( "242_hack03.c5",  0x2000000, 0x800000, CRC(71641718) SHA1(b88a00aca2fc34230d2d2da0b235195a5eb1ecf0) )
	ROM_LOAD16_BYTE( "242_hack03.c6",  0x2000001, 0x800000, CRC(982ba2b3) SHA1(232ce3be7beaad13b35865da770157ef4b29a7a9) )
	ROM_LOAD16_BYTE( "242_hack03.c7",  0x3000000, 0x800000, CRC(8d495552) SHA1(20ff76b681b2e544c5a57060bb98ab6bb91bea3c) )
	ROM_LOAD16_BYTE( "242_hack03.c8",  0x3000001, 0x800000, CRC(8bfc3417) SHA1(38bb85563d0a7f008a64cf76d71d82b935cd98e5) )
	ROM_LOAD16_BYTE( "242_hack05.c9",  0x4000000, 0x800000, CRC(33a453ca) SHA1(3f9b740dbec21506133b1ac2e3ffa204279690f5) )
	ROM_LOAD16_BYTE( "242_hack05.c10", 0x4000001, 0x800000, CRC(0c85069f) SHA1(693f026832e8137530b231745b03ce297d8d1363) )
	ROM_LOAD16_BYTE( "242_hack03.c11", 0x5000000, 0x800000, CRC(56d361cb) SHA1(594826cf58b7cd3b0bee05cdab1265d03c057b58) )
	ROM_LOAD16_BYTE( "242_hack03.c12", 0x5000001, 0x800000, CRC(f8cb115b) SHA1(ba8a152a59acfcf72c73c6e29cd6e133630e100d) )
ROM_END

ROM_START( kof98hack06 ) //kof98pfe
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack06.p1", 0x000000, 0x100000, CRC(23876d95) SHA1(1e3bcd98d861618fa8b02503f1c8d342d6d45768) )
	ROM_LOAD16_WORD_SWAP( "242_hack06.p2", 0x100000, 0x400000, CRC(dfa686e9) SHA1(2cbcbb7907e934ee743fe4dbb40f70a1962b9eea) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "242_hack06.p3", 0x000000, 0x020000, CRC(930ea34e) SHA1(8eb58c20a6854a8feba454ef280147eb1319c0c5) )

	NEO_SFIX_128K( "242_hack06.s1", CRC(7f4dbf23) SHA1(bce6dcea6dc40d4072afe67682c7dacde2edce8d) )

	NEO_BIOS_AUDIO_256K( "242_hack01.m1", CRC(4e7a6b1b) SHA1(b54d08f88713ed0271aa06f9f7c9c572ef555b1a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack06.c1", 0x0000000, 0x800000, CRC(379654a5) SHA1(fe5d9f1d3072ac83224382abd7f371cf065a8366) )
	ROM_LOAD16_BYTE( "242_hack06.c2", 0x0000001, 0x800000, CRC(9c71fa3d) SHA1(1ccbab3378aeef5445fa73d6c59b93c6f9d65557) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242_hack06.c7", 0x3000000, 0x800000, CRC(02f09b2e) SHA1(f72246873e425f4b78c453f30b78eabc5a244fd3) )
	ROM_LOAD16_BYTE( "242_hack06.c8", 0x3000001, 0x800000, CRC(d43ab3e6) SHA1(90fbc49c687245fcde1b3e58289b3b0728dc6b0c) )
ROM_END

ROM_START( kof98hack07 ) //kof98ult
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack07.p1", 0x000000, 0x100000, CRC(9c450313) SHA1(447e4d422af94ededad84a382a5a762234a9c8ab) )
	ROM_LOAD16_WORD_SWAP( "242_hack07.p2", 0x100000, 0x600000, CRC(6d109dc7) SHA1(57cefc9383d0261fd6258ac5109683f3b0339d65) )

	NEO_SFIX( 0x40000, "242_hack07.s1", CRC(8fd2c84d) SHA1(298625444e41f4e3c8882a653a12fde6f7bd193d) )

	NEO_BIOS_AUDIO_256K( "242_hack03.m1", CRC(9ade0528) SHA1(67d0c3b146d369416b84c081544fe51fc6c2a140) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "242_hack03.v5", 0x000000, 0x400000, CRC(afdd9660) SHA1(0d67fb61111256c0d74d4f2b473ab5a42d1909b9) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack07.c1",  0x0000000, 0x800000, CRC(8e297a35) SHA1(8177d37dba3e1e3aea74d255d0a292d03c430c0b) )
	ROM_LOAD16_BYTE( "242_hack07.c2",  0x0000001, 0x800000, CRC(d8a4b216) SHA1(7a8aee14008ee9ccef0d76f710d6d4029098548d) )
	ROM_LOAD16_BYTE( "242.c3",     0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4",     0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242_hack03.c5",  0x2000000, 0x800000, CRC(71641718) SHA1(b88a00aca2fc34230d2d2da0b235195a5eb1ecf0) )
	ROM_LOAD16_BYTE( "242_hack03.c6",  0x2000001, 0x800000, CRC(982ba2b3) SHA1(232ce3be7beaad13b35865da770157ef4b29a7a9) )
	ROM_LOAD16_BYTE( "242_hack03.c7",  0x3000000, 0x800000, CRC(8d495552) SHA1(20ff76b681b2e544c5a57060bb98ab6bb91bea3c) )
	ROM_LOAD16_BYTE( "242_hack03.c8",  0x3000001, 0x800000, CRC(8bfc3417) SHA1(38bb85563d0a7f008a64cf76d71d82b935cd98e5) )
	ROM_LOAD16_BYTE( "242_hack07.c9",  0x4000000, 0x800000, CRC(de7e8582) SHA1(a257beff275dffc960eb5f2df3bcbf579be64a89) )
	ROM_LOAD16_BYTE( "242_hack07.c10", 0x4000001, 0x800000, CRC(76c7b038) SHA1(1a2fca5103749ada63876cbd661c3c6f2bf68bc5) )
	ROM_LOAD16_BYTE( "242_hack03.c11", 0x5000000, 0x800000, CRC(56d361cb) SHA1(594826cf58b7cd3b0bee05cdab1265d03c057b58) )
	ROM_LOAD16_BYTE( "242_hack03.c12", 0x5000001, 0x800000, CRC(f8cb115b) SHA1(ba8a152a59acfcf72c73c6e29cd6e133630e100d) )
ROM_END

ROM_START( kof98hack08 ) //kof98eck
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack08.p1", 0x000000, 0x100000, CRC(0ced4a93) SHA1(98a790af5edd0f57c9ab13a4c90d8711a502d662) )
	ROM_LOAD16_WORD_SWAP( "242_hack08.p2", 0x100000, 0x400000, CRC(ef36876a) SHA1(559ae66b30eca2cc419cc13dd571c5c56dd9ca69) )

	NEO_SFIX_128K( "242_hack08.s1", CRC(5a498ed2) SHA1(b6165341d873a27076a28762dd2b14037321c66c) )

	NEO_BIOS_AUDIO_256K( "242_hack01.m1", CRC(4e7a6b1b) SHA1(b54d08f88713ed0271aa06f9f7c9c572ef555b1a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242_hack08.v1", 0x000000, 0x400000, CRC(b5650f76) SHA1(8f88108deb8083a54cb489510f4c3ff511928cdd) )
	ROM_LOAD( "242_hack08.v2", 0x400000, 0x400000, CRC(b43b83ee) SHA1(4e912d94c02ac9d6d62c36d5d7b4ae2eb7398820) )
	ROM_LOAD( "242_hack08.v3", 0x800000, 0x400000, CRC(59c53922) SHA1(5b572a95a965aa8a25499124692bc71c106a46fb) )
	ROM_LOAD( "242_hack08.v4", 0xc00000, 0x400000, CRC(46ca6f5e) SHA1(412c1a0b6a4a3ff21a1d3a13e814adfb2981afbd) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack08.c1", 0x0000000, 0x800000, CRC(d528dab9) SHA1(ae4396472c1cf5f384f5fc3d9bfc8b59f2d5742f) )
	ROM_LOAD16_BYTE( "242_hack08.c2", 0x0000001, 0x800000, CRC(73556130) SHA1(3cb59940cba8c0a33d3497907144488d37f0311e) )
	ROM_LOAD16_BYTE( "242_hack08.c3", 0x1000000, 0x800000, CRC(4091bca0) SHA1(a4346ece9b1ad9a32599ad618e482cf13acba04d) )
	ROM_LOAD16_BYTE( "242_hack08.c4", 0x1000001, 0x800000, CRC(56918bab) SHA1(73c0e1614c164aea03c860db35f6027bb4ff4ce1) )
	ROM_LOAD16_BYTE( "242_hack08.c5", 0x2000000, 0x800000, CRC(76252bdc) SHA1(2b41cfd7c99d78818147e13eecbae9183e219556) )
	ROM_LOAD16_BYTE( "242_hack08.c6", 0x2000001, 0x800000, CRC(13d5b9b7) SHA1(5a1e9ff19e92f96bfdb0883d4521332c8fccfcf9) )
	ROM_LOAD16_BYTE( "242_hack08.c7", 0x3000000, 0x800000, CRC(1b3a6aa9) SHA1(5d50eaf58268788f77d42a047795773a4b2d9a9e) )
	ROM_LOAD16_BYTE( "242_hack08.c8", 0x3000001, 0x800000, CRC(ef3390ca) SHA1(4be429719db9612dc31b00320525868b6d3eeed6) )
ROM_END

ROM_START( kof98hack09 ) //kof98cb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack09.p1", 0x000000, 0x100000, CRC(d2077b89) SHA1(6ca89f31596621d6464960254668a809802643fa) )
	ROM_LOAD16_WORD_SWAP( "242_hack09.p2", 0x100000, 0x400000, CRC(d34a4d38) SHA1(7af1d73a6b82f7be847719b013f77a5692bd0d49) )

	NEO_SFIX_128K( "242_hack09.s1", CRC(8e37c589) SHA1(44b9f1cd06cd029fcc39bf29724b03e021974c20) )

	NEO_BIOS_AUDIO_256K( "242_hack09.m1", CRC(db046fc4) SHA1(4b793a125bade067ef0fdcd1775d135c646c6b94) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack09.c1",  0x0000000, 0x800000, CRC(066db0a6) SHA1(ea200b79bf60d2d9a025da873173cc4704d254b0) )
	ROM_LOAD16_BYTE( "242_hack09.c2",  0x0000001, 0x800000, CRC(99d0b0fa) SHA1(b56813917de0c19858c13e51888b6af2030bc324) )
	ROM_LOAD16_BYTE( "242_hack05.c3",  0x1000000, 0x800000, CRC(ea84bdae) SHA1(27108247ed7406060f0b149a9dfa64869a6bbcf2) )
	ROM_LOAD16_BYTE( "242_hack05.c4",  0x1000001, 0x800000, CRC(2c17ac8e) SHA1(fc704f5cf358df81a88d20932be3edffe202dea9) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof98hack10 ) //kof98cb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack10.p1", 0x000000, 0x100000, CRC(04d17f87) SHA1(a68300fd8b77bfe671a8f4dd5ee52004903d7037) )
	ROM_LOAD16_WORD_SWAP( "242_hack09.p2", 0x100000, 0x400000, CRC(d34a4d38) SHA1(7af1d73a6b82f7be847719b013f77a5692bd0d49) )

	NEO_SFIX_128K( "242_hack09.s1", CRC(8e37c589) SHA1(44b9f1cd06cd029fcc39bf29724b03e021974c20) )

	NEO_BIOS_AUDIO_256K( "242_hack09.m1", CRC(db046fc4) SHA1(4b793a125bade067ef0fdcd1775d135c646c6b94) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack09.c1",  0x0000000, 0x800000, CRC(066db0a6) SHA1(ea200b79bf60d2d9a025da873173cc4704d254b0) )
	ROM_LOAD16_BYTE( "242_hack09.c2",  0x0000001, 0x800000, CRC(99d0b0fa) SHA1(b56813917de0c19858c13e51888b6af2030bc324) )
	ROM_LOAD16_BYTE( "242_hack05.c3",  0x1000000, 0x800000, CRC(ea84bdae) SHA1(27108247ed7406060f0b149a9dfa64869a6bbcf2) )
	ROM_LOAD16_BYTE( "242_hack05.c4",  0x1000001, 0x800000, CRC(2c17ac8e) SHA1(fc704f5cf358df81a88d20932be3edffe202dea9) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof98hack11 ) //kof98cbc
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack11.p1", 0x000000, 0x100000, CRC(fc644aed) SHA1(d12febda348eea8ff3968a8a8b5a0af37c0bf5e3) )
	ROM_LOAD16_WORD_SWAP( "242_hack11.p2", 0x100000, 0x400000, CRC(485eb7a9) SHA1(87021f3c33c76d743486a2c6bcdc9276609d7265) )

	NEO_SFIX_128K( "242_hack05.s1", CRC(b4ed8fb0) SHA1(59dde17cf8a1d1001a8bbb5dc1fc47d9886c5fd4) )

	NEO_BIOS_AUDIO_256K( "242_hack01.m1", CRC(4e7a6b1b) SHA1(b54d08f88713ed0271aa06f9f7c9c572ef555b1a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242_hack09.c1",  0x0000000, 0x800000, CRC(066db0a6) SHA1(ea200b79bf60d2d9a025da873173cc4704d254b0) )
	ROM_LOAD16_BYTE( "242_hack09.c2",  0x0000001, 0x800000, CRC(99d0b0fa) SHA1(b56813917de0c19858c13e51888b6af2030bc324) )
	ROM_LOAD16_BYTE( "242_hack09.c3",  0x1000000, 0x800000, CRC(35800c82) SHA1(8dc25967a2c9d21d4f6f34a40507f8d9e7f14d9f) )
	ROM_LOAD16_BYTE( "242_hack09.c4",  0x1000001, 0x800000, CRC(499e6d69) SHA1(728bb3a505e22df096a05dd978fff7702928eb83) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242.c7", 0x3000000, 0x800000, CRC(f6d7a38a) SHA1(dd295d974dd4a7e5cb26a3ef3febcd03f28d522b) )
	ROM_LOAD16_BYTE( "242.c8", 0x3000001, 0x800000, CRC(c823e045) SHA1(886fbf64bcb58bc4eabb1fc9262f6ac9901a0f28) )
ROM_END

ROM_START( kof98hack12 ) //kof98cryu
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "242_hack12.p1",  0x000000, 0x100000, CRC(0d19e746) SHA1(b7b90580409f22af59dfa096ba42a1bd79ed61cb) )
	ROM_LOAD16_WORD_SWAP( "242_hack12.p2",  0x100000, 0x400000, CRC(33f7a7df) SHA1(1428154c0f28e6efb159e9ab7ec63828de517508) )
	ROM_FILL(0x30632,1,0x65)

	NEO_SFIX_128K( "242cos.s1", CRC(554c4b66) SHA1(0fa53c7433ed4b66515890ad0526f292ae88c7ad) )

	NEO_BIOS_AUDIO_256K( "242.m1", CRC(4ef7016b) SHA1(4182235e963bd70d398a79abeb54ab4d62887c48) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "242.v1", 0x000000, 0x400000, CRC(b9ea8051) SHA1(49606f64eb249263b3341b4f50cc1763c390b2af) )
	ROM_LOAD( "242.v2", 0x400000, 0x400000, CRC(cc11106e) SHA1(d3108bc05c9bf041d4236b2fa0c66b013aa8db1b) )
	ROM_LOAD( "242.v3", 0x800000, 0x400000, CRC(044ea4e1) SHA1(062a2f2e52098d73bc31c9ad66f5db8080395ce8) )
	ROM_LOAD( "242.v4", 0xc00000, 0x400000, CRC(7985ea30) SHA1(54ed5f0324de6164ea81943ebccb3e8d298368ec) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "242.c1", 0x0000000, 0x800000, CRC(e564ecd6) SHA1(78f22787a204f26bae9b2b1c945ddbc27143352f) )
	ROM_LOAD16_BYTE( "242.c2", 0x0000001, 0x800000, CRC(bd959b60) SHA1(2c97c59e77c9a3fe7d664e741d37944f3d56c10b) )
	ROM_LOAD16_BYTE( "242.c3", 0x1000000, 0x800000, CRC(22127b4f) SHA1(bd0d00f889d9da7c6ac48f287d9ed8c605ae22cf) )
	ROM_LOAD16_BYTE( "242.c4", 0x1000001, 0x800000, CRC(0b4fa044) SHA1(fa13c3764fae6b035a626601bc43629f1ebaaffd) )
	ROM_LOAD16_BYTE( "242.c5", 0x2000000, 0x800000, CRC(9d10bed3) SHA1(4d44addc7c808649bfb03ec45fb9529da413adff) )
	ROM_LOAD16_BYTE( "242.c6", 0x2000001, 0x800000, CRC(da07b6a2) SHA1(9c3f0da7cde1ffa8feca89efc88f07096e502acf) )
	ROM_LOAD16_BYTE( "242_hack12.c7",  0x3000000, 0x800000, CRC(960f0df4) SHA1(0bf8381d14be1fe16ffc122df0f49a425cfef9bc) )
	ROM_LOAD16_BYTE( "242_hack12.c8",  0x3000001, 0x800000, CRC(f1eab90b) SHA1(224f829bbc75d51f1152e16e60e58951c62d188c) )
ROM_END


 /************************
 The King of Fighters '99
****************************/

ROM_START( kof99hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack01.p1",  0x000000, 0x100000, CRC(eedcc5bd) SHA1(8280c4329d666b4831c766853f5334355dc40cc9) )
	ROM_LOAD16_WORD_SWAP( "251_hack01.p2", 0x100000, 0x400000, CRC(274ef47a) SHA1(98654b68cc85c19d4a90b46f3110f551fa2e5357) )

	NEO_SFIX_128K( "251_hack01.s1", CRC(b696a8c9) SHA1(3388feab11374a6eca6f1615fdfd93464f86dcee) )

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251_hack01.c1", 0x0000000, 0x800000, CRC(9864655e) SHA1(9c543ae156aaca186fa8da2443358573c9e673be) )
	ROM_LOAD16_BYTE( "251_hack01.c2", 0x0000001, 0x800000, CRC(4cd81e02) SHA1(f3b9202a4997c039277800cdef69800c8f499ac4) )
	ROM_LOAD16_BYTE( "251_hack01.c3", 0x1000000, 0x800000, CRC(3fc352cb) SHA1(5dcf47e8b48214dc32653de8bacbf65fb90912e3) )
	ROM_LOAD16_BYTE( "251_hack01.c4", 0x1000001, 0x800000, CRC(a7668796) SHA1(682c1df0ff8e3ce9cbee8af87a213b26cb4f8b27) )
	ROM_LOAD16_BYTE( "251_hack01.c5", 0x2000000, 0x800000, CRC(5e82dc3f) SHA1(c113744685aa0c63bda088ebe9e2f8fae6feab3b) )
	ROM_LOAD16_BYTE( "251_hack01.c6", 0x2000001, 0x800000, CRC(fa5419ae) SHA1(92c9aa9c800c4d906f01a66c633319267b5a769b) )
	ROM_LOAD16_BYTE( "251_hack01.c7", 0x3000000, 0x800000, CRC(3733fd9d) SHA1(78678ec0841bdcbb37c66274ad73bc6e69fc3530) )
	ROM_LOAD16_BYTE( "251_hack01.c8", 0x3000001, 0x800000, CRC(947dedbc) SHA1(186a75b8c43aa2733404a5763c8c90d5265ab751) )
ROM_END

ROM_START( kof99hack02 ) //kof99eur
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack02.p1", 0x000000, 0x100000, CRC(66fa20fd) SHA1(6fbda6db41558e4d7a073c7d5fc568f7cc57df2d) )
	ROM_LOAD16_WORD_SWAP( "251_hack02.p2", 0x100000, 0x400000, CRC(aec93904) SHA1(9270e1a920c6f99dc2b716fc1d3536ce7783cd74) )
	ROM_FILL(0x2abe8,1,0x65)

	NEO_SFIX_128K( "251_hack02.s1", CRC(a6747969) SHA1(6577c00aceb0d612819c703b3334ebb6c760e126) )

	NEO_BIOS_AUDIO_128K( "251_hack02.m1", CRC(f847e188) SHA1(0b98595a457292b04f518afcc82c2d6b8f249a7b) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251_hack02.v1",  0x000000, 0x400000, CRC(4ae46226) SHA1(77dbd53772e1e5000a091b9ff4b3cdc14b62ab0b) )
	ROM_LOAD( "251_hack02.v2",  0x400000, 0x400000, CRC(07d70650) SHA1(70d274771c07215268292f91517ec61634e32611) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251_hack02.c1", 0x0000000, 0x800000, CRC(c7e8bf32) SHA1(ede68ac10356f6824fa6b30e558038e78b604caf) )
	ROM_LOAD16_BYTE( "251_hack02.c2", 0x0000001, 0x800000, CRC(4e8f9f7d) SHA1(3a60df66bb24702041facb71025241483e5a1832) )
	ROM_LOAD16_BYTE( "251d.c3", 0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4", 0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5", 0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6", 0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack02.c7", 0x3000000, 0x800000, CRC(f0b5ff02) SHA1(b9a25e6dad48547acce8edb5623be9d35bff1b4c) )
	ROM_LOAD16_BYTE( "251_hack02.c8", 0x3000001, 0x800000, CRC(7107b8d5) SHA1(6754113de3eb6a34c1913bb5117dd12d705b961c) )
ROM_END

ROM_START( kof99hack03 ) // kof99rp2f
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack03.p1", 0x000000, 0x100000, CRC(1c5fd0ad) SHA1(78cfe664ca9b461587e5029ec34a0da228ba0788) )
	ROM_LOAD16_WORD_SWAP( "251_hack03.p2", 0x100000, 0x400000, CRC(01e111b3) SHA1(4ae2fbd552246779146684d2191a7bc9a3554c59) )

	NEO_SFIX_128K( "251_hack03.s1", CRC(ca942904) SHA1(f84b79282bb2de61d24faacca1feda0187e42dcd) )

	NEO_BIOS_AUDIO_128K( "251_hack03.m1", CRC(0cb24d46) SHA1(7e6f41c093e2cc7353592cfda53e855b5e03c4e3) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251_hack03.v3",  0x800000, 0x400000, CRC(403628da) SHA1(e9fddfbedfdb70ba345820d95b154c1c73101a1c) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251_hack03.c1", 0x0000000, 0x800000, CRC(f0409101) SHA1(419ca1ea525f76d92b562e30bb043d39bd9a6c41) )
	ROM_LOAD16_BYTE( "251_hack03.c2", 0x0000001, 0x800000, CRC(3a9eff54) SHA1(8eb1918e7d6f823c8965546cd064f44b1b6982f2) )
	ROM_LOAD16_BYTE( "251d.c3",  0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4",  0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251_hack03.c5", 0x2000000, 0x800000, CRC(dd023649) SHA1(e65c3e82a66553a4f87277984e630ec7e288626a) )
	ROM_LOAD16_BYTE( "251_hack03.c6", 0x2000001, 0x800000, CRC(b8f60a8c) SHA1(f6a31066ffbf87937701df80abed8d9603629a5d) )
	ROM_LOAD16_BYTE( "251_hack03.c7", 0x3000000, 0x800000, CRC(1dfd5103) SHA1(edbe56d9434abdff58e7e9ceed7ea3d7975c40d2) )
	ROM_LOAD16_BYTE( "251_hack03.c8", 0x3000001, 0x800000, CRC(23fe3c43) SHA1(b601a14cd023b28f8e8eed8afd6a05d2c3df115a) )
ROM_END

ROM_START( kof99hack04 ) //kof99maxp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack04.p1", 0x000000, 0x100000, CRC(b33712fd) SHA1(c0e1076c0bb34df003a979a89a0347c09900e3ef) )
	ROM_LOAD16_WORD_SWAP( "251_hack04.p2", 0x100000, 0x400000, CRC(3955b253) SHA1(993610003b23c0fcdcfa44f7537e9d31c5c9378e) )

	NEO_SFIX_128K( "251_hack04.s1", CRC(2c0a314a) SHA1(2ca439e6c2798864d9bade7753d512b64ed34f50) )

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251d.c1", 0x0000000, 0x800000, CRC(b3d88546) SHA1(c277525f3db5b4cb07e9842605c7c40e6c203ad9) )
	ROM_LOAD16_BYTE( "251d.c2", 0x0000001, 0x800000, CRC(915c8634) SHA1(685ecb4271edf61f6a28a2235de11dd219b999d6) )
	ROM_LOAD16_BYTE( "251d.c3", 0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4", 0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5", 0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6", 0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251d.c7", 0x3000000, 0x800000, CRC(2f68fdeb) SHA1(37167c84a39141c179f94800f207dac3aabc5478) )
	ROM_LOAD16_BYTE( "251d.c8", 0x3000001, 0x800000, CRC(4c2fad1e) SHA1(26779e79296eb1988a8c4d60d2e1baf041f2c0cf) )
ROM_END

ROM_START( kof99hack05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack05.p1", 0x000000, 0x100000, CRC(b96da0bf) SHA1(260b8fd7888b54f5f61fa07175dacd1743f1c6bb) )
	ROM_LOAD16_WORD_SWAP( "251_hack05.p2", 0x100000, 0x400000, CRC(9f1d1660) SHA1(d23466a5aac70c69d10c6bec59f019419940d914) )
	ROM_FILL(0x2abe8,1,0x65)

	NEO_SFIX_128K( "251_hack05.s1", CRC(c16fbccc) SHA1(ab019608617ba4868673e35836261c9557ac74f2) )
	ROM_IGNORE(0x0D01C0)

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251d.c1",  0x0000000, 0x800000, CRC(b3d88546) SHA1(c277525f3db5b4cb07e9842605c7c40e6c203ad9) )
	ROM_LOAD16_BYTE( "251d.c2",  0x0000001, 0x800000, CRC(915c8634) SHA1(685ecb4271edf61f6a28a2235de11dd219b999d6) )
	ROM_LOAD16_BYTE( "251d.c3",  0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4",  0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5",  0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6",  0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack05.c7",  0x3000000, 0x800000, CRC(65de862c) SHA1(83c0446f19ca567cd7664956f617f4f7625422b7) )
	ROM_LOAD16_BYTE( "251_hack05.c8",  0x3000001, 0x800000, CRC(3ff457fe) SHA1(1838de77ceaf78cb75604f227ffdf59311c0bbe5) )
ROM_END

ROM_START( kof99hack06 ) //kof99ae
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack06.p1", 0x000000, 0x100000, CRC(4c63e81c) SHA1(29bf0446db097e594c3705f7ff9ca20fa9d0d565) )
	ROM_LOAD16_WORD_SWAP( "251_hack06.p2", 0x100000, 0x400000, CRC(f3f47f41) SHA1(319ddb12c5353f4e27712db041c750f36df832b0) )
	ROM_LOAD16_WORD_SWAP( "251_hack06.p3", 0x500000, 0x400000, CRC(bc85cf02) SHA1(a1a8228299a76bf840822d50528a67d4d3c5ac5e) )

	NEO_SFIX_128K( "251_hack06.s1", CRC(3c31ee43) SHA1(f3bf265ad41037c2317702818136eb08e3ade3a2) )

	NEO_BIOS_AUDIO_128K( "251_hack02.m1", CRC(f847e188) SHA1(0b98595a457292b04f518afcc82c2d6b8f249a7b) )

	ROM_REGION( 0xe00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251_hack06.v1", 0x000000, 0x400000, CRC(ceaa3bae) SHA1(ec598f92e7d3b41f38448fee2dd2ee599a482f8f) )
	ROM_LOAD( "251_hack02.v2",  0x400000, 0x400000, CRC(07d70650) SHA1(70d274771c07215268292f91517ec61634e32611) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251_hack06.c1", 0x000000, 0x800000, CRC(7eabea6c) SHA1(246db26c521ec3b25ac20cdc96a3c0ababffa3c2) )
	ROM_LOAD16_BYTE( "251_hack06.c2", 0x000001, 0x800000, CRC(e5a5bc5c) SHA1(d131c4be01e97e55d2645c28defe6899fb3e0ab6) )
	ROM_LOAD16_BYTE( "251d.c3",    0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4",    0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5",    0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6",    0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack06.c7",  0x3000000, 0x800000, CRC(963c28ab) SHA1(35d987c115ca5785c04ebdc087667a9467c8661c) )
	ROM_LOAD16_BYTE( "251_hack06.c8",  0x3000001, 0x800000, CRC(43f1e92e) SHA1(70f4ece035999e80c4a39a908696369c67734121) )
	ROM_LOAD16_BYTE( "251_hack06.c9",  0x4000000, 0x800000, CRC(86a3550d) SHA1(ec74bcadb315a004297dc9e02c72efe5f2e2c5b0) )
	ROM_LOAD16_BYTE( "251_hack06.c10", 0x4000001, 0x800000, CRC(986bb897) SHA1(14572621122e97c3ae96a3b85069b9cde062ef19) )
	ROM_LOAD16_BYTE( "251_hack06.c11", 0x5000000, 0x800000, CRC(7fe785c2) SHA1(b49c3ab1ebf89090388c4757f24d1c02c730cc99) )
	ROM_LOAD16_BYTE( "251_hack06.c12", 0x5000001, 0x800000, CRC(a7541483) SHA1(3b3f0e67acfae1c3b6c5549ac350d855482f285f) )
ROM_END

ROM_START( kof99hack07 ) //kof99ndps
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack07.p1", 0x000000, 0x100000, CRC(09c70195) SHA1(e3db41b49700138ce7b9423438a2fc9cbe15e65e) )
	ROM_LOAD16_WORD_SWAP( "251_hack07.p2", 0x100000, 0x400000, CRC(d89a2677) SHA1(20cfaea7da0934286675ad0718e09474aa87b38e) )
	ROM_FILL(0x02abe8,1,0x65)

	NEO_SFIX_128K( "251sr.s1", CRC(08055c7a) SHA1(682e0084e62b547482929babe828027fdd64cde0) )

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251d.c1", 0x0000000, 0x800000, CRC(b3d88546) SHA1(c277525f3db5b4cb07e9842605c7c40e6c203ad9) )
	ROM_LOAD16_BYTE( "251d.c2", 0x0000001, 0x800000, CRC(915c8634) SHA1(685ecb4271edf61f6a28a2235de11dd219b999d6) )
	ROM_LOAD16_BYTE( "251d.c3", 0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4", 0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5", 0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6", 0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack07.c7", 0x3000000, 0x800000, CRC(96992b0c) SHA1(ef516e5c114f2adef0f3b8ec861bc25977e748c2) )
	ROM_LOAD16_BYTE( "251_hack07.c8", 0x3000001, 0x800000, CRC(a791c727) SHA1(d62e99683fea28ec405360e4379466b7d91386f8) )
ROM_END

ROM_START( kof99hack08 ) //kof99ndps
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack08.p1", 0x000000, 0x100000, CRC(afa3ef37) SHA1(5a275cb5de5703a7388a6805adf2ed7200b416f5) )
	ROM_LOAD16_WORD_SWAP( "251_hack07.p2", 0x100000, 0x400000, CRC(d89a2677) SHA1(20cfaea7da0934286675ad0718e09474aa87b38e) )
	ROM_FILL(0x02abe8,1,0x65)

	NEO_SFIX_128K( "251sr.s1", CRC(08055c7a) SHA1(682e0084e62b547482929babe828027fdd64cde0) )

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251d.c1", 0x0000000, 0x800000, CRC(b3d88546) SHA1(c277525f3db5b4cb07e9842605c7c40e6c203ad9) )
	ROM_LOAD16_BYTE( "251d.c2", 0x0000001, 0x800000, CRC(915c8634) SHA1(685ecb4271edf61f6a28a2235de11dd219b999d6) )
	ROM_LOAD16_BYTE( "251d.c3", 0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4", 0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5", 0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6", 0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack07.c7", 0x3000000, 0x800000, CRC(96992b0c) SHA1(ef516e5c114f2adef0f3b8ec861bc25977e748c2) )
	ROM_LOAD16_BYTE( "251_hack07.c8", 0x3000001, 0x800000, CRC(a791c727) SHA1(d62e99683fea28ec405360e4379466b7d91386f8) )
ROM_END

ROM_START( kof99hack09 ) //kof99evoru
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251_hack09.p1", 0x000000, 0x100000, CRC(211146a3) SHA1(e24ab9d56143afd701de21f11d0beceb7e4d87a1) )
	ROM_LOAD16_WORD_SWAP( "251_hack09.p2", 0x100000, 0x400000, CRC(f152908b) SHA1(b48a359dee478be540ae0f11c7d3ae9177d44908) )
	ROM_FILL(0x02abe8,1,0x65)

	NEO_SFIX_128K( "251_hack02.s1", CRC(a6747969) SHA1(6577c00aceb0d612819c703b3334ebb6c760e126) )

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251_hack02.v1",  0x000000, 0x400000, CRC(4ae46226) SHA1(77dbd53772e1e5000a091b9ff4b3cdc14b62ab0b) )
	ROM_LOAD( "251_hack02.v2",  0x400000, 0x400000, CRC(07d70650) SHA1(70d274771c07215268292f91517ec61634e32611) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251_hack02.c1", 0x0000000, 0x800000, CRC(c7e8bf32) SHA1(ede68ac10356f6824fa6b30e558038e78b604caf) )
	ROM_LOAD16_BYTE( "251_hack02.c2", 0x0000001, 0x800000, CRC(4e8f9f7d) SHA1(3a60df66bb24702041facb71025241483e5a1832) )
	ROM_LOAD16_BYTE( "251d.c3", 0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4", 0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5", 0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6", 0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251_hack09.c7", 0x3000000, 0x800000, CRC(7c55a99c) SHA1(d874a79b802860d4eee4a55bacd0bb6a034be625) )
	ROM_LOAD16_BYTE( "251_hack09.c8", 0x3000001, 0x800000, CRC(ee2fec70) SHA1(1a093c8e727b167062b985a0d33ee932fafc7c8a) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof99nd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "251b.p1",  0x000000, 0x100000, CRC(f2c7ddfa) SHA1(d592eecc53d442c55c2f26a6a721fdf2924d2a5b) )
	ROM_LOAD16_WORD_SWAP( "251r.p2", 0x100000, 0x400000, CRC(274ef47a) SHA1(98654b68cc85c19d4a90b46f3110f551fa2e5357) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "251.m1", CRC(5e74539c) SHA1(6f49a9343cbd026b2c6720ff3fa2e5b1f85e80da) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "251.v1", 0x000000, 0x400000, CRC(ef2eecc8) SHA1(8ed13b9db92dba3124bc5ba66e3e275885ece24a) )
	ROM_LOAD( "251.v2", 0x400000, 0x400000, CRC(73e211ca) SHA1(0e60fa64cab6255d9721e2b4bc22e3de64c874c5) )
	ROM_LOAD( "251.v3", 0x800000, 0x400000, CRC(821901da) SHA1(c6d4975bfaa19a62ed59126cadf2578c0a5c257f) )
	ROM_LOAD( "251.v4", 0xc00000, 0x200000, CRC(b49e6178) SHA1(dde6f76e958841e8c99b693e13ced9aa9ef316dc) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "251d.c1",  0x0000000, 0x800000, CRC(b3d88546) SHA1(c277525f3db5b4cb07e9842605c7c40e6c203ad9) )
	ROM_LOAD16_BYTE( "251d.c2",  0x0000001, 0x800000, CRC(915c8634) SHA1(685ecb4271edf61f6a28a2235de11dd219b999d6) )
	ROM_LOAD16_BYTE( "251d.c3",  0x1000000, 0x800000, CRC(b047c9d5) SHA1(b840eab2208e6c0a1db0cdb28df46ba07da2ddca) )
	ROM_LOAD16_BYTE( "251d.c4",  0x1000001, 0x800000, CRC(6bc8e4b1) SHA1(674cb8145aeada1683a70beb02ed4ea028f5bdf8) )
	ROM_LOAD16_BYTE( "251d.c5",  0x2000000, 0x800000, CRC(9746268c) SHA1(59d839f01f4827377a752679922bc7281099430d) )
	ROM_LOAD16_BYTE( "251d.c6",  0x2000001, 0x800000, CRC(238b3e71) SHA1(f929c942972f768e68a5a009a3d174d203029160) )
	ROM_LOAD16_BYTE( "251d.c7",  0x3000000, 0x800000, CRC(2f68fdeb) SHA1(37167c84a39141c179f94800f207dac3aabc5478) )
	ROM_LOAD16_BYTE( "251d.c8",  0x3000001, 0x800000, CRC(4c2fad1e) SHA1(26779e79296eb1988a8c4d60d2e1baf041f2c0cf) )
ROM_END

ROM_START( kof2000hack01 )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma.ev",   0x0c0000, 0x040000, CRC(329d80c9) SHA1(b1360c265a17a14b885dccfce67d14b3e5bc43a5) )
	ROM_LOAD16_WORD_SWAP( "257.p1", 0x100000, 0x400000, CRC(60947b4c) SHA1(5faa0a7ac7734d6c8e276589bd12dd574264647d) )
	ROM_LOAD16_WORD_SWAP( "257_hack01.p2", 0x500000, 0x400000, CRC(eea411cc) SHA1(7c249beffe1f30417540bae83508acdf2166731a) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_256K( "257.m1", CRC(4b749113) SHA1(2af2361146edd0ce3966614d90165a5c1afb8de4) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257.c1", 0x0000000, 0x800000, CRC(cef1cdfa) SHA1(6135080f3a6b4712b76cc217edcc58e72b55c2b9) )
	ROM_LOAD16_BYTE( "257.c2", 0x0000001, 0x800000, CRC(f7bf0003) SHA1(9f7b19a2100cf7d12867e742f440dd5277b4f895) )
	ROM_LOAD16_BYTE( "257.c3", 0x1000000, 0x800000, CRC(101e6560) SHA1(8073ae1139e215d1167f8d32c14079a46ce3ee1c) )
	ROM_LOAD16_BYTE( "257.c4", 0x1000001, 0x800000, CRC(bd2fc1b1) SHA1(da0006761923ad49b404a08d7a151193ee307a69) )
	ROM_LOAD16_BYTE( "257.c5", 0x2000000, 0x800000, CRC(89775412) SHA1(b221b30224bc4239f1b3c2d2fd1cd4fa84e3523c) )
	ROM_LOAD16_BYTE( "257.c6", 0x2000001, 0x800000, CRC(fa7200d5) SHA1(6f2b0d38af34e280d56a58955400e5c679906871) )
	ROM_LOAD16_BYTE( "257.c7", 0x3000000, 0x800000, CRC(7da11fe4) SHA1(065336cf166807acb6c8569d59d3bf37a19b0a42) )
	ROM_LOAD16_BYTE( "257.c8", 0x3000001, 0x800000, CRC(b1afa60b) SHA1(b916184f5cfe4121752270f4f65abf35d8eb0519) )
ROM_END

ROM_START( kof2000hack02 ) // kof2000otc
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257_hack02.p1", 0x000000, 0x100000, CRC(11dbf4dc) SHA1(cb55339922a40ffdb779189863e5e379db052ce4) )
	ROM_LOAD16_WORD_SWAP( "257_hack02.p2", 0x100000, 0x400000, CRC(e1095f50) SHA1(53c31c6b64b670314c2e774e1ed7afde0dd9c984) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257_hack02.m1", CRC(80ebc3aa) SHA1(1595314fd3dd9c68b8a653cda030916689546b88) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "257_hack02.v5", 0x000000, 0x400000, CRC(54df285f) SHA1(218ac90b2660bd66f86e78eb96bfb1477daf7eb4) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257_hack02.c7", 0x3000000, 0x800000, CRC(b2cd7102) SHA1(9c124c37ac680848b45a0b5a0c1f432995ad5c4d) )
	ROM_LOAD16_BYTE( "257_hack02.c8", 0x3000001, 0x800000, CRC(d11c3b66) SHA1(c79bb675aaa4dfbdee70275a4cd37a5b4eb0d0da) )
ROM_END

ROM_START( kof2000hack03 ) // kof2000otc
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257_hack03.p1", 0x000000, 0x100000, CRC(25561805) SHA1(600f52bd8859fa34b40079d623069a53366b37c4) )
	ROM_LOAD16_WORD_SWAP( "257_hack02.p2", 0x100000, 0x400000, CRC(e1095f50) SHA1(53c31c6b64b670314c2e774e1ed7afde0dd9c984) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257_hack02.m1", CRC(80ebc3aa) SHA1(1595314fd3dd9c68b8a653cda030916689546b88) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "257_hack02.v5", 0x000000, 0x400000, CRC(54df285f) SHA1(218ac90b2660bd66f86e78eb96bfb1477daf7eb4) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257_hack02.c7", 0x3000000, 0x800000, CRC(b2cd7102) SHA1(9c124c37ac680848b45a0b5a0c1f432995ad5c4d) )
	ROM_LOAD16_BYTE( "257_hack02.c8", 0x3000001, 0x800000, CRC(d11c3b66) SHA1(c79bb675aaa4dfbdee70275a4cd37a5b4eb0d0da) )
ROM_END

ROM_START( kof2000hack04 ) // kof2kps2, kof2000ps2 : cx,px confirmed
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257_hack04.p1", 0x000000, 0x100000, CRC(56941018) SHA1(92d6d131111726f8ab616bf9dd54c1d5cbaa5bd5) )
	ROM_LOAD16_WORD_SWAP( "257_hack04.p2", 0x100000, 0x400000, CRC(1669a5ad) SHA1(64169a059aafcd91262243f6f5625a3104410d5b) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257_hack04.m1", CRC(a082f086) SHA1(809ef4a533cf541b86786f455a7f283a5acecd82) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257_hack04.c7", 0x3000000, 0x800000, CRC(93c343ec) SHA1(b1a30c16f1bc2eee8c27e391b32dddecd979cc59) )
	ROM_LOAD16_BYTE( "257_hack04.c8", 0x3000001, 0x800000, CRC(ba92f698) SHA1(7a80c24472752f86130e7c963d14fed88b106bf6) )
ROM_END

ROM_START( kof2000hack05 ) //kof2000plus
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257_hack05.p1",  0x000000, 0x100000, CRC(3805c7ab) SHA1(3d50a90cf3e3b90679e7730c9accf2a474fbb7ed) )
	ROM_LOAD16_WORD_SWAP( "257_hack05.p2",  0x100000, 0x400000, CRC(7a6d80b8) SHA1(0a3bcf71a43b2b5dde0c33ed107c207a7c59e90c) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257d.m1", CRC(d404db70) SHA1(8cd1f3e140a9a367de23544e76371b0491287909) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257_hack05.c7", 0x3000000, 0x800000, CRC(110f72a3) SHA1(7540aa2be692fbbcd887c8377ca6891d1ba1b6ed) )
	ROM_LOAD16_BYTE( "257_hack05.c8", 0x3000001, 0x800000, CRC(4347f6af) SHA1(fc744d68e04905af1fa8d39d8772fba67e14777f) )
ROM_END

ROM_START( kof2000hack06 ) //kof2000sp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257_hack06.p1",  0x000000, 0x100000, CRC(dccee7c7) SHA1(07a33210501008f2843b7653a0e0e229d34c0ac7) )
	ROM_LOAD16_WORD_SWAP( "257_hack06.p2",  0x100000, 0x400000, CRC(48a1a381) SHA1(c32a7303c351422f66a3c0a535b01dbbdfca7d91) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257d.m1", CRC(d404db70) SHA1(8cd1f3e140a9a367de23544e76371b0491287909) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257_hack05.c7", 0x3000000, 0x800000, CRC(110f72a3) SHA1(7540aa2be692fbbcd887c8377ca6891d1ba1b6ed) )
	ROM_LOAD16_BYTE( "257_hack05.c8", 0x3000001, 0x800000, CRC(4347f6af) SHA1(fc744d68e04905af1fa8d39d8772fba67e14777f) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof2000d )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma",   0x0c0000, 0x040000, CRC(71c6e6bb) SHA1(1bd29ded4c6b29780db8e8b772c452189699ca89) )
	ROM_LOAD16_WORD_SWAP( "257.p1", 0x100000, 0x400000, CRC(60947b4c) SHA1(5faa0a7ac7734d6c8e276589bd12dd574264647d) )
	ROM_LOAD16_WORD_SWAP( "257.p2", 0x500000, 0x400000, CRC(1b7ec415) SHA1(f19fa44e9ee5b5a6eb4a051349d6bc4acc3bbbdb) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257d.m1", CRC(d404db70) SHA1(8cd1f3e140a9a367de23544e76371b0491287909) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257d.c7", 0x3000000, 0x800000, CRC(71dfc3e2) SHA1(1889a8dc88993e35f9fd93ce2bee1de52995932d) )
	ROM_LOAD16_BYTE( "257d.c8", 0x3000001, 0x800000, CRC(0fa30e5f) SHA1(0cb7fa6b0219e1af2df9b97786c677651a78f37a) )
ROM_END

ROM_START( kof2knd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "257n.p1",  0x000000, 0x100000, CRC(5f809dbe) SHA1(2bc233dcff5622de86d01e3b74b840c7caf12982) )
	ROM_LOAD16_WORD_SWAP( "257n.p2",  0x100000, 0x400000, CRC(693c2c5e) SHA1(dc9121b7369ef46596343cac055a00aec81704d4) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_256K( "257d.m1", CRC(d404db70) SHA1(8cd1f3e140a9a367de23544e76371b0491287909) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "257.v1", 0x000000, 0x400000, CRC(17cde847) SHA1(4bcc0205b70dc6d9216b29025450c9c5b08cb65d) )
	ROM_LOAD( "257.v2", 0x400000, 0x400000, CRC(1afb20ff) SHA1(57dfd2de058139345ff2b744a225790baaecd5a2) )
	ROM_LOAD( "257.v3", 0x800000, 0x400000, CRC(4605036a) SHA1(51b228a0600d38a6ec37aec4822879ec3b0ee106) )
	ROM_LOAD( "257.v4", 0xc00000, 0x400000, CRC(764bbd6b) SHA1(df23c09ca6cf7d0ae5e11ff16e30c159725106b3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "257d.c1", 0x0000000, 0x800000, CRC(abcdd424) SHA1(1d52aae8a7806d48c098c2a7a77dff6e02ac4870) )
	ROM_LOAD16_BYTE( "257d.c2", 0x0000001, 0x800000, CRC(cda33778) SHA1(a619740364c952c443f27ed9b7c395610f2673c7) )
	ROM_LOAD16_BYTE( "257d.c3", 0x1000000, 0x800000, CRC(087fb15b) SHA1(f77cb6e670cdf7709d84d770ecf28533cbfbe6de) )
	ROM_LOAD16_BYTE( "257d.c4", 0x1000001, 0x800000, CRC(fe9dfde4) SHA1(23750ff0c4bc084d55eea66a5cdd0ef2d6c32cdc) )
	ROM_LOAD16_BYTE( "257d.c5", 0x2000000, 0x800000, CRC(03ee4bf4) SHA1(8f26c5bc525a5786de8e25797e2875a1dfe527be) )
	ROM_LOAD16_BYTE( "257d.c6", 0x2000001, 0x800000, CRC(8599cc5b) SHA1(9a05fc12273aebfbc4ac22e88b32ae9ecd269462) )
	ROM_LOAD16_BYTE( "257d.c7", 0x3000000, 0x800000, CRC(71dfc3e2) SHA1(1889a8dc88993e35f9fd93ce2bee1de52995932d) )
	ROM_LOAD16_BYTE( "257d.c8", 0x3000001, 0x800000, CRC(0fa30e5f) SHA1(0cb7fa6b0219e1af2df9b97786c677651a78f37a) )
ROM_END

 /**************************
 The King of Fighters '2001
******************************/

ROM_START( kof2001hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262_hack01.p1",  0x000000, 0x100000, CRC(0f64d1a9) SHA1(2ad8c09050349e88ef3f7616a8e7edc831764b38) )
	ROM_LOAD16_WORD_SWAP( "262_hack01.p2", 0x100000, 0x400000, CRC(e489032a) SHA1(72f15340f7acef7aa823ac2f6100e304b9501eab) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_256K( "262.m1", CRC(a7f8119f) SHA1(71805b39b8b09c32425cf39f9de59b2f755976c2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262_hack01.c1", 0x0000000, 0x800000, CRC(9bafc9ea) SHA1(d38ebed0e978ae343fa5ed07446b36446dd8fec3) )
	ROM_LOAD16_BYTE( "262_hack01.c2", 0x0000001, 0x800000, CRC(a40f2b0f) SHA1(3a70b982e2a7bb1c5ceeb8cf0166979bfa38b791) )
	ROM_LOAD16_BYTE( "262_hack01.c3", 0x1000000, 0x800000, CRC(50572280) SHA1(6b63754ffb12460f710da5ea929c55d8584c6443) )
	ROM_LOAD16_BYTE( "262_hack01.c4", 0x1000001, 0x800000, CRC(8ddd733e) SHA1(850f1cefdc691985fd0935c7f02da0c3568d901a) )
	ROM_LOAD16_BYTE( "262_hack01.c5", 0x2000000, 0x800000, CRC(156db33a) SHA1(e14b981dc6451f8c9a37c75a542f8192d9873a7f) )
	ROM_LOAD16_BYTE( "262_hack01.c6", 0x2000001, 0x800000, CRC(b19c667a) SHA1(ef96abaa13ec002533b21a0bb033ee5c7020f4b3) )
	ROM_LOAD16_BYTE( "262_hack01.c7", 0x3000000, 0x800000, CRC(af7ff938) SHA1(b28e3d58b5ff78511e992b1e0cbb2421680678b2) )
	ROM_LOAD16_BYTE( "262_hack01.c8", 0x3000001, 0x800000, CRC(f4bae201) SHA1(a31e92fd0b19bef2fea1ef82515fc1127bd8ca44) )
ROM_END

ROM_START( kof2001hack02 ) //kf2k1pkz
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262_hack02.p1",  0x000000, 0x100000, CRC(c484272e) SHA1(6ceed676f2f5b1663215d22bf74a58cb0bbd42b2) )
	ROM_LOAD16_WORD_SWAP( "262_hack02.p2",  0x100000, 0x400000, CRC(91d4f9f7) SHA1(c14bb14d407210c8a132438c310222d01323df7a) )

	NEO_SFIX_128K( "262nd.s1", CRC(73efb81d) SHA1(9d294c5ecd658c2133a7d9f8c61c29715db33810) )

	NEO_BIOS_AUDIO_256K( "262n.m1", CRC(4bcc537b) SHA1(9fcf1342bcd53d5eec12c46ee41a51bf543256c2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262d.c1", 0x0000000, 0x800000, CRC(103225b1) SHA1(41486C7BB421B6B54F3CA07621AABD907BF10E15) )
	ROM_LOAD16_BYTE( "262d.c2", 0x0000001, 0x800000, CRC(f9d05d99) SHA1(C135DD3D5584DC58A46315D64F663E34BB64BEBF) )
	ROM_LOAD16_BYTE( "262d.c3", 0x1000000, 0x800000, CRC(4c7ec427) SHA1(0156E2F79E7A62B15ACC2314AC6563A67AF0F256) )
	ROM_LOAD16_BYTE( "262d.c4", 0x1000001, 0x800000, CRC(1d237aa6) SHA1(B007FE9F1F32F0FF947C6575741B47FB70976728) )
	ROM_LOAD16_BYTE( "262d.c5", 0x2000000, 0x800000, CRC(c2256db5) SHA1(DAE6B7B0673B431F223D82F7C3A685DE70A1C035) )
	ROM_LOAD16_BYTE( "262d.c6", 0x2000001, 0x800000, CRC(8d6565a9) SHA1(137C950D588D40C812C36967EC17D04D4FC56362) )
	ROM_LOAD16_BYTE( "262_hack02.c7",  0x3000000, 0x800000, CRC(fd3f6dcb) SHA1(d6e5f9fdb83beadf35a41ab9e74008682b43c25b) )
	ROM_LOAD16_BYTE( "262_hack02.c8",  0x3000001, 0x800000, CRC(3de3f84f) SHA1(03624ee89a8f42650407835713557de25b12c677) )
	ROM_LOAD16_BYTE( "262_hack02.c9",  0x4000000, 0x800000, CRC(54e0b19a) SHA1(75b0f8d59a8b160dfb1b025389c0cb3bd8a6ce6d) )
	ROM_LOAD16_BYTE( "262_hack02.c10", 0x4000001, 0x800000, CRC(35109b7d) SHA1(6152f4bfef59ec7f5cc40de29814266ee13fb7ad) )
ROM_END

ROM_START( kof2001hack03 ) //kf2k1pls
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262_hack03.p1", 0x000000, 0x100000, CRC(2c765ded) SHA1 (8b29cb6071c2ba90e872dbb49a5bbcbb20be4253) )
	ROM_LOAD16_WORD_SWAP( "262h.p2", 0x100000, 0x400000, CRC(91eea062) SHA1 (82bae42bbeedb9f3aa0c7c0b0a7a69be499cf98f) )

	NEO_SFIX_128K( "262pls.s1", CRC(088657e6) SHA1(7ab5eac3ff2a82e04fdc9dc8ee5d193db580e8d4) )

	NEO_BIOS_AUDIO_256K( "262n.m1", CRC(4bcc537b) SHA1(9fcf1342bcd53d5eec12c46ee41a51bf543256c2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262d.c1", 0x0000000, 0x800000, CRC(103225b1) SHA1(41486C7BB421B6B54F3CA07621AABD907BF10E15) )
	ROM_LOAD16_BYTE( "262d.c2", 0x0000001, 0x800000, CRC(f9d05d99) SHA1(C135DD3D5584DC58A46315D64F663E34BB64BEBF) )
	ROM_LOAD16_BYTE( "262d.c3", 0x1000000, 0x800000, CRC(4c7ec427) SHA1(0156E2F79E7A62B15ACC2314AC6563A67AF0F256) )
	ROM_LOAD16_BYTE( "262d.c4", 0x1000001, 0x800000, CRC(1d237aa6) SHA1(B007FE9F1F32F0FF947C6575741B47FB70976728) )
	ROM_LOAD16_BYTE( "262d.c5", 0x2000000, 0x800000, CRC(c2256db5) SHA1(DAE6B7B0673B431F223D82F7C3A685DE70A1C035) )
	ROM_LOAD16_BYTE( "262d.c6", 0x2000001, 0x800000, CRC(8d6565a9) SHA1(137C950D588D40C812C36967EC17D04D4FC56362) )
	ROM_LOAD16_BYTE( "262d.c7", 0x3000000, 0x800000, CRC(d1408776) SHA1(E77C786070B2B851A8A36250722B4C902E7213ED) )
	ROM_LOAD16_BYTE( "262d.c8", 0x3000001, 0x800000, CRC(954d0e16) SHA1(975803C130DF3A6E835B9BF0F8532D6586058C54) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof2001d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262.p1",  0x000000, 0x100000, CRC(9381750d) SHA1(dcfecd69e563ff52fe07d23c5372d0f748b07819) )
	ROM_LOAD16_WORD_SWAP( "262.p2", 0x100000, 0x400000, CRC(8e0d8329) SHA1(10dcc1baf0aaf1fc84c4d856bca6bcff85aed2bc) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "262d.m1",  CRC(2fb0a8a5) SHA1(9878370ff8cef3e9c9f307ad64c29522dd625c8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262d.c1", 0x0000000, 0x800000, CRC(103225b1) SHA1(41486C7BB421B6B54F3CA07621AABD907BF10E15) )
	ROM_LOAD16_BYTE( "262d.c2", 0x0000001, 0x800000, CRC(f9d05d99) SHA1(C135DD3D5584DC58A46315D64F663E34BB64BEBF) )
	ROM_LOAD16_BYTE( "262d.c3", 0x1000000, 0x800000, CRC(4c7ec427) SHA1(0156E2F79E7A62B15ACC2314AC6563A67AF0F256) )
	ROM_LOAD16_BYTE( "262d.c4", 0x1000001, 0x800000, CRC(1d237aa6) SHA1(B007FE9F1F32F0FF947C6575741B47FB70976728) )
	ROM_LOAD16_BYTE( "262d.c5", 0x2000000, 0x800000, CRC(c2256db5) SHA1(DAE6B7B0673B431F223D82F7C3A685DE70A1C035) )
	ROM_LOAD16_BYTE( "262d.c6", 0x2000001, 0x800000, CRC(8d6565a9) SHA1(137C950D588D40C812C36967EC17D04D4FC56362) )
	ROM_LOAD16_BYTE( "262d.c7", 0x3000000, 0x800000, CRC(d1408776) SHA1(E77C786070B2B851A8A36250722B4C902E7213ED) )
	ROM_LOAD16_BYTE( "262d.c8", 0x3000001, 0x800000, CRC(954d0e16) SHA1(975803C130DF3A6E835B9BF0F8532D6586058C54) )
ROM_END

ROM_START( kof2001n )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262n.p1", 0x000000, 0x100000, CRC(eaf3acf1) SHA1 (d697b9820d4832b944def118c30c82bbb1495b97) )
	ROM_LOAD16_WORD_SWAP( "262h.p2", 0x100000, 0x400000, CRC(91eea062) SHA1 (82bae42bbeedb9f3aa0c7c0b0a7a69be499cf98f) )

	NEO_SFIX_128K( "262n.s1", CRC(de828076) SHA1 (7f79d0245b02e64bd4b50c103722f7e115d1e711) )

	NEO_BIOS_AUDIO_256K( "262n.m1", CRC(4bcc537b) SHA1(9fcf1342bcd53d5eec12c46ee41a51bf543256c2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262d.c1", 0x0000000, 0x800000, CRC(103225b1) SHA1(41486C7BB421B6B54F3CA07621AABD907BF10E15) )
	ROM_LOAD16_BYTE( "262d.c2", 0x0000001, 0x800000, CRC(f9d05d99) SHA1(C135DD3D5584DC58A46315D64F663E34BB64BEBF) )
	ROM_LOAD16_BYTE( "262d.c3", 0x1000000, 0x800000, CRC(4c7ec427) SHA1(0156E2F79E7A62B15ACC2314AC6563A67AF0F256) )
	ROM_LOAD16_BYTE( "262d.c4", 0x1000001, 0x800000, CRC(1d237aa6) SHA1(B007FE9F1F32F0FF947C6575741B47FB70976728) )
	ROM_LOAD16_BYTE( "262d.c5", 0x2000000, 0x800000, CRC(c2256db5) SHA1(DAE6B7B0673B431F223D82F7C3A685DE70A1C035) )
	ROM_LOAD16_BYTE( "262d.c6", 0x2000001, 0x800000, CRC(8d6565a9) SHA1(137C950D588D40C812C36967EC17D04D4FC56362) )
	ROM_LOAD16_BYTE( "262d.c7", 0x3000000, 0x800000, CRC(d1408776) SHA1(E77C786070B2B851A8A36250722B4C902E7213ED) )
	ROM_LOAD16_BYTE( "262d.c8", 0x3000001, 0x800000, CRC(954d0e16) SHA1(975803C130DF3A6E835B9BF0F8532D6586058C54) )
ROM_END

ROM_START( kof2k1pls )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262pls.p1", 0x000000, 0x100000, CRC(758529a7) SHA1(cefdc7049772dcb77d658ac8571a12eedabda3c9) )
	ROM_LOAD16_WORD_SWAP( "262h.p2", 0x100000, 0x400000, CRC(91eea062) SHA1(82bae42bbeedb9f3aa0c7c0b0a7a69be499cf98f) )

	NEO_SFIX_128K( "262pls.s1", CRC(088657e6) SHA1(7ab5eac3ff2a82e04fdc9dc8ee5d193db580e8d4) )

	NEO_BIOS_AUDIO_128K( "262d.m1",  CRC(2fb0a8a5) SHA1(9878370ff8cef3e9c9f307ad64c29522dd625c8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262hc03.c1", 0x0000000, 0x800000, CRC(d750a13e) SHA1(8173cdfd3d43a7c4b2153952fd14d9cb1f2ff6b9) )
	ROM_LOAD16_BYTE( "262hc03.c2", 0x0000001, 0x800000, CRC(4310979a) SHA1(6eacac44266d8232f6fb253150bd4e0aae37e946) )
	ROM_LOAD16_BYTE( "262hc03.c3", 0x1000000, 0x800000, CRC(eed3f5cf) SHA1(c0d1e9ff8da6c948dbc4b2b8f084d73f1d7f640d) )
	ROM_LOAD16_BYTE( "262hc03.c4", 0x1000001, 0x800000, CRC(7966c2c7) SHA1(85ef98838b72366a876f3d8cd502abeb5dfb0b08) )
	ROM_LOAD16_BYTE( "262hc03.c5", 0x2000000, 0x800000, CRC(27cfda9f) SHA1(87c987d3e40f04f1c03db6800cede3cfb73068d4) )
	ROM_LOAD16_BYTE( "262hc03.c6", 0x2000001, 0x800000, CRC(99cd0921) SHA1(f7dffb83f3d91a00cb9c2342b0b9949c0b18cbb3) )
	ROM_LOAD16_BYTE( "262hc03.c7", 0x3000000, 0x800000, CRC(129d63ed) SHA1(e565f949c674017523f6e24d184edbe5e5f4de32) )
	ROM_LOAD16_BYTE( "262hc03.c8", 0x3000001, 0x800000, CRC(51a5b6c1) SHA1(aed4fd5037b7beb4ad0d6c2d56bfd4dce62ed604) )
ROM_END

ROM_START( kof2k1pa )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262pa.p1", 0x000000, 0x100000, CRC(f8a71b6f) SHA1(e4cc249b36b8cb72aa162adff4cdb302ce220812) )
	ROM_LOAD16_WORD_SWAP( "262h.p2", 0x100000, 0x400000, CRC(91eea062) SHA1(82bae42bbeedb9f3aa0c7c0b0a7a69be499cf98f) )

	NEO_SFIX_128K( "262pa.s1", CRC(50986eeb) SHA1(cdca34ba5afdd7b56cd5430b3df80da53de9b990) )

	NEO_BIOS_AUDIO_128K( "262d.m1",  CRC(2fb0a8a5) SHA1(9878370ff8cef3e9c9f307ad64c29522dd625c8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262hc02.c1", 0x0000000, 0x800000, CRC(72deb348) SHA1(810be830884f493859e7e754f1eda188a5b817e3) )
	ROM_LOAD16_BYTE( "262hc02.c2", 0x0000001, 0x800000, CRC(a3d834f5) SHA1(23c185a8606ae49ae617788b6d2a05f94174ce94) )
	ROM_LOAD16_BYTE( "262hc02.c3", 0x1000000, 0x800000, CRC(5d4a4348) SHA1(0ad7bce9c89115d55fdb153ad085d9c300c8e458) )
	ROM_LOAD16_BYTE( "262hc02.c4", 0x1000001, 0x800000, CRC(51909ad2) SHA1(95e4263dc27385d192750b0e3b3f0e6320f9d602) )
	ROM_LOAD16_BYTE( "262hc02.c5", 0x2000000, 0x800000, CRC(c8d48454) SHA1(5d4512eea02c1ca91fab2531881b2397278fd357) )
	ROM_LOAD16_BYTE( "262hc02.c6", 0x2000001, 0x800000, CRC(6b2c1877) SHA1(4cdd8f91e61326f82116600f702084ea1d24c348) )
	ROM_LOAD16_BYTE( "262hc02.c7", 0x3000000, 0x800000, CRC(357ad526) SHA1(6c39c5704b9a132d67e32a31fa8bd22170a74c40) )
	ROM_LOAD16_BYTE( "262hc02.c8", 0x3000001, 0x800000, CRC(b5ef2aa4) SHA1(5f94d85bb205726bcb49afaa9c1672845149ceb9) )
ROM_END

ROM_START( kof2k1nd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "262nd.p1", 0x000000, 0x100000, CRC(0121982f) SHA1(153662faa50da948ac3b08b897b6d4ce5bd247e0) )
	ROM_LOAD16_WORD_SWAP( "262nd.p2", 0x100000, 0x400000, CRC(f9ac401f) SHA1(5844c64b9b4bdf4fbff79aad3d3073b4e41d40b8) )

	NEO_SFIX_128K( "262nd.s1", CRC(73efb81d) SHA1(9d294c5ecd658c2133a7d9f8c61c29715db33810) )

	NEO_BIOS_AUDIO_256K( "262n.m1", CRC(4bcc537b) SHA1(9fcf1342bcd53d5eec12c46ee41a51bf543256c2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "262.v1", 0x000000, 0x400000, CRC(83d49ecf) SHA1(2f2c116e45397652e77fcf5d951fa5f71b639572) )
	ROM_LOAD( "262.v2", 0x400000, 0x400000, CRC(003f1843) SHA1(bdd58837ad542548bd4053c262f558af88e3b989) )
	ROM_LOAD( "262.v3", 0x800000, 0x400000, CRC(2ae38dbe) SHA1(4e82b7dd3b899d61907620517a5a27bdaba0725d) )
	ROM_LOAD( "262.v4", 0xc00000, 0x400000, CRC(26ec4dd9) SHA1(8bd68d95a2d913be41a51f51e48dbe3bff5924fb) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "262nd.c1",  0x0000000, 0x800000, CRC(f298b87b) SHA1(fbbcb51a74af006cfa66925e61b410f4e7f71246) )
	ROM_LOAD16_BYTE( "262d.c2",   0x0000001, 0x800000, CRC(f9d05d99) SHA1(C135DD3D5584DC58A46315D64F663E34BB64BEBF) )
	ROM_LOAD16_BYTE( "262d.c3",   0x1000000, 0x800000, CRC(4c7ec427) SHA1(0156E2F79E7A62B15ACC2314AC6563A67AF0F256) )
	ROM_LOAD16_BYTE( "262d.c4",   0x1000001, 0x800000, CRC(1d237aa6) SHA1(B007FE9F1F32F0FF947C6575741B47FB70976728) )
	ROM_LOAD16_BYTE( "262d.c5",   0x2000000, 0x800000, CRC(c2256db5) SHA1(DAE6B7B0673B431F223D82F7C3A685DE70A1C035) )
	ROM_LOAD16_BYTE( "262d.c6",   0x2000001, 0x800000, CRC(8d6565a9) SHA1(137C950D588D40C812C36967EC17D04D4FC56362) )
	ROM_LOAD16_BYTE( "262nd.c7",  0x3000000, 0x800000, CRC(b2b503ea) SHA1(2becf8a5462ff283fde76bec8137cdafe70cb7c6) )
	ROM_LOAD16_BYTE( "262nd.c8",  0x3000001, 0x800000, CRC(9c89c168) SHA1(ddc6a93c3ba766cfded63ee9355fa86835ead3b1) )
	ROM_LOAD16_BYTE( "262nd.c9",  0x4000000, 0x800000, CRC(773d08cc) SHA1(d38636baff48994f2c665874635c631be22ef440) )
	ROM_LOAD16_BYTE( "262nd.c10", 0x4000001, 0x800000, CRC(8101701d) SHA1(6a7fea6b1206efc51a37f9c39d4db79eb9a33d74) )
ROM_END

 /**************************
 The King of Fighters '2002
******************************/

ROM_START( kof2002hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack01.p1", 0x000000, 0x100000, CRC(536e002e) SHA1(553c7143b58929b4d536b32d9f7c643af26fd4fd) )
	ROM_LOAD16_WORD_SWAP( "265_hack01.p2", 0x100000, 0x400000, CRC(99f23ff4) SHA1(257edc07d71039ceb309af03754a2396bf7a6300) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "265.m1", CRC(85aaa632) SHA1(744fba4ca3bc3a5873838af886efb97a8a316104) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265_hack01.c1", 0x0000000, 0x800000, CRC(12778375) SHA1(e62e073b70fbe1eeacfb67ec8d930f7c819ad3b6) )
	ROM_LOAD16_BYTE( "265_hack01.c2", 0x0000001, 0x800000, CRC(49fc07a8) SHA1(1c2fdbb7fb374c84f0342d5430e9ffed052e9e92) )
	ROM_LOAD16_BYTE( "265_hack01.c3", 0x1000000, 0x800000, CRC(e23c81c7) SHA1(92ac7ad686bc1e5b564e63fd3fba09e68c0d21ab) )
	ROM_LOAD16_BYTE( "265_hack01.c4", 0x1000001, 0x800000, CRC(dfbbf29b) SHA1(94e65cb5e721f29d3978bda6783f552a43c85427) )
	ROM_LOAD16_BYTE( "265_hack01.c5", 0x2000000, 0x800000, CRC(d704a363) SHA1(995f90d5cc90b522c789cb961dafb93ac7b35a8b) )
	ROM_LOAD16_BYTE( "265_hack01.c6", 0x2000001, 0x800000, CRC(61abc03d) SHA1(7c45b4b35f3391f51994b61c43ac4b4599aeb350) )
	ROM_LOAD16_BYTE( "265_hack01.c7", 0x3000000, 0x800000, CRC(237629eb) SHA1(d44e90b7ede5b119e9c8487b2ffeee27ac23dc7f) )
	ROM_LOAD16_BYTE( "265_hack01.c8", 0x3000001, 0x800000, CRC(a70ab4be) SHA1(877dd294d50ec4b343f68e9317e3d19e4917b47a) )
ROM_END

ROM_START( kof2002hack02 ) // omega v9
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265.p1",  0x000000, 0x100000, CRC(9ede7323) SHA1(ad9d45498777fda9fa58e75781f48e09aee705a6) )
	ROM_LOAD16_WORD_SWAP( "265_hack02.p2", 0x100000, 0x400000, CRC(cd503fcf) SHA1(8f83fd34f441d706fabac6eee1357e27cb55d60b) )

	NEO_SFIX_128K( "265_hack02.s1", CRC(4562fb20) SHA1(623bc397f5fa9e2686ea098d16e8c11215bfd61d) )

	NEO_BIOS_AUDIO_128K( "265_hack02.m1", CRC(ab9d360e) SHA1(a0c8a5aae387c4f0b72790211695da7df924c351) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265nu.v1", 0x000000, 0x400000, CRC(13d98607) SHA1(0f1a374247992d301bc26c0bab200631a13a9f4a) )
	ROM_LOAD( "265nu.v2", 0x400000, 0x400000, CRC(9cf74677) SHA1(073e7cb00127690fdec05c19f00347ec449f15ac) )
	ROM_LOAD( "265nu.v3", 0x800000, 0x400000, CRC(8e9448b5) SHA1(c22420649c7c68a172290548cab846345c861cb0) )
	ROM_LOAD( "265nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c7",     0x0000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )  // to fix KOF2002 logo
	ROM_LOAD16_BYTE( "265d.c8",     0x0000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )  // to fix KOF2002 logo
	ROM_LOAD16_BYTE( "265_hack02.c3", 0x1000000, 0x800000, CRC(6898f6e2) SHA1(4d8101534f85838eff3823ead72bdbcee57a3a50) )
	ROM_LOAD16_BYTE( "265_hack02.c4", 0x1000001, 0x800000, CRC(9232e491) SHA1(e56b5657842d5e147e56c485dfa9a5a0a6ffba2d) )
	ROM_LOAD16_BYTE( "265_hack02.c5", 0x2000000, 0x800000, CRC(11126545) SHA1(4ae58848c8a6701c9cfcd60e6674e8fb7491c0c0) )
	ROM_LOAD16_BYTE( "265_hack02.c6", 0x2000001, 0x800000, CRC(d225bb9b) SHA1(ee627dab6aeebfb1af9dbdc315a927245243817c) )
	ROM_LOAD16_BYTE( "265_hack02.c7", 0x3000000, 0x800000, CRC(6954ef57) SHA1(807f7bcdac379c5dce7745f10925e5c4a10c2c85) )
	ROM_LOAD16_BYTE( "265_hack02.c8", 0x3000001, 0x800000, CRC(3e394883) SHA1(03e92267bdef8651bac6278c8068892dadba3ad8) )
	ROM_COPY("sprites", 0xc6ed00, 0x3c6ed00, 0x3e00)
	ROM_LOAD16_BYTE( "265_hack02.c1", 0x0000000, 0x800000, CRC(c8314ae2) SHA1(7d89b556de98e0d89448e5118ffeea4239461635) )
	ROM_LOAD16_BYTE( "265_hack02.c2", 0x0000001, 0x800000, CRC(e0ddced5) SHA1(d8b5bd419bc825b99c206f54fbce97305839bcc9) )
ROM_END

ROM_START( kof2002hack03 ) //kf2k2ps2re1
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack03.p1",  0x000000, 0x100000, CRC(77a0044c) SHA1(57665dcc803d6bf406fb047823aaf71348996b2b) )
	ROM_LOAD16_WORD_SWAP( "265_hack03.p2",  0x100000, 0x500000, CRC(f9e4456a) SHA1(95e8ebdb7fd0db8ce3116091d160ad1260c2a5e4) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "265_hack03.p3",  0x000000, 0x020000, CRC(6e6beeba) SHA1(9ab1687c9d8aacaa2626d25b8177b6ae48828674) )

	NEO_SFIX_128K( "265_hack03.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x30000, "audiocpu_m2", 0 )
	ROM_LOAD( "214-m1.m1", 0x00000, 0x20000, CRC(dabc427c) SHA1(b76722ed142ee7addceb4757424870dbd003e8b3) )
	ROM_RELOAD(     0x10000, 0x20000 )

	ROM_REGION( 0x50000, "audiocpu_m3", 0 )
	ROM_LOAD( "262-m1d.m1", 0x00000, 0x40000, CRC(4BCC537B) SHA1(9FCF1342BCD53D5EEC12C46EE41A51BF543256C2) )
	ROM_RELOAD(     0x10000, 0x40000 )

	ROM_REGION( 0x90000, "audiocpu_m4", 0 )
	ROM_LOAD( "269-m1d.m1", 0x00000, 0x80000, CRC(7B7BF462) SHA1(7466A6962DE5242F71B9C52D7BD21A9832115E11) )
	ROM_RELOAD(     0x10000, 0x80000 )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0FC9A58D) SHA1(9D79EF00E2C2ABD9F29AF5521C2FBE5798BF336F) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(B8C475A4) SHA1(10CAF9C69927A223445D2C4B147864C02CE520A8) )
	ROM_REGION( 0x1000000, "ymsnd_m2", 0 )
	ROM_LOAD( "214-v1.v1", 0x000000, 0x400000, CRC(63f7b045) SHA1(1353715f1a8476dca6f8031d9e7a401eacab8159) )
	ROM_LOAD( "214-v2.v2", 0x400000, 0x400000, CRC(25929059) SHA1(6a721c4cb8f8dc772774023877d4a9f50d5a9e31) )
	ROM_LOAD( "214-v3.v3", 0x800000, 0x200000, CRC(92a2257d) SHA1(5064aec78fa0d104e5dd5869b95382aa170214ee) )
	ROM_REGION( 0x1000000, "ymsnd_m3", 0 )
	ROM_LOAD( "262-v1d.v1", 0x000000, 0x800000, CRC(AC2913BF) SHA1(1721EC3D19684AF702F6C93DA25BB787A5D9DBFF) )
	ROM_LOAD( "262-v2d.v2", 0x800000, 0x800000, CRC(15042F30) SHA1(F92E49110BDE007104590BE1A0FDC8064C216C37) )
	ROM_REGION( 0x1000000, "ymsnd_m4", 0 )
	ROM_LOAD( "269-v1d.v1", 0x000000, 0x800000, CRC(FF64CD56) SHA1(E2754C554ED5CA14C2020C5D931021D5AC82660C) )
	ROM_LOAD( "269-v2d.v2", 0x800000, 0x800000, CRC(A8DD6446) SHA1(8972AAB271C33F8AF344BFFE6359D9DDC4B8AF2E) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265_hack03.c7", 0x3000000, 0x800000, CRC(F0897B93) SHA1(F1C38737B148C459212B61066E8C279852F080B3) )
	ROM_LOAD16_BYTE( "265_hack03.c8", 0x3000001, 0x800000, CRC(8D27A4A6) SHA1(F05A1FEA1B2E542B70B11E58455812E9186D0D77) )
	ROM_LOAD16_BYTE( "265_hack03.c9", 0x4000000, 0x800000, CRC(9939C08A) SHA1(75033A18ECD9177EC6DA00E32FE1E391FDE0BC39) )
	ROM_LOAD16_BYTE( "265_hack03.c10",0x4000001, 0x800000, CRC(C724C069) SHA1(26974478ED31C68A3B987986849C8386F8C5AF8C) )
ROM_END

ROM_START( kof2002hack04 ) //kof02ps2z
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack04.p1",  0x000000, 0x100000, CRC(9da95b36) SHA1(367910e5d69e0ed5e0b8c17aa3b8b93a1bf15cae) )
	ROM_LOAD16_WORD_SWAP( "265_hack04.p2",  0x100000, 0x500000, CRC(9846db3a) SHA1(be27f2f7dea6d74710ebfccf44689b5534356369) )

	NEO_SFIX_128K( "265_hack03.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265nu.v1", 0x000000, 0x400000, CRC(13d98607) SHA1(0f1a374247992d301bc26c0bab200631a13a9f4a) )
	ROM_LOAD( "265nu.v2", 0x400000, 0x400000, CRC(9cf74677) SHA1(073e7cb00127690fdec05c19f00347ec449f15ac) )
	ROM_LOAD( "265nu.v3", 0x800000, 0x400000, CRC(8e9448b5) SHA1(c22420649c7c68a172290548cab846345c861cb0) )
	ROM_LOAD( "265nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265_hack03.c7", 0x3000000, 0x800000, CRC(F0897B93) SHA1(F1C38737B148C459212B61066E8C279852F080B3) )
	ROM_LOAD16_BYTE( "265_hack03.c8", 0x3000001, 0x800000, CRC(8D27A4A6) SHA1(F05A1FEA1B2E542B70B11E58455812E9186D0D77) )
	ROM_LOAD16_BYTE( "265_hack03.c9", 0x4000000, 0x800000, CRC(9939C08A) SHA1(75033A18ECD9177EC6DA00E32FE1E391FDE0BC39) )
	ROM_LOAD16_BYTE( "265_hack03.c10",0x4000001, 0x800000, CRC(C724C069) SHA1(26974478ED31C68A3B987986849C8386F8C5AF8C) )
ROM_END

ROM_START( kof2002hack05 ) //kof02ps2z
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack05.p1",  0x000000, 0x100000, CRC(86814195) SHA1(982c7579dbbc5341e0d2654f66bbcfe8edeb11e0) )
	ROM_LOAD16_WORD_SWAP( "265_hack04.p2",  0x100000, 0x500000, CRC(9846db3a) SHA1(be27f2f7dea6d74710ebfccf44689b5534356369) )

	NEO_SFIX_128K( "265_hack03.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265nu.v1", 0x000000, 0x400000, CRC(13d98607) SHA1(0f1a374247992d301bc26c0bab200631a13a9f4a) )
	ROM_LOAD( "265nu.v2", 0x400000, 0x400000, CRC(9cf74677) SHA1(073e7cb00127690fdec05c19f00347ec449f15ac) )
	ROM_LOAD( "265nu.v3", 0x800000, 0x400000, CRC(8e9448b5) SHA1(c22420649c7c68a172290548cab846345c861cb0) )
	ROM_LOAD( "265nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) )

	ROM_REGION( 0x5000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265_hack03.c7", 0x3000000, 0x800000, CRC(F0897B93) SHA1(F1C38737B148C459212B61066E8C279852F080B3) )
	ROM_LOAD16_BYTE( "265_hack03.c8", 0x3000001, 0x800000, CRC(8D27A4A6) SHA1(F05A1FEA1B2E542B70B11E58455812E9186D0D77) )
	ROM_LOAD16_BYTE( "265_hack03.c9", 0x4000000, 0x800000, CRC(9939C08A) SHA1(75033A18ECD9177EC6DA00E32FE1E391FDE0BC39) )
	ROM_LOAD16_BYTE( "265_hack03.c10",0x4000001, 0x800000, CRC(C724C069) SHA1(26974478ED31C68A3B987986849C8386F8C5AF8C) )
ROM_END

ROM_START( kof2002hack06 ) //kof2k2plus
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack06.p1",  0x000000, 0x100000, CRC(059da6f9) SHA1(f581d018cac7e25a89f16c8c14d629f7f766dfee) )
	ROM_LOAD16_WORD_SWAP( "265_hack06.p2",  0x100000, 0x500000, CRC(7ad26451) SHA1(2a594d91a73bd857505934eb3fbad35310a1e55a) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "265_hack06.p3",  0x000000, 0x020000, CRC(80aacc1f) SHA1(f4e0943ecfb708f7bb7599245c2c17f1eb37254c) )

	NEO_SFIX_128K( "265_hack06.s1", CRC(96bdd036) SHA1(62baba893e10dbed5c5099040b07432c0737be42) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "265.m1", CRC(85aaa632) SHA1(744fba4ca3bc3a5873838af886efb97a8a316104) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265_hack06.c1", 0x0000000, 0x800000, CRC(93cf6345) SHA1(1e4194556082ed29bab45a5ccf42022c9c9515b0) )
	ROM_LOAD16_BYTE( "265_hack06.c2", 0x0000001, 0x800000, CRC(d7373d66) SHA1(ed1040e2433480c89ffbae4dc854984cc734e15d) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

ROM_START( kof2002hack07 ) //kof2k2c2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack07.p1", 0x000000, 0x100000, CRC(de02918d) SHA1(476abd71dd74178c2dcb9d1857b1053d339a586b) )
	ROM_LOAD16_WORD_SWAP( "265_hack07.p2", 0x100000, 0x400000, CRC(293bbf78) SHA1(647eea522d0a163675205aeab9b0b9ebb3a47eea) )

	NEO_SFIX_128K( "265_hack07.s1", CRC(67e7cbe1) SHA1(3582c2073f04e71a6d5a34145727d148a11ab375) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1C661A4B)  SHA1(4E5AA862A0A182A806D538996DDC68D9F2DFFAF7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265nu.v1", 0x000000, 0x400000, CRC(13d98607) SHA1(0f1a374247992d301bc26c0bab200631a13a9f4a) )
	ROM_LOAD( "265nu.v2", 0x400000, 0x400000, CRC(9cf74677) SHA1(073e7cb00127690fdec05c19f00347ec449f15ac) )
	ROM_LOAD( "265nu.v3", 0x800000, 0x400000, CRC(8e9448b5) SHA1(c22420649c7c68a172290548cab846345c861cb0) )
	ROM_LOAD( "265nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265_hack07.c4", 0x1000001, 0x800000, CRC(2558972F) SHA1(2e0627fc5d689bf64fe36f02a489408d103c4499) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265_hack07.c7", 0x3000000, 0x800000, CRC(201e75e0) SHA1(7805d7a753df4d97bd73e05f22f31fe6440eaf6c) )
	ROM_LOAD16_BYTE( "265_hack07.c8", 0x3000001, 0x800000, CRC(ff0fd53b) SHA1(68289ab3f808b9c067a948948f48d5a3f1787e6c) )
ROM_END

ROM_START( kof2002hack08 ) //kf2k2js2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack08.p1", 0x000000, 0x100000, CRC(8da72d5c) SHA1(315d104da120fe1df041221209dc609a2f1d41a8) )
	ROM_LOAD16_WORD_SWAP( "265_hack08.p2", 0x100000, 0x400000, CRC(75185760) SHA1(5501d4d9ac7fcd83102f6fbee7d986006566bf9d) )

	NEO_SFIX_128K( "265_hack08.s1", CRC(BD19C308) SHA1(38a9055bbd981a794e1fd9065985b4c033b78e93) )

	NEO_BIOS_AUDIO_128K( "265_hack08.m1", CRC(9956CCD8) SHA1(ed0f5e87360dfa89a58e1b333f4a66e91c2faf56) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265_hack08.v1", 0x000000, 0x800000, CRC(6e7e3508) SHA1(9d995fd9e9665da26153704a9fa539259759c105) )
	ROM_LOAD( "265_hack08.v2", 0x800000, 0x800000, CRC(2157b90f) SHA1(407422df7a24530d21119ef9233b831fb0166661) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1",    0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2",    0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265_hack08.c3",   0x1000000, 0x800000, CRC(ca656090) SHA1(ed7f79eeabb965f5cdfbdd1c2d137ef5db366e03) )
	ROM_LOAD16_BYTE( "265_hack08.c4",   0x1000001, 0x800000, CRC(eb898849) SHA1(eac79b04382fc20dd9f7b9345d929aff58889e8f) )
	ROM_LOAD16_BYTE( "265d.c5",    0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6",    0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265_hack08.c7", 0x3000000, 0x800000, CRC(2A9D505C) SHA1(dcd3899aebc4602e4249b3e61299a0c8bf2e5e56) )
	ROM_LOAD16_BYTE( "265_hack08.c8", 0x3000001, 0x800000, CRC(93D8501F) SHA1(470f29de0bad8c15d924fffbbfa85926b551a8ae) )
ROM_END

ROM_START( kof2002hack09 ) //kofallmix
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack09.p1",  0x000000, 0x100000, CRC(945c70cb) SHA1(7cbfae9d6fbe9b70b9105563c7617d36f10196fc) )
	ROM_LOAD16_WORD_SWAP( "265_hack09.p2",  0x100000, 0x800000, CRC(9daf65b8) SHA1(8426ad697494193f54f2d3b050c3cc6113eed0d8) )

	NEO_SFIX_128K( "265_hack03.s1", CRC(714ade47) SHA1(a46115ed89454d8090fae59cfa4aea61a4a81ebf) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1c661a4b) SHA1(4e5aa862a0a182a806d538996ddc68d9f2dffaf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265nu.v1", 0x000000, 0x400000, CRC(13d98607) SHA1(0f1a374247992d301bc26c0bab200631a13a9f4a) )
	ROM_LOAD( "265nu.v2", 0x400000, 0x400000, CRC(9cf74677) SHA1(073e7cb00127690fdec05c19f00347ec449f15ac) )
	ROM_LOAD( "265nu.v3", 0x800000, 0x400000, CRC(8e9448b5) SHA1(c22420649c7c68a172290548cab846345c861cb0) )
	ROM_LOAD( "265nu.v4", 0xc00000, 0x400000, CRC(067271b5) SHA1(36e07da78aaf634824c98023053bef802be4e218) )

	ROM_REGION( 0x8000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265_hack09.c1",  0x0000000, 0x800000, BAD_DUMP CRC(7f4e7f53) SHA1(00ea5486846751feb967128fbcc3b72847c42e99) )
	ROM_LOAD16_BYTE( "265_hack09.c2",  0x0000001, 0x800000, BAD_DUMP CRC(fbb6e56f) SHA1(f5b6c75acbe69ef3b326a33b0b467c85b6a23369) )
	ROM_LOAD16_BYTE( "265_hack09.c3",  0x1000000, 0x800000, CRC(c17e12d8) SHA1(0ddb8ade8265dd254e893e2beedbe7c0604cce6b) )
	ROM_LOAD16_BYTE( "265_hack09.c4",  0x1000001, 0x800000, CRC(5b9dd220) SHA1(a3f8793cb2fc1ae9eb4848dc1ef1a939b90c4ee1) )
	ROM_LOAD16_BYTE( "265_hack09.c5",  0x2000000, 0x800000, CRC(92701baf) SHA1(b592760d604aabf6a043b277b0f21893bac64d9f) )
	ROM_LOAD16_BYTE( "265_hack09.c6",  0x2000001, 0x800000, CRC(a54a31c4) SHA1(c4711813066def032674d8f779772971b5fc88b7) )
	ROM_LOAD16_BYTE( "265_hack09.c7",  0x3000000, 0x800000, BAD_DUMP CRC(e14b86d9) SHA1(decbbcde2d12179a648c51f5d94c18cb3c3a6fdd) ) // bad dump
	ROM_LOAD16_BYTE( "265_hack09.c8",  0x3000001, 0x800000, BAD_DUMP CRC(1d0f6bd8) SHA1(ba4d0c269120acdbf0e13cdcb0d9fa99c3483fc6) ) // bad dump
	ROM_LOAD16_BYTE( "265_hack09.c9",  0x4000000, 0x800000, CRC(4c5f9a30) SHA1(ce2f8338b37c9bca73ab933d374bdd45784d680b) )
	ROM_LOAD16_BYTE( "265_hack09.c10", 0x4000001, 0x800000, CRC(3ee65411) SHA1(55c2b57e9ee3760ee871934c7a118d001b95111f) )
	ROM_LOAD16_BYTE( "265_hack09.c11", 0x5000000, 0x800000, CRC(e4aad9a3) SHA1(749c68eebee7dc073e3dcbf453db7a2f0a78b1a6) )
	ROM_LOAD16_BYTE( "265_hack09.c12", 0x5000001, 0x800000, CRC(55761088) SHA1(295ab138bedd18fc341c7747998bb55e69cba8c7) )
	ROM_LOAD16_BYTE( "265_hack09.c13", 0x6000000, 0x800000, CRC(d5c7149a) SHA1(68eb718b1dc3f754191099ebeaf43f1c141be1e7) )
	ROM_LOAD16_BYTE( "265_hack09.c14", 0x6000001, 0x800000, CRC(c6a64bf3) SHA1(5fc0de29a13c38d44f0e6cda94fc31e3a598ecf5) )
	ROM_LOAD16_BYTE( "265_hack09.c15", 0x7000000, 0x800000, CRC(867ac5b3) SHA1(e508492f9056f045b378bb2136c0df9fc4e710b8) )
	ROM_LOAD16_BYTE( "265_hack09.c16", 0x7000001, 0x800000, CRC(8d7d8b38) SHA1(3f0bd5cbcee0173df81396c97f01315caf1378b2) ) // not used
	// use the standard decrypted ones to fix the bad gfx
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

ROM_START( kof2002hack10 ) // KOF2k2plus2017
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265_hack10.p1", 0x000000, 0x100000, CRC(bd94702d) SHA1(85f1c0930ebf160eeb0995c00eab9bfd896b87e3) )
	ROM_LOAD16_WORD_SWAP( "265_hack10.p2", 0x100000, 0x500000, CRC(76e75315) SHA1(f95cc585676a3d2d49b4249fea3872fd7f4af5ef) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "265_hack10.p3", 0x000000, 0x020000, CRC(6bfe80b0) SHA1(2ea3e2ed1bf5e20c256a41dd5c1160e945fa333e) )

	NEO_SFIX_128K( "265_hack06.s1", CRC(96bdd036) SHA1(62baba893e10dbed5c5099040b07432c0737be42) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "265.m1", CRC(85aaa632) SHA1(744fba4ca3bc3a5873838af886efb97a8a316104) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265_hack06.c1", 0x0000000, 0x800000, CRC(93cf6345) SHA1(1e4194556082ed29bab45a5ccf42022c9c9515b0) )
	ROM_LOAD16_BYTE( "265_hack06.c2", 0x0000001, 0x800000, CRC(d7373d66) SHA1(ed1040e2433480c89ffbae4dc854984cc734e15d) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof2k2pls )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "2k2-p1p.bin",0x000000, 0x100000, CRC(3ab03781) SHA1(86946c19f1c4d9ab5cde86688d698bf63118a39d) )
	ROM_LOAD16_WORD_SWAP( "265.p2", 0x100000, 0x400000, CRC(327266b8) SHA1(98f445cc0a94f8744d74bca71cb420277622b034) )

	NEO_SFIX_128K( "265plb.s1", CRC(2072d5e9) SHA1(45f9eb101f4b58ce40c307fd9fa8b1e95215a81b) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1c661a4b) SHA1(4e5aa862a0a182a806d538996ddc68d9f2dffaf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265.c1", 0x0000000, 0x800000, CRC(2b65a656) SHA1(9c46d8cf5b1ef322db442ac6a9b9406ab49206c5) )
	ROM_LOAD16_BYTE( "265.c2", 0x0000001, 0x800000, CRC(adf18983) SHA1(150cd4a5e51e9df88688469d2ea7675c2cf3658a) )
	ROM_LOAD16_BYTE( "265.c3", 0x1000000, 0x800000, CRC(875e9fd7) SHA1(28f52d56192d48bbc5dc3c97abf456bd34a58cbd) )
	ROM_LOAD16_BYTE( "265.c4", 0x1000001, 0x800000, CRC(2da13947) SHA1(f8d79ec2c236aa3d3648a4f715676899602122c1) )
	ROM_LOAD16_BYTE( "265.c5", 0x2000000, 0x800000, CRC(61bd165d) SHA1(b3424db84bc683d858fb635bc42728f9cdd89caf) )
	ROM_LOAD16_BYTE( "265.c6", 0x2000001, 0x800000, CRC(03fdd1eb) SHA1(6155c7e802062f4eafa27e414c4e73ee59b868bf) )
	ROM_LOAD16_BYTE( "265.c7", 0x3000000, 0x800000, CRC(1a2749d8) SHA1(af7d9ec1d576209826fa568f676bbff92f6d6ddd) )
	ROM_LOAD16_BYTE( "265.c8", 0x3000001, 0x800000, CRC(ab0bb549) SHA1(d23afb60b7f831f7d4a98ad3c4a00ee19877a1ce) )
ROM_END

ROM_START( kof2k2pla ) /* same as kof2k2plb except s1 from smg, p1 is new */
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265pa.p1", 0x000000, 0x100000, CRC(6a3a02f3) SHA1(c9973b64e9a87fa38dde233ee3e9a73ba085b013) )
	ROM_LOAD16_WORD_SWAP( "265.p2", 0x100000, 0x400000, CRC(327266b8) SHA1(98f445cc0a94f8744d74bca71cb420277622b034) )

	NEO_SFIX_128K( "265smg.s1", CRC(1a3ed064) SHA1(9749bb55c750e6b65d651998c2649c5fb68db68e) )

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1c661a4b) SHA1(4e5aa862a0a182a806d538996ddc68d9f2dffaf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265.c1", 0x0000000, 0x800000, CRC(2b65a656) SHA1(9c46d8cf5b1ef322db442ac6a9b9406ab49206c5) )
	ROM_LOAD16_BYTE( "265.c2", 0x0000001, 0x800000, CRC(adf18983) SHA1(150cd4a5e51e9df88688469d2ea7675c2cf3658a) )
	ROM_LOAD16_BYTE( "265.c3", 0x1000000, 0x800000, CRC(875e9fd7) SHA1(28f52d56192d48bbc5dc3c97abf456bd34a58cbd) )
	ROM_LOAD16_BYTE( "265.c4", 0x1000001, 0x800000, CRC(2da13947) SHA1(f8d79ec2c236aa3d3648a4f715676899602122c1) )
	ROM_LOAD16_BYTE( "265.c5", 0x2000000, 0x800000, CRC(61bd165d) SHA1(b3424db84bc683d858fb635bc42728f9cdd89caf) )
	ROM_LOAD16_BYTE( "265.c6", 0x2000001, 0x800000, CRC(03fdd1eb) SHA1(6155c7e802062f4eafa27e414c4e73ee59b868bf) )
	ROM_LOAD16_BYTE( "265.c7", 0x3000000, 0x800000, CRC(1a2749d8) SHA1(af7d9ec1d576209826fa568f676bbff92f6d6ddd) )
	ROM_LOAD16_BYTE( "265.c8", 0x3000001, 0x800000, CRC(ab0bb549) SHA1(d23afb60b7f831f7d4a98ad3c4a00ee19877a1ce) )
ROM_END

ROM_START( kof2k2plb )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "2k2-p1p.bin",0x000000, 0x100000, CRC(3ab03781) SHA1(86946c19f1c4d9ab5cde86688d698bf63118a39d) )
	ROM_LOAD16_WORD_SWAP( "265.p2", 0x100000, 0x400000, CRC(327266b8) SHA1(98f445cc0a94f8744d74bca71cb420277622b034) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "265.m1", CRC(85aaa632) SHA1(744fba4ca3bc3a5873838af886efb97a8a316104) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265.c1", 0x0000000, 0x800000, CRC(2b65a656) SHA1(9c46d8cf5b1ef322db442ac6a9b9406ab49206c5) )
	ROM_LOAD16_BYTE( "265.c2", 0x0000001, 0x800000, CRC(adf18983) SHA1(150cd4a5e51e9df88688469d2ea7675c2cf3658a) )
	ROM_LOAD16_BYTE( "265.c3", 0x1000000, 0x800000, CRC(875e9fd7) SHA1(28f52d56192d48bbc5dc3c97abf456bd34a58cbd) )
	ROM_LOAD16_BYTE( "265.c4", 0x1000001, 0x800000, CRC(2da13947) SHA1(f8d79ec2c236aa3d3648a4f715676899602122c1) )
	ROM_LOAD16_BYTE( "265.c5", 0x2000000, 0x800000, CRC(61bd165d) SHA1(b3424db84bc683d858fb635bc42728f9cdd89caf) )
	ROM_LOAD16_BYTE( "265.c6", 0x2000001, 0x800000, CRC(03fdd1eb) SHA1(6155c7e802062f4eafa27e414c4e73ee59b868bf) )
	ROM_LOAD16_BYTE( "265.c7", 0x3000000, 0x800000, CRC(1a2749d8) SHA1(af7d9ec1d576209826fa568f676bbff92f6d6ddd) )
	ROM_LOAD16_BYTE( "265.c8", 0x3000001, 0x800000, CRC(ab0bb549) SHA1(d23afb60b7f831f7d4a98ad3c4a00ee19877a1ce) )
ROM_END

ROM_START( kof2k2plc )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265nu.p1",  0x000000, 0x100000, CRC(ebedae17) SHA1(5f84562f9ef7a6b007290e59b31a62aca208f579) )
	ROM_LOAD16_WORD_SWAP( "265.p2", 0x100000, 0x400000, CRC(327266b8) SHA1(98f445cc0a94f8744d74bca71cb420277622b034) )

	NEO_SFIX_128K( "265pc.s1", CRC(fecbb589) SHA1(90ffc38651af9907aa7c54914d6f74b674fb0ad9) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "265.m1", CRC(85aaa632) SHA1(744fba4ca3bc3a5873838af886efb97a8a316104) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265.c1", 0x0000000, 0x800000, CRC(2b65a656) SHA1(9c46d8cf5b1ef322db442ac6a9b9406ab49206c5) )
	ROM_LOAD16_BYTE( "265.c2", 0x0000001, 0x800000, CRC(adf18983) SHA1(150cd4a5e51e9df88688469d2ea7675c2cf3658a) )
	ROM_LOAD16_BYTE( "265.c3", 0x1000000, 0x800000, CRC(875e9fd7) SHA1(28f52d56192d48bbc5dc3c97abf456bd34a58cbd) )
	ROM_LOAD16_BYTE( "265.c4", 0x1000001, 0x800000, CRC(2da13947) SHA1(f8d79ec2c236aa3d3648a4f715676899602122c1) )
	ROM_LOAD16_BYTE( "265.c5", 0x2000000, 0x800000, CRC(61bd165d) SHA1(b3424db84bc683d858fb635bc42728f9cdd89caf) )
	ROM_LOAD16_BYTE( "265.c6", 0x2000001, 0x800000, CRC(03fdd1eb) SHA1(6155c7e802062f4eafa27e414c4e73ee59b868bf) )
	ROM_LOAD16_BYTE( "265.c7", 0x3000000, 0x800000, CRC(1a2749d8) SHA1(af7d9ec1d576209826fa568f676bbff92f6d6ddd) )
	ROM_LOAD16_BYTE( "265.c8", 0x3000001, 0x800000, CRC(ab0bb549) SHA1(d23afb60b7f831f7d4a98ad3c4a00ee19877a1ce) )
ROM_END

ROM_START( kof2002d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "265.p1",  0x000000, 0x100000, CRC(9ede7323) SHA1(ad9d45498777fda9fa58e75781f48e09aee705a6) )
	ROM_LOAD16_WORD_SWAP( "265.p2", 0x100000, 0x400000, CRC(327266b8) SHA1(98f445cc0a94f8744d74bca71cb420277622b034) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "265d.m1", CRC(1c661a4b) SHA1(4e5aa862a0a182a806d538996ddc68d9f2dffaf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265.v1", 0x000000, 0x800000, CRC(15e8f3f5) SHA1(7c9e6426b9fa6db0158baa17a6485ffce057d889) )
	ROM_LOAD( "265.v2", 0x800000, 0x800000, CRC(da41d6f9) SHA1(a43021f1e58947dcbe3c8ca5283b20b649f0409d) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c1", 0x0000000, 0x800000, CRC(7efa6ef7) SHA1(71345A4202E7CC9239538FB978638141416C8893) )
	ROM_LOAD16_BYTE( "265d.c2", 0x0000001, 0x800000, CRC(aa82948b) SHA1(B2A40797F68BDEB80BC54DCCC5495BE68934BF0E) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

 /***************************
  The King of Fighters '2003
*****************************/

ROM_START( kof2003hack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271_hack01.p1", 0x000000, 0x800000, CRC(a131786e) SHA1(3754e6361063221138c95c52a3edb3e88a74048a) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "271_hack01.m1", CRC(0e86af8f) SHA1(769102b67bb1a699cfa5674d66cdb46ae633cb65) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271b.v1", 0x000000, 0x400000, CRC(d2b8aa5e) SHA1(498f0556c1de56822141f8043f1ce20444f4ed0a) )
	ROM_LOAD( "271b.v2", 0x400000, 0x400000, CRC(71956ee2) SHA1(a890941e60db358cf45b58909f4719f4826f3bb1) )
	ROM_LOAD( "271b.v3", 0x800000, 0x400000, CRC(ddbbb199) SHA1(0eea4b064bdb8daa03c354fe0a0aa27c4c665bda) )
	ROM_LOAD( "271b.v4", 0xc00000, 0x400000, CRC(01b90c4f) SHA1(387164aa1995d8c11ed939b3afbc294d86d2e27f) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271_hack01.c7", 0x3000000, 0x800000, CRC(f3d81b6e) SHA1(62bf0c9d1d9f7a016d125df64c8328e01e003e43) )
	ROM_LOAD16_BYTE( "271_hack01.c8", 0x3000001, 0x800000, CRC(5310c6dc) SHA1(e9cb89392fa5604d3ac3ba58acdb1b739d1267be) )
ROM_END

ROM_START( kof2003hack02 ) //kof2003ps2bs
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271_hack02.p0", 0x000000, 0x100000, CRC(fa166186) SHA1(8f0db81b7edb2a4ba90da7869db2be39f01f9b2f) )
	ROM_LOAD16_WORD_SWAP( "271_hack02.p1", 0x100000, 0x400000, CRC(34f6a1f5) SHA1(31f32205b8cf3078c9236f2ea28585b9d2370328) )
	ROM_LOAD16_WORD_SWAP( "271d.p2", 0x500000, 0x200000, CRC(0d0a5861) SHA1(823dd3d11574a42f98588a6d1f9bb5d721f34be2) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "271_hack03.m1", CRC(4374cb5c) SHA1(d245a09fcebc8fe59f4af2de2fc30a0871e56d68) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271bs1.v1", 0x000000, 0x800000, CRC(dd6c6a85) SHA1(e32b16d0b716a4f3b95868457347303d020147fb) )
	ROM_LOAD( "271bs1.v2", 0x800000, 0x800000, CRC(0e84f8c1) SHA1(28c964fe89c800b445d3a65c965de32b4e504726) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271_hack02.c7", 0x3000000, 0x800000, CRC(36c14e4c) SHA1(20970488f37a48105842d9257af758e27d42ae5f) )
	ROM_LOAD16_BYTE( "271_hack03.c8", 0x3000001, 0x800000, CRC(709feee8) SHA1(1a8b7db93756ba38548ced1e586c7b660e94ecd3) )
ROM_END

ROM_START( kof2003hack03 ) // kf2k3upd
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "364_hack03.p1", 0x000000, 0x100000, CRC(09253c62) SHA1(1e86c2294013f2e2e06679803599b201d7d68700) )
	ROM_LOAD16_WORD_SWAP( "364pd.p2", 0x100000, 0x600000, CRC(264744e2) SHA1(cfd43006dad8dc8349b6cb3db0953991ab1d414d) )

	NEO_SFIX( 0x80000, "364_hack03.s1", CRC(3df88da7) SHA1(50ad48ef1e57bbf522d9e6c0c9e7f88c4a96d1f9) )

	NEO_BIOS_AUDIO_128K( "271b.m1", CRC(3a4969ff) SHA1(2fc107a023a82053a8df63025829bcf12cee9610) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271b.v1", 0x000000, 0x400000, CRC(d2b8aa5e) SHA1(498f0556c1de56822141f8043f1ce20444f4ed0a) )
	ROM_LOAD( "271b.v2", 0x400000, 0x400000, CRC(71956ee2) SHA1(a890941e60db358cf45b58909f4719f4826f3bb1) )
	ROM_LOAD( "271b.v3", 0x800000, 0x400000, CRC(ddbbb199) SHA1(0eea4b064bdb8daa03c354fe0a0aa27c4c665bda) )
	ROM_LOAD( "271b.v4", 0xc00000, 0x400000, CRC(01b90c4f) SHA1(387164aa1995d8c11ed939b3afbc294d86d2e27f) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271b.c7", 0x3000000, 0x800000, CRC(374ea523) SHA1(613827d72c6181f3e08353750c9af0c4dbad020b) )
	ROM_LOAD16_BYTE( "271b.c8", 0x3000001, 0x800000, CRC(75211f4d) SHA1(d82f044e816ee539ff131d9c931200c818d34cd0) )
ROM_END

 /********
  bootleg
***********/

ROM_START( kof2003d )
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271d.p0", 0x000000, 0x100000, CRC(7c2aee16) SHA1(b4373569c4a26829a93759ba6917879518f2fb20) )
	ROM_LOAD16_WORD_SWAP( "271b.p1", 0x100000, 0x400000, CRC(92ed6ee3) SHA1(5e7e21eb40dfcc453ba73808760d5ddedd49c58a) )
	ROM_LOAD16_WORD_SWAP( "271d.p2", 0x500000, 0x200000, CRC(0d0a5861) SHA1(823dd3d11574a42f98588a6d1f9bb5d721f34be2) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "271.m1", CRC(f5515629) SHA1(7516bf1b0207a3c8d41dc30c478f8d8b1f71304b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271.v1", 0x000000, 0x800000, CRC(ffa3f8c7) SHA1(7cf4a933973ca23b7f87c81151d8659e6ec4bd20) )
	ROM_LOAD( "271.v2", 0x800000, 0x800000, CRC(5382c7d1) SHA1(1bf999705eda80ba1e7b0d6bdd010d9bfb18bd76) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271d.c7", 0x3000000, 0x800000, CRC(8ee6b43c) SHA1(c78b749c7c8e1d8ab8335b96da03764cc8e9eb21) )
	ROM_LOAD16_BYTE( "271d.c8", 0x3000001, 0x800000, CRC(6d8d2d60) SHA1(841698eead0f7d422937ce2f2329e17c383d90a6) )
ROM_END

ROM_START( kof2003f )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271f.p1", 0x000000, 0x400000, CRC(d29a0d06) SHA1(a5ba68ceac0b4bceb0c37d65844d96835eb244e4) )
	ROM_LOAD16_WORD_SWAP( "271f.p2", 0x400000, 0x400000, CRC(95477dd4) SHA1(3a57befa3e0772b2989ba5b76afd95f3aed1936b) )

	NEO_SFIX_128K( "271f.s1", CRC(7c7829aa) SHA1(22f8d38d1d0891295d0593741d9477fbe6b4f48c) )

	NEO_BIOS_AUDIO_512K( "271_hack01.m1", CRC(0e86af8f) SHA1(769102b67bb1a699cfa5674d66cdb46ae633cb65) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271f.v1", 0x000000, 0x1000000, CRC(2964f36e) SHA1(bfc52f81354eb57e27acb3b80fe1547b9e070293) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271pcd.c1", 0x0000000, 0x1000000, CRC(c29acd28) SHA1(8a10409c5a9ad95fa9b56e94c14f1b96101fb179) )
	ROM_LOAD16_BYTE( "271pcd.c2", 0x0000001, 0x1000000, CRC(328e80b1) SHA1(c7f8069488be1a88a2ea03718b6a131f5c96bd3f) )
	ROM_LOAD16_BYTE( "271pcd.c3", 0x2000000, 0x1000000, CRC(020a11f1) SHA1(85cfae76234f72b0039ebc02f931bb2a9c10b1af) )
	ROM_LOAD16_BYTE( "271pcd.c4", 0x2000001, 0x1000000, CRC(991b5ed2) SHA1(99c4c470bc9cb388773e27de6df4a16803fc7b45) )
	ROM_LOAD16_BYTE( "271pcd.c5", 0x4000000, 0x1000000, CRC(c2de8b66) SHA1(40c2ea48fc20d470163a9dbb40c0276fc4cfceb9) )
	ROM_LOAD16_BYTE( "271pcd.c6", 0x4000001, 0x1000000, CRC(3ff750db) SHA1(714f14a2eb2df6f25d10a6b6aff4b3adfbc7a5dc) )
ROM_END

ROM_START( kof2k3d )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "271.p1", 0x000000, 0x400000, CRC(530ecc14) SHA1(812cf7e9902af3f5e9e330b7c05c2171b139ad2b) )
	ROM_LOAD32_WORD_SWAP( "271.p2", 0x000002, 0x400000, CRC(fd568da9) SHA1(46364906a1e81dc251117e91a1a7b43af1373ada) )
	ROM_LOAD16_WORD_SWAP( "271.p3", 0x800000, 0x100000, CRC(aec5b4a9) SHA1(74087f785590eda5898ce146029818f86ced42b6) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "271.m1", CRC(f5515629) SHA1(7516bf1b0207a3c8d41dc30c478f8d8b1f71304b) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271.v1", 0x000000, 0x800000, CRC(ffa3f8c7) SHA1(7cf4a933973ca23b7f87c81151d8659e6ec4bd20) )
	ROM_LOAD( "271.v2", 0x800000, 0x800000, CRC(5382c7d1) SHA1(1bf999705eda80ba1e7b0d6bdd010d9bfb18bd76) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271d.c7", 0x3000000, 0x800000, CRC(8ee6b43c) SHA1(c78b749c7c8e1d8ab8335b96da03764cc8e9eb21) )
	ROM_LOAD16_BYTE( "271d.c8", 0x3000001, 0x800000, CRC(6d8d2d60) SHA1(841698eead0f7d422937ce2f2329e17c383d90a6) )
ROM_END

ROM_START( kof2k3fd )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271nrx.p1", 0x000000, 0x800000, CRC(57a1981d) SHA1(0bc21c09c127e9abb3b03924166c0065ddec2057) )

	NEO_SFIX_128K( "271nrx.s1", CRC(df422440) SHA1(ed84dfd6cc5a73f3ef35db6616bcc3b45754aa4d) )
	//NEO_SFIX( 0x80000, "271fd.s1", CRC(3230e10f) SHA1(a7aa4c11be7de5969e00338c7f424811174a8ad6) )    // bad

	NEO_BIOS_AUDIO_512K( "271fd.m1", CRC(cc8b54c0) SHA1(c0dabaad7a8e9e57774af8fa1009296af5c60005) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271bs1.v1", 0x000000, 0x800000, CRC(dd6c6a85) SHA1(e32b16d0b716a4f3b95868457347303d020147fb) )
	ROM_LOAD( "271bs1.v2", 0x800000, 0x800000, CRC(0e84f8c1) SHA1(28c964fe89c800b445d3a65c965de32b4e504726) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271d.c7", 0x3000000, 0x800000, CRC(8ee6b43c) SHA1(c78b749c7c8e1d8ab8335b96da03764cc8e9eb21) )
	ROM_LOAD16_BYTE( "271d.c8", 0x3000001, 0x800000, CRC(6d8d2d60) SHA1(841698eead0f7d422937ce2f2329e17c383d90a6) )
ROM_END

ROM_START( kof2k3hd ) // decrypted version of kof2003h
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "271h.p1", 0x000000, 0x400000, CRC(d0d0ae3e) SHA1(538d054ac50c91694fbbfefcce548b063713e14e) )
	ROM_LOAD32_WORD_SWAP( "271h.p2", 0x000002, 0x400000, CRC(fb3f79d9) SHA1(f253d10e732d6e23ae82d74ac9269d21f69ddb4d) )
	ROM_LOAD16_WORD_SWAP( "271h.p3", 0x800000, 0x100000, CRC(232702ad) SHA1(6045046027dac1cbd4cbd14b5c1ece522bc6197f) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "271-m1k.m1", CRC(48d9affe) SHA1(68f01560b91bbada39001ce01bdeeed5c9bb29f2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271.v1", 0x000000, 0x800000, CRC(ffa3f8c7) SHA1(7cf4a933973ca23b7f87c81151d8659e6ec4bd20) )
	ROM_LOAD( "271.v2", 0x800000, 0x800000, CRC(5382c7d1) SHA1(1bf999705eda80ba1e7b0d6bdd010d9bfb18bd76) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271hd.c7", 0x3000000, 0x800000, CRC(efe714f8) SHA1(c239744cf372f6f7b2cd1b539a59f792038fe179) )
	ROM_LOAD16_BYTE( "271hd.c8", 0x3000001, 0x800000, CRC(c015cdb7) SHA1(baa1bf179f13e07bd93d33bf40b37deaf65d0b0c) )
ROM_END

ROM_START( kof2k3nd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "271nd.p1", 0x000000, 0x800000, CRC(11624854) SHA1(a7488a0f5d731656aa82cc51282adcfe6ad9f658) )

	NEO_SFIX_128K( "271nrx.s1", CRC(df422440) SHA1(ed84dfd6cc5a73f3ef35db6616bcc3b45754aa4d) )

	NEO_BIOS_AUDIO_512K( "271_hack01.m1", CRC(0e86af8f) SHA1(769102b67bb1a699cfa5674d66cdb46ae633cb65) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271f.v1", 0x000000, 0x1000000, CRC(2964f36e) SHA1(bfc52f81354eb57e27acb3b80fe1547b9e070293) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271b.c1", 0x0000000, 0x800000, CRC(e42fc226) SHA1(1cd9364993e141facdcdd53ec2277df7b275d8a7) )
	ROM_LOAD16_BYTE( "271b.c2", 0x0000001, 0x800000, CRC(1b5e3b58) SHA1(0eb254477a9479541291e43e415310852a0f0bed) )
	ROM_LOAD16_BYTE( "271b.c3", 0x1000000, 0x800000, CRC(d334fdd9) SHA1(1efe30b4f56a55e25ab518cf6999de797b5e407c) )
	ROM_LOAD16_BYTE( "271b.c4", 0x1000001, 0x800000, CRC(0d457699) SHA1(ec73d0c9fc7094d0ac6c0986a6e07cde25893e57) )
	ROM_LOAD16_BYTE( "271b.c5", 0x2000000, 0x800000, CRC(8a91aae4) SHA1(802f4baacf801646be1ef686e105b2e867a6a5df) )
	ROM_LOAD16_BYTE( "271b.c6", 0x2000001, 0x800000, CRC(9f8674b8) SHA1(65964f40b2227d020023fb436579927e65807dcd) )
	ROM_LOAD16_BYTE( "271b.c7", 0x3000000, 0x800000, CRC(374ea523) SHA1(613827d72c6181f3e08353750c9af0c4dbad020b) )
	ROM_LOAD16_BYTE( "271b.c8", 0x3000001, 0x800000, CRC(75211f4d) SHA1(d82f044e816ee539ff131d9c931200c818d34cd0) )
ROM_END

ROM_START( kof2k3pcd )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "271c.p1", 0x000000, 0x400000, CRC(b9da070c) SHA1(1a26325af142a4dd221c336061761468598c4634) )
	ROM_LOAD32_WORD_SWAP( "271c.p2", 0x000002, 0x400000, CRC(da3118c4) SHA1(582e4f44f03276adecb7b2848d3b96bf6da57f1e) )
	ROM_LOAD16_WORD_SWAP( "271c.p3", 0x800000, 0x100000, CRC(5cefd0d2) SHA1(cddc3164629fed4b6f715e12b109ad35d1009355) )

	ROM_Y_ZOOM
	ROM_REGION( 0x100000, "fixed", ROMREGION_ERASE00 )
	ROM_REGION( 0x20000, "fixedbios", 0 )
	ROM_LOAD( "sfix.sfix", 0x000000, 0x20000, CRC(c2ea0cfd) SHA1(fd4a618cdcdbf849374f0a50dd8efe9dbab706c3) )

	ROM_REGION16_BE( 0x80000, "mainbios", 0 )
	ROM_LOAD16_WORD_SWAP( "271pcd.sp1", 0x00000, 0x080000, CRC(e873f1ef) SHA1(e7f7a6084b9d7f4b4f8819ea0fd115b1f2cac2c2) )

	ROM_REGION( 0x80000, "audiocrypt", 0 )
	ROM_LOAD( "271c.m1", 0x00000, 0x80000, CRC(d6bcf2bc) SHA1(df78bc95990eb8e8f3638dde6e1876354df7fe84) )
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "271c.v1", 0x000000, 0x1000000, CRC(1d96154b) SHA1(1d4e262b0d30cee79a4edc83bb9706023c736668) )

	ROM_REGION( 0x6000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "271pcd.c1", 0x0000000, 0x1000000, CRC(c29acd28) SHA1(8a10409c5a9ad95fa9b56e94c14f1b96101fb179) )
	ROM_LOAD16_BYTE( "271pcd.c2", 0x0000001, 0x1000000, CRC(328e80b1) SHA1(c7f8069488be1a88a2ea03718b6a131f5c96bd3f) )
	ROM_LOAD16_BYTE( "271pcd.c3", 0x2000000, 0x1000000, CRC(020a11f1) SHA1(85cfae76234f72b0039ebc02f931bb2a9c10b1af) )
	ROM_LOAD16_BYTE( "271pcd.c4", 0x2000001, 0x1000000, CRC(991b5ed2) SHA1(99c4c470bc9cb388773e27de6df4a16803fc7b45) )
	ROM_LOAD16_BYTE( "271pcd.c5", 0x4000000, 0x1000000, CRC(c2de8b66) SHA1(40c2ea48fc20d470163a9dbb40c0276fc4cfceb9) )
	ROM_LOAD16_BYTE( "271pcd.c6", 0x4000001, 0x1000000, CRC(3ff750db) SHA1(714f14a2eb2df6f25d10a6b6aff4b3adfbc7a5dc) )
ROM_END

 /*****************
 King of Gladiator
********************/

ROM_START( koghack01 ) //kogptgb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "5232_hack01.p1", 0x000000, 0x100000, CRC(ea2ce829) SHA1(a7a1c785ed3eb1c0c6e348942cbe1fc154da8ce0) )
	ROM_LOAD16_WORD_SWAP( "5232_hack01.p2", 0x100000, 0x400000, CRC(d1d6c3a5) SHA1(a8878c2da73e7c25330d2f2130b63b6e8df71961) )
	ROM_FILL(0x0263ba,1,0x65)

	NEO_SFIX_128K( "5232_hack01.s1", CRC(f178fbde) SHA1(886142f1f52439712329f0028654829340f28627) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "5232_hack01.c1", 0x0000000, 0x800000, CRC(be9ec0a5) SHA1(c77006fe0f24fb7cb80b4ed079927148059f72d9) )
	ROM_LOAD16_BYTE( "5232_hack01.c2", 0x0000001, 0x800000, CRC(f4739e81) SHA1(1f2ea1f84edeb867e8ed630f6e148852a44803a2) )
	ROM_LOAD16_BYTE( "232yk.c3",  0x1000000, 0x800000, CRC(b092e64f) SHA1(7d809432d068db8f9056f5210570191416f1a53b) )
	ROM_LOAD16_BYTE( "232yk.c4",  0x1000001, 0x800000, CRC(d25e8a04) SHA1(52227828e3fdd27399b4cd514551abf856293f47) )
	ROM_LOAD16_BYTE( "5232_hack01.c5",   0x2000000, 0x400000, CRC(3c779848) SHA1(20ec25277295de9dfd00b3380607e9f40b32c6e6) )
	ROM_LOAD16_BYTE( "5232_hack01.c6",   0x2000001, 0x400000, CRC(b0178cf0) SHA1(a20a9bbd90e76ba72baea2c8ada2ec4cca64d02c) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kogd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "361d.p1", 0x000000, 0x100000, CRC(58F20940) SHA1(693819ff606b7c7b27400586f34073625445c412) )
	ROM_LOAD16_WORD_SWAP( "232.p2", 0x100000, 0x400000, CRC(158b23f6) SHA1(9744620a70513490aaf9c5eda33e5ec31222be19) )

	NEO_SFIX_128K( "361d.s1", CRC(EC690592) SHA1(a5af2880778a1904dd6c6006be769ea36b15dae3) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "361d.c1", 0x0000000, 0x800000, CRC(8E964A75) SHA1(e7611d7908ca056f4b3c0ae2c4da8f6809db4556) )
	ROM_LOAD16_BYTE( "361d.c2", 0x0000001, 0x800000, CRC(D102DE17) SHA1(e98f33a4d16ebda6cf369b92480d4306c780cf8b) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "361d.c5", 0x2000000, 0x400000, CRC(C2698BE3) SHA1(0b1d414549947f6c29ebc71057aef96634656f32) )
	ROM_LOAD16_BYTE( "361d.c6", 0x2000001, 0x400000, CRC(B057DB30) SHA1(2f342f03848f3f553bc70b2dbee2cc388a344a06) )
ROM_END

ROM_START( kogplus )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "361plus.p1", 0x000000, 0x200000, CRC(0338f453) SHA1(01ea5ca6cc632d732a539542dc163bbafb071b88) )
	ROM_LOAD16_WORD_SWAP( "kf97-p2p.bin", 0x200000, 0x400000, CRC(5502b020) SHA1(37c48198d8b3798910a44075782cd1a20b687b4a) )

	NEO_SFIX_128K( "361plus.s1", CRC(6271e9ba) SHA1(5bc94426e916d0ad16b6660bfc2b9e21f717fa32) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "5232-c1a.bin", 0x0000000, 0x800000, CRC(4eab9b0a) SHA1(a6f6b755215a3f41474e0a76b5463303a522c2d3) )
	ROM_LOAD16_BYTE( "5232-c2a.bin", 0x0000001, 0x800000, CRC(697f8fd0) SHA1(5784464c2357ccef8e6e79b6298843fc3d13b39c) )
	ROM_LOAD16_BYTE( "5232-c1b.bin", 0x1000000, 0x800000, CRC(1143fdf3) SHA1(9dc5fe9a3b7599380db62095880e2d6f237a41bd) )
	ROM_LOAD16_BYTE( "5232-c2b.bin", 0x1000001, 0x800000, CRC(ea82cf8f) SHA1(3d9ab64b69cecd6b3950839ac2c6d151ad66dcf8) )
	ROM_LOAD16_BYTE( "5232-c3.bin",  0x2000000, 0x400000, CRC(abd1be07) SHA1(857eb68bbee4538770bbfa77aaa540d61ab0abcd) )
	ROM_LOAD16_BYTE( "5232-c4.bin",  0x2000001, 0x400000, CRC(d2bd967b) SHA1(c494e0a98e127d37ca360a28accc167fa50fb626) )
ROM_END

ROM_START( kogplus1 ) 
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "361plus1.p1",  0x000000, 0x100000, CRC(98241142) SHA1(9bb2c382063f84920841463f9b4ca87862e336de) )
	ROM_LOAD16_WORD_SWAP( "kf97-p2p.bin", 0x100000, 0x400000, CRC(5502b020) SHA1(37c48198d8b3798910a44075782cd1a20b687b4a) )

	NEO_SFIX_128K( "361plus1.s1", CRC(1a58ab17) SHA1(964ef974570923743124f4879baa8582ed4fc9f9) )

	NEO_BIOS_AUDIO_128K( "232.m1", CRC(45348747) SHA1(ed77cbae2b208d1177a9f5f6e8cd57070e90b65b) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "232.v1", 0x000000, 0x400000, CRC(22a2b5b5) SHA1(ebdbc977332e6d93e266755000b43857e0082965) )
	ROM_LOAD( "232.v2", 0x400000, 0x400000, CRC(2304e744) SHA1(98d283e2bcc9291a53f52afd35ef76dfb0828432) )
	ROM_LOAD( "232.v3", 0x800000, 0x400000, CRC(759eb954) SHA1(54e77c4e9e6b89458e59824e478ddc33a9c72655) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "361d.c1", 0x0000000, 0x800000, CRC(8E964A75) SHA1(e7611d7908ca056f4b3c0ae2c4da8f6809db4556) )
	ROM_LOAD16_BYTE( "361d.c2", 0x0000001, 0x800000, CRC(D102DE17) SHA1(e98f33a4d16ebda6cf369b92480d4306c780cf8b) )
	ROM_LOAD16_BYTE( "232.c3", 0x1000000, 0x800000, CRC(581d6618) SHA1(14d3124a08ded59f86932c6b28e1a4e48c564ccd) )
	ROM_LOAD16_BYTE( "232.c4", 0x1000001, 0x800000, CRC(49bb1e68) SHA1(f769c1bd1b019521111ff3f0d22c63cb1f2640ef) )
	ROM_LOAD16_BYTE( "361d.c5", 0x2000000, 0x400000, CRC(C2698BE3) SHA1(0b1d414549947f6c29ebc71057aef96634656f32) )
	ROM_LOAD16_BYTE( "361d.c6", 0x2000001, 0x400000, CRC(B057DB30) SHA1(2f342f03848f3f553bc70b2dbee2cc388a344a06) )
ROM_END

 /********************
 Kof 10th Anniversary
***********************/

ROM_START( kof10thephack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "5008-p1_hack01.bin",  0x000000, 0x200000, CRC(8981b627) SHA1(b25754a6339f6533af791cd7dea95988b5f9072a) )
	ROM_IGNORE(0x254f9f)
	ROM_LOAD16_WORD_SWAP( "5008-p2_hack01.bin",  0x200000, 0x400000, CRC(8eca396b) SHA1(58918c9dff5dc6e8975e16943ef8aefcd4caf34d) )
	ROM_IGNORE(0x054f59)
	ROM_LOAD16_WORD_SWAP( "5008-p3.bin", 0x600000, 0x200000, CRC(e629e13c) SHA1(6ebe080ce01c51064cb2f4d89315ba98a45ae727) )

	NEO_SFIX_128K( "5008-s1.bin", CRC(92410064) SHA1(1fb800b46341858207d3b6961a760289fbec7faa) )

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "kf10-c1a.bin", 0x0000000, 0x400000, CRC(3bbc0364) SHA1(e8aa7ff82f151ce1db56f259377b64cceef85af0) )
	ROM_LOAD16_BYTE( "kf10-c2a.bin", 0x0000001, 0x400000, CRC(91230075) SHA1(d9098e05a7ba6008661147b6bf8bc2f494b8b72b) )
	ROM_LOAD16_BYTE( "kf10-c1b.bin", 0x0800000, 0x400000, CRC(b5abfc28) SHA1(eabf60992bb3485c95330065294071ec155bfe7c) )
	ROM_LOAD16_BYTE( "kf10-c2b.bin", 0x0800001, 0x400000, CRC(6cc4c6e1) SHA1(be824a944e745ee18efdc45c81fd496a4d624b9c) )
	ROM_LOAD16_BYTE( "kf10-c3a.bin", 0x1000000, 0x400000, CRC(5b3d4a16) SHA1(93ac1cd7739100f8c32732644f81f2a19837b131) )
	ROM_LOAD16_BYTE( "kf10-c4a.bin", 0x1000001, 0x400000, CRC(c6f3419b) SHA1(340c17a73aeb7bf8a6209f8459e6f00000075b50) )
	ROM_LOAD16_BYTE( "kf10-c3b.bin", 0x1800000, 0x400000, CRC(9d2bba19) SHA1(5ebbd0af3f83a60e33c8ccb743e3d5f5a96f1273) )
	ROM_LOAD16_BYTE( "kf10-c4b.bin", 0x1800001, 0x400000, CRC(5a4050cb) SHA1(8fd2291f349efa1ed5cd37ad4e273b60fe831a77) )
	ROM_LOAD16_BYTE( "kf10-c5a.bin", 0x2000000, 0x400000, CRC(a289d1e1) SHA1(50c7d7ebde6e118a01036cc3e40827fcd9f0d3fd) )
	ROM_LOAD16_BYTE( "kf10-c6a.bin", 0x2000001, 0x400000, CRC(e6494b5d) SHA1(18e064b9867ae0b0794065f8dbefd486620419db) )
	ROM_LOAD16_BYTE( "kf10-c5b.bin", 0x2800000, 0x400000, CRC(404fff02) SHA1(56d1b32c87ea4885e49264e8b21846e465a20e1f) )
	ROM_LOAD16_BYTE( "kf10-c6b.bin", 0x2800001, 0x400000, CRC(f2ccfc9e) SHA1(69db7fac7023785ab94ea711a72dbc2826cfe1a3) )
	ROM_LOAD16_BYTE( "kf10-c7a.bin", 0x3000000, 0x400000, CRC(be79c5a8) SHA1(ded3c5eb3571647f50533eb682c2675372ace3fb) )
	ROM_LOAD16_BYTE( "kf10-c8a.bin", 0x3000001, 0x400000, CRC(a5952ca4) SHA1(76dbb3cb45ce5a4beffa1ed29491204fc6617e42) )
	ROM_LOAD16_BYTE( "5008-c7b.bin", 0x3800000, 0x400000, CRC(33604ef0) SHA1(57deec23c81d5d673ce5992cef1f2567f1a2148e) )
	ROM_LOAD16_BYTE( "5008-c8b.bin", 0x3800001, 0x400000, CRC(51f6a8f8) SHA1(9ef1cdbdd125a2b430346c22b59f36902312905f) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof10thd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "363d.p1", 0x000000, 0x800000, CRC(30c82f4c) SHA1(d02e6fa14af6fd4aff0349fcb3d275d9226ccbe7) )

	NEO_SFIX_128K( "363d.s1", CRC(3c757cb1) SHA1(fea798902c59c125c8d31f42d52aa22caa31fc7b) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "kf10-c1a.bin", 0x0000000, 0x400000, CRC(3bbc0364) SHA1(e8aa7ff82f151ce1db56f259377b64cceef85af0) )
	ROM_LOAD16_BYTE( "kf10-c2a.bin", 0x0000001, 0x400000, CRC(91230075) SHA1(d9098e05a7ba6008661147b6bf8bc2f494b8b72b) )
	ROM_LOAD16_BYTE( "kf10-c1b.bin", 0x0800000, 0x400000, CRC(b5abfc28) SHA1(eabf60992bb3485c95330065294071ec155bfe7c) )
	ROM_LOAD16_BYTE( "kf10-c2b.bin", 0x0800001, 0x400000, CRC(6cc4c6e1) SHA1(be824a944e745ee18efdc45c81fd496a4d624b9c) )
	ROM_LOAD16_BYTE( "kf10-c3a.bin", 0x1000000, 0x400000, CRC(5b3d4a16) SHA1(93ac1cd7739100f8c32732644f81f2a19837b131) )
	ROM_LOAD16_BYTE( "kf10-c4a.bin", 0x1000001, 0x400000, CRC(c6f3419b) SHA1(340c17a73aeb7bf8a6209f8459e6f00000075b50) )
	ROM_LOAD16_BYTE( "kf10-c3b.bin", 0x1800000, 0x400000, CRC(9d2bba19) SHA1(5ebbd0af3f83a60e33c8ccb743e3d5f5a96f1273) )
	ROM_LOAD16_BYTE( "kf10-c4b.bin", 0x1800001, 0x400000, CRC(5a4050cb) SHA1(8fd2291f349efa1ed5cd37ad4e273b60fe831a77) )
	ROM_LOAD16_BYTE( "kf10-c5a.bin", 0x2000000, 0x400000, CRC(a289d1e1) SHA1(50c7d7ebde6e118a01036cc3e40827fcd9f0d3fd) )
	ROM_LOAD16_BYTE( "kf10-c6a.bin", 0x2000001, 0x400000, CRC(e6494b5d) SHA1(18e064b9867ae0b0794065f8dbefd486620419db) )
	ROM_LOAD16_BYTE( "kf10-c5b.bin", 0x2800000, 0x400000, CRC(404fff02) SHA1(56d1b32c87ea4885e49264e8b21846e465a20e1f) )
	ROM_LOAD16_BYTE( "kf10-c6b.bin", 0x2800001, 0x400000, CRC(f2ccfc9e) SHA1(69db7fac7023785ab94ea711a72dbc2826cfe1a3) )
	ROM_LOAD16_BYTE( "kf10-c7a.bin", 0x3000000, 0x400000, CRC(be79c5a8) SHA1(ded3c5eb3571647f50533eb682c2675372ace3fb) )
	ROM_LOAD16_BYTE( "kf10-c8a.bin", 0x3000001, 0x400000, CRC(a5952ca4) SHA1(76dbb3cb45ce5a4beffa1ed29491204fc6617e42) )
	ROM_LOAD16_BYTE( "kf10-c7b.bin", 0x3800000, 0x400000, CRC(3fdb3542) SHA1(7d2050752a2064cd6729f483a0da93808e2c6033) )
	ROM_LOAD16_BYTE( "kf10-c8b.bin", 0x3800001, 0x400000, CRC(661b7a52) SHA1(0ae2ad2389134892f156337332b77adade3ddad1) )
ROM_END

 /*********************************
 Kof 10th Anniversary 2005 Unique
************************************/

ROM_START( kf2k5unihack01 ) //kof10thuo
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "363_hack01.p1", 0x000000, 0x800000, CRC(27daccbb) SHA1(690875dba906231cb5a5a64afdfd01874488de55) )

	NEO_SFIX_128K( "363_hack01.s1", CRC(312bf34b) SHA1(308037c274a491ec974cd9a1be9bda30e55fd25d) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "363_hack01.c1", 0x0000000, 0x800000, CRC(2c59db66) SHA1(67af8d7b31098ffc71b302bc48384689ad02e706) )
	ROM_LOAD16_BYTE( "363_hack01.c2", 0x0000001, 0x800000, CRC(024ffd69) SHA1(10282a65a1fac990ede1333a44d225be1b0d6950) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

ROM_START( kf2k5unihack02 ) //kof10thuomg
	ROM_REGION( 0xC00000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "363_hack02.p1",  0x000000, 0x800000, CRC(fcd074e7) SHA1(2b62d16e22726a8d560bc00b678517a3c70b3243) )
	ROM_LOAD16_WORD_SWAP( "363_hack02.p2",  0x800000, 0x400000, CRC(293bbf78) SHA1(647eea522d0a163675205aeab9b0b9ebb3a47eea) )

	NEO_SFIX_128K( "363_hack02.s1", CRC(614db6b7) SHA1(11c65a37e3eb1441a13ce68c6db7367270b0e65b) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "265d.c7",     0x0000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )  // to fix KOF2002 logo
	ROM_LOAD16_BYTE( "265d.c8",     0x0000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )  // to fix KOF2002 logo
	ROM_LOAD16_BYTE( "265omg8.c3", 0x1000000, 0x800000, CRC(2367927c) SHA1(31a5b58feaeb2ecbde5573b48b50cdd8bcaa6ab4) )
	ROM_LOAD16_BYTE( "265omg8.c4", 0x1000001, 0x800000, CRC(4ad48c29) SHA1(9908a7c2d579059a97927d58f4ea95b17deb9952) )
	ROM_LOAD16_BYTE( "365ru.c5",  0x2000000, 0x800000, CRC(11126545) SHA1(4ae58848c8a6701c9cfcd60e6674e8fb7491c0c0) )
	ROM_LOAD16_BYTE( "365ru.c6",  0x2000001, 0x800000, CRC(d225bb9b) SHA1(ee627dab6aeebfb1af9dbdc315a927245243817c) )
	ROM_LOAD16_BYTE( "265omg8.c7", 0x3000000, 0x800000, CRC(890c2a4a) SHA1(04e8720ee62782b8fd6621ef4dc4e7469f57b10c) )
	ROM_LOAD16_BYTE( "265omg8.c8", 0x3000001, 0x800000, CRC(b726009d) SHA1(29e84e7012cd5c109937ea2e6a455ed9829776ac) )
	ROM_COPY("sprites", 0xc6ed00, 0x3c6ed00, 0x3e00)
	ROM_LOAD16_BYTE( "363hc19.c1", 0x0000000, 0x800000, CRC(44cb2a44) SHA1(f02fd816fa526d8b4a2d3f4bfdd2dcfd66e927d9) )
	ROM_LOAD16_BYTE( "363hc19.c2", 0x0000001, 0x800000, CRC(202a8cba) SHA1(6241a2f54e93eb89ea5be1d002ed8aed078cd93e) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof2k8uni )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "366.p1", 0x000000, 0x400000, CRC(966210bf) SHA1(9f86f29dbfe7ac951485908a20a6ce120ebdeb5f) )
	ROM_LOAD16_WORD_SWAP( "366.p2", 0x400000, 0x400000, CRC(ae5f00f7) SHA1(807cbd02cdba320c7cf32c915a663338d7a89252) )

	NEO_SFIX_128K( "366.s1", CRC(e57b5757) SHA1(d3dbb83c3d8b9860361b36496bfdc95220b8b75f) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "366.c1", 0x0000000, 0x400000, CRC(ea774fb8) SHA1(01ce6330716046b591c606b5d016dd5e57fa007b) )
	ROM_LOAD16_BYTE( "366.c2", 0x0000001, 0x400000, CRC(6f497789) SHA1(8c8573ea191d29f0d250901ebf0116a41e184c78) )
	ROM_LOAD16_BYTE( "kf10-c1b.bin", 0x0800000, 0x400000, CRC(b5abfc28) SHA1(eabf60992bb3485c95330065294071ec155bfe7c) )
	ROM_LOAD16_BYTE( "kf10-c2b.bin", 0x0800001, 0x400000, CRC(6cc4c6e1) SHA1(be824a944e745ee18efdc45c81fd496a4d624b9c) )
	ROM_LOAD16_BYTE( "kf10-c3a.bin", 0x1000000, 0x400000, CRC(5b3d4a16) SHA1(93ac1cd7739100f8c32732644f81f2a19837b131) )
	ROM_LOAD16_BYTE( "kf10-c4a.bin", 0x1000001, 0x400000, CRC(c6f3419b) SHA1(340c17a73aeb7bf8a6209f8459e6f00000075b50) )
	ROM_LOAD16_BYTE( "kf10-c3b.bin", 0x1800000, 0x400000, CRC(9d2bba19) SHA1(5ebbd0af3f83a60e33c8ccb743e3d5f5a96f1273) )
	ROM_LOAD16_BYTE( "kf10-c4b.bin", 0x1800001, 0x400000, CRC(5a4050cb) SHA1(8fd2291f349efa1ed5cd37ad4e273b60fe831a77) )
	ROM_LOAD16_BYTE( "kf10-c5a.bin", 0x2000000, 0x400000, CRC(a289d1e1) SHA1(50c7d7ebde6e118a01036cc3e40827fcd9f0d3fd) )
	ROM_LOAD16_BYTE( "kf10-c6a.bin", 0x2000001, 0x400000, CRC(e6494b5d) SHA1(18e064b9867ae0b0794065f8dbefd486620419db) )
	ROM_LOAD16_BYTE( "kf10-c5b.bin", 0x2800000, 0x400000, CRC(404fff02) SHA1(56d1b32c87ea4885e49264e8b21846e465a20e1f) )
	ROM_LOAD16_BYTE( "kf10-c6b.bin", 0x2800001, 0x400000, CRC(f2ccfc9e) SHA1(69db7fac7023785ab94ea711a72dbc2826cfe1a3) )
	ROM_LOAD16_BYTE( "kf10-c7a.bin", 0x3000000, 0x400000, CRC(be79c5a8) SHA1(ded3c5eb3571647f50533eb682c2675372ace3fb) )
	ROM_LOAD16_BYTE( "kf10-c8a.bin", 0x3000001, 0x400000, CRC(a5952ca4) SHA1(76dbb3cb45ce5a4beffa1ed29491204fc6617e42) )
	ROM_LOAD16_BYTE( "366.c7", 0x3800000, 0x400000, CRC(1b6f3bde) SHA1(5e1222197f5a09837930cc34713667dc1644c02a) )
	ROM_LOAD16_BYTE( "366.c8", 0x3800001, 0x400000, CRC(113555e3) SHA1(52b48e182d23422492d95c95de4c86b1d30864e0) )
ROM_END

ROM_START( kof10thu )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "363u.p1", 0x600000, 0x100000, CRC(dd135fb6) SHA1(4e85814a44ee9fee9760048dd6d6fd2b66521bb1) )
	ROM_CONTINUE( 0x100000, 0x500000 )
	ROM_CONTINUE( 0x000000, 0x100000 )
	ROM_CONTINUE( 0x700000, 0x100000 )

	NEO_SFIX_128K( "363u.s1", CRC(b22ba74f) SHA1(3ba9e9d4b95fef9bf35f76e9d4fb6e1a36aeee5a) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "kf10-c1a.bin", 0x0000000, 0x400000, CRC(3bbc0364) SHA1(e8aa7ff82f151ce1db56f259377b64cceef85af0) )
	ROM_LOAD16_BYTE( "kf10-c2a.bin", 0x0000001, 0x400000, CRC(91230075) SHA1(d9098e05a7ba6008661147b6bf8bc2f494b8b72b) )
	ROM_LOAD16_BYTE( "kf10-c1b.bin", 0x0800000, 0x400000, CRC(b5abfc28) SHA1(eabf60992bb3485c95330065294071ec155bfe7c) )
	ROM_LOAD16_BYTE( "kf10-c2b.bin", 0x0800001, 0x400000, CRC(6cc4c6e1) SHA1(be824a944e745ee18efdc45c81fd496a4d624b9c) )
	ROM_LOAD16_BYTE( "kf10-c3a.bin", 0x1000000, 0x400000, CRC(5b3d4a16) SHA1(93ac1cd7739100f8c32732644f81f2a19837b131) )
	ROM_LOAD16_BYTE( "kf10-c4a.bin", 0x1000001, 0x400000, CRC(c6f3419b) SHA1(340c17a73aeb7bf8a6209f8459e6f00000075b50) )
	ROM_LOAD16_BYTE( "kf10-c3b.bin", 0x1800000, 0x400000, CRC(9d2bba19) SHA1(5ebbd0af3f83a60e33c8ccb743e3d5f5a96f1273) )
	ROM_LOAD16_BYTE( "kf10-c4b.bin", 0x1800001, 0x400000, CRC(5a4050cb) SHA1(8fd2291f349efa1ed5cd37ad4e273b60fe831a77) )
	ROM_LOAD16_BYTE( "kf10-c5a.bin", 0x2000000, 0x400000, CRC(a289d1e1) SHA1(50c7d7ebde6e118a01036cc3e40827fcd9f0d3fd) )
	ROM_LOAD16_BYTE( "kf10-c6a.bin", 0x2000001, 0x400000, CRC(e6494b5d) SHA1(18e064b9867ae0b0794065f8dbefd486620419db) )
	ROM_LOAD16_BYTE( "kf10-c5b.bin", 0x2800000, 0x400000, CRC(404fff02) SHA1(56d1b32c87ea4885e49264e8b21846e465a20e1f) )
	ROM_LOAD16_BYTE( "kf10-c6b.bin", 0x2800001, 0x400000, CRC(f2ccfc9e) SHA1(69db7fac7023785ab94ea711a72dbc2826cfe1a3) )
	ROM_LOAD16_BYTE( "kf10-c7a.bin", 0x3000000, 0x400000, CRC(be79c5a8) SHA1(ded3c5eb3571647f50533eb682c2675372ace3fb) )
	ROM_LOAD16_BYTE( "kf10-c8a.bin", 0x3000001, 0x400000, CRC(a5952ca4) SHA1(76dbb3cb45ce5a4beffa1ed29491204fc6617e42) )
	ROM_LOAD16_BYTE( "kf10-c7b.bin", 0x3800000, 0x400000, CRC(3fdb3542) SHA1(7d2050752a2064cd6729f483a0da93808e2c6033) )
	ROM_LOAD16_BYTE( "kf10-c8b.bin", 0x3800001, 0x400000, CRC(661b7a52) SHA1(0ae2ad2389134892f156337332b77adade3ddad1) )
ROM_END

ROM_START( kof10thud )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "363ud.p1", 0x000000, 0x800000, CRC(acbe077a) SHA1(7d3b1ce4e06021f26dd1a7d97b03975f4ed17c24) )

	NEO_SFIX_128K( "363u.s1", CRC(b22ba74f) SHA1(3ba9e9d4b95fef9bf35f76e9d4fb6e1a36aeee5a) )

	NEO_BIOS_AUDIO_128K( "kf10-m1.bin", CRC(f6fab859) SHA1(0184aa1394b9f9946d610278b53b846020dd88dc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "kf10-v1.bin", 0x000000, 0x800000, CRC(0fc9a58d) SHA1(9d79ef00e2c2abd9f29af5521c2fbe5798bf336f) )
	ROM_LOAD( "kf10-v2.bin", 0x800000, 0x800000, CRC(b8c475a4) SHA1(10caf9c69927a223445d2c4b147864c02ce520a8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "363ud.c1", 0x0000000, 0x800000, CRC(2491af91) SHA1(e3e237e50395dc985c564867046ad202bc744d37) )
	ROM_LOAD16_BYTE( "363ud.c2", 0x0000001, 0x800000, CRC(47ff1d91) SHA1(7d6ade9409af2c64114ed5836ccf9f50c7ebac69) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "265d.c7", 0x3000000, 0x800000, CRC(8a5b561c) SHA1(A19697D4C2CC8EDEBC669C95AE1DB4C8C2A70B2C) )
	ROM_LOAD16_BYTE( "265d.c8", 0x3000001, 0x800000, CRC(bef667a3) SHA1(D5E8BC185DCF63343D129C31D2DDAB9F723F1A12) )
ROM_END

 /************************
 Kof Special Edition 2004
***************************/

ROM_START( kof2k4sehack01 ) //kof04omg
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "k2k4s_hack01.p1",  0x000000, 0x3BD4C1, CRC(176576d4) SHA1(adb1529665b918d6628886b712d49c26a5f1eb43))
	ROM_LOAD16_WORD_SWAP( "k2k4s_hack01.p2",  0x100000, 0x400000, CRC(35c8ec2f) SHA1(2587a0452d3929820f0c1507d0657683a9a9b125))

	NEO_SFIX_128K( "365omg.s1", CRC(a41ebb8b) SHA1(e7fde454fe3adfdfa1f43f5a0042979e0f43c89f) )

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000400, "sprites", 0 )
	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "k2k4s_hack01.c4", 0x1000001, 0x880400, CRC(72482b25) SHA1(0ad1aaeaa359a0a35f6be84bdb813559c11ddcd2) )
	ROM_LOAD16_BYTE( "k2k4s_hack01.c5", 0x2000000, 0x800000, CRC(cef18d97) SHA1(8a42837828f8d69854186fe1bbe6acd520414f8e) )
	ROM_LOAD16_BYTE( "k2k4s_hack01.c6", 0x2000001, 0x800000, CRC(cafb6c0a) SHA1(683efa1435c5fc07c357b2530390559980cfb5d9) )
	ROM_LOAD16_BYTE( "k2k4s_hack01.c7", 0x3000000, 0x800000, CRC(13d68c11) SHA1(11de1fb01068fecd2b68c64a49be83e1c7a5c790) )
	ROM_LOAD16_BYTE( "k2k4s_hack01.c8", 0x3000001, 0x800000, CRC(f155bf30) SHA1(c1ac15ae6d3b8a9f719ff819398685ebd5c42450) )
ROM_END

ROM_START( kof2k4sehack02 ) //kofse2k4ap2a
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "k2k4s_hack01.p1",  0x000000, 0x100000, CRC(59c06f53) SHA1(0e2c09f7349911986e3b1af911da719f9b4d0773))
	ROM_LOAD16_WORD_SWAP( "k2k4s_hack01.p2", 0x100000, 0x400000, CRC(b5e8e68e) SHA1(02b8dcb0193ea981e712a6d961767d1d47858909))
	ROM_FILL(0x012510,1,0x65)

	NEO_SFIX_128K( "365mp.s1", CRC(f5f10e6f) SHA1(ab9d390785e03561f6b32f6c66fad2b2e9a5d996))

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( kof2k4mp )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "365mp.p1",  0x000000, 0x100000, CRC(499d739d) SHA1(b6344f49acc68405261592115a3c6c6e298309f5))
	ROM_LOAD16_WORD_SWAP( "365.p3", 0x100000, 0x400000, CRC(e6c50566) SHA1(cc6a3489a3bfeb4dcc65b6ddae0030f7e66fbabe))

	NEO_SFIX_128K( "365mp.s1", CRC(f5f10e6f) SHA1(ab9d390785e03561f6b32f6c66fad2b2e9a5d996))

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000000, "sprites", 0 )

	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

ROM_START( kof2k4mp2 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "365mp2.p1",  0x000000, 0x100000, CRC(50863a7a) SHA1(cf6abfde35b0104c0f1f4be639e7ea4b8127f70f) )
	ROM_LOAD16_WORD_SWAP( "365.p3", 0x100000, 0x400000, CRC(e6c50566) SHA1(cc6a3489a3bfeb4dcc65b6ddae0030f7e66fbabe))

	NEO_SFIX_128K( "365mp.s1", CRC(f5f10e6f) SHA1(ab9d390785e03561f6b32f6c66fad2b2e9a5d996))

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000000, "sprites", 0 )

	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

ROM_START( kof2k4pls )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "365pls.p1", 0x000000, 0x100000, CRC(ed97c684) SHA1(0eb8f18f422252c32186b7daa278c9206e189a1a) )
	ROM_CONTINUE(       0x500000, 0x100000 )
	ROM_LOAD16_WORD_SWAP( "365.p3",  0x400000, 0x100000, CRC(e6c50566) SHA1(cc6a3489a3bfeb4dcc65b6ddae0030f7e66fbabe) )
	ROM_CONTINUE(       0x300000, 0x100000 )
	ROM_CONTINUE(       0x200000, 0x100000 )
	ROM_CONTINUE(       0x100000, 0x100000 )

	NEO_SFIX_128K( "365pls.s1", CRC(cc0ab564) SHA1(83b86f61c2470f9eb4b7d92e5e6274d06f878fdc) )

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "365.v1", 0x000000, 0x800000, CRC(e4ddfb3f) SHA1(eb8220ab01c16cf9244b7f3f9912bec0db561b85) )
	ROM_LOAD( "365.v2", 0x800000, 0x800000, CRC(b887d287) SHA1(f593a5722df6f6fac023d189a739a117e976bb2f) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3",      0x1000000, 0x800000, CRC(959fad0b) SHA1(63ab83ddc5f688dc8165a7ff8d262df3fcd942a2) )
	ROM_LOAD16_BYTE( "265d.c4",      0x1000001, 0x800000, CRC(efe6a468) SHA1(2a414285e48aa948b5b0d4a9333bab083b5fb853) )
	ROM_LOAD16_BYTE( "265d.c5",      0x2000000, 0x800000, CRC(74bba7c6) SHA1(e01adc7a4633bc0951b9b4f09abc07d728e9a2d9) )
	ROM_LOAD16_BYTE( "265d.c6",      0x2000001, 0x800000, CRC(e20d2216) SHA1(5d28eea7b581e780b78f391a8179f1678ee0d9a5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

ROM_START( kof2k4spl )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "365spl.p1",  0x000000, 0x100000, CRC(388c5cba) SHA1(a9dbb3cbe622f8d7c28079883d54665da522dea9))
	ROM_LOAD16_WORD_SWAP( "365.p3",  0x100000, 0x400000, CRC(e6c50566) SHA1(cc6a3489a3bfeb4dcc65b6ddae0030f7e66fbabe))

	NEO_SFIX_128K( "365spl.s1", CRC(348686dd) SHA1(676d83b98cc0b923ef4997c04cecaf74b4ef1687))

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3", 0x1000000, 0x800000, CRC(959fad0b) SHA1(63AB83DDC5F688DC8165A7FF8D262DF3FCD942A2) )
	ROM_LOAD16_BYTE( "265d.c4", 0x1000001, 0x800000, CRC(efe6a468) SHA1(2A414285E48AA948B5B0D4A9333BAB083B5FB853) )
	ROM_LOAD16_BYTE( "265d.c5", 0x2000000, 0x800000, CRC(74bba7c6) SHA1(E01ADC7A4633BC0951B9B4F09ABC07D728E9A2D9) )
	ROM_LOAD16_BYTE( "265d.c6", 0x2000001, 0x800000, CRC(e20d2216) SHA1(5D28EEA7B581E780B78F391A8179F1678EE0D9A5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

ROM_START( kof2k4se1 ) // also known as kofse2k4
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "365se1.p2",    0x000000, 0x100000, CRC(5f482757) SHA1(0c2cba7fc6256e62eb7464306036784703c35a70) )
	ROM_LOAD16_WORD_SWAP( "365.p3", 0x100000, 0x400000, CRC(e6c50566) SHA1(cc6a3489a3bfeb4dcc65b6ddae0030f7e66fbabe) )
	//ROM_LOAD16_WORD_SWAP( "365se2.p1", 0x100000, 0x400000, BAD_DUMP CRC(698679a7) SHA1(2bf3a7a7423141685ecd36c266139a97bac0a69c) )   // total breakage

	NEO_SFIX_128K( "365.s1", CRC(a3c9b2d8) SHA1(1472d2cbd7bb73e84824ecf773924007e6117e77) )

	NEO_BIOS_AUDIO_128K( "365.m1", CRC(5a47d9ad) SHA1(0197737934653acc6c97221660d789e9914f3578) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "265sm.v1", 0x000000, 0x400000, CRC(8991f6e9) SHA1(ce5afc0169c142d7f98111afbc60b3a9a2e17256) )
	ROM_LOAD( "265sm.v2", 0x400000, 0x400000, CRC(511e4d2f) SHA1(28190ec86ac2169ab6fad80635ed13cfcf430c65) )
	ROM_LOAD( "265sm.v3", 0x800000, 0x400000, CRC(1dd8b53a) SHA1(9aa84f145ba07f5f3c3b3fb682eb8a8ab1500b4c) )
	ROM_LOAD( "265sm.v4", 0xc00000, 0x400000, CRC(63bb5462) SHA1(afc6a085e25bd006f26e16beda4244bc8bc19bb8) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "365.c1", 0x0000000, 0x800000, CRC(7a050288) SHA1(55a20c5b01e11a859f096af3f8e09986025d288f) )
	ROM_LOAD16_BYTE( "365.c2", 0x0000001, 0x800000, CRC(e924afcf) SHA1(651e974f7339d2cdcfa58c5398013197a0525b77) )
	ROM_LOAD16_BYTE( "265d.c3",      0x1000000, 0x800000, CRC(959fad0b) SHA1(63ab83ddc5f688dc8165a7ff8d262df3fcd942a2) )
	ROM_LOAD16_BYTE( "265d.c4",      0x1000001, 0x800000, CRC(efe6a468) SHA1(2a414285e48aa948b5b0d4a9333bab083b5fb853) )
	ROM_LOAD16_BYTE( "265d.c5",      0x2000000, 0x800000, CRC(74bba7c6) SHA1(e01adc7a4633bc0951b9b4f09abc07d728e9a2d9) )
	ROM_LOAD16_BYTE( "265d.c6",      0x2000001, 0x800000, CRC(e20d2216) SHA1(5d28eea7b581e780b78f391a8179f1678ee0d9a5) )
	ROM_LOAD16_BYTE( "365.c7", 0x3000000, 0x800000, CRC(fa705b2b) SHA1(f314c66876589601806352484dd8e45bc41be692) )
	ROM_LOAD16_BYTE( "365.c8", 0x3000001, 0x800000, CRC(2c912ff9) SHA1(b624a625ea3e221808b7ea43fb0b1a51d8c1853e) )
ROM_END

 /***************
 Lansquenet 2004
******************/

ROM_START( lans2k4d )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "246lnsq.p1", 0x000000, 0x100000, CRC(724576EA) SHA1(075f5d8ccb7303c8b6bebf630c4fa9c204554044) )
	ROM_LOAD16_WORD_SWAP( "246.p2", 0x100000, 0x400000, CRC(72ea04c3) SHA1(4fb1d22c30f5f3db4637dd92a4d2705c88de399d) )

	NEO_SFIX_128K( "246lnsq.s1", CRC(8699c63a) SHA1(d1a0345bcb4e3f6044c93abd52ff6fe6280dc5ee) )

	NEO_BIOS_AUDIO_128K( "246.m1", CRC(d0604ad1) SHA1(fae3cd52a177eadd5f5775ace957cc0f8301e65d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "246.v1", 0x000000, 0x400000, CRC(16986fc6) SHA1(cff3103dadf2f4390460456a5bd3fb5f28e21f6a) )
	ROM_LOAD( "246.v2", 0x400000, 0x400000, CRC(ada41e83) SHA1(78e37ffaaa5679c8775a3a71f6df7a0d15082bdc) )
	ROM_LOAD( "246.v3", 0x800000, 0x200000, CRC(a05ba5db) SHA1(09d739cad323d918f4196f91b654627fcafd8f4d) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "246.c1", 0x0000000, 0x800000, CRC(47ac9ec5) SHA1(2d9eea11ba87baa23b18a1a3f607dc137846e807) )
	ROM_LOAD16_BYTE( "246.c2", 0x0000001, 0x800000, CRC(7bcab64f) SHA1(08d0edddd14b53d606e9a7a46aa4fb4e7398e0d0) )
	ROM_LOAD16_BYTE( "246lnsq.c3", 0x1000000, 0x800000, CRC(b28d702a) SHA1(21d70c94ecf9ca519b4e50e2d31620f6dfab9ad0) )
	ROM_LOAD16_BYTE( "246lnsq.c4", 0x1000001, 0x800000, CRC(da10dd64) SHA1(55bf0922a117325ef6479f654caf13b287a16f67) )
	ROM_LOAD16_BYTE( "246lnsq.c5", 0x2000000, 0x800000, CRC(bab3b417) SHA1(232f718fecf597434b7a93602387fabab2bda9c0) )
	ROM_LOAD16_BYTE( "246lnsq.c6", 0x2000001, 0x800000, CRC(4c089a28) SHA1(f854ce46b4723aa971de280007268d474d2a6361) )
ROM_END

 /****************
 The Last Blade 1
*******************/

ROM_START( lastbladhack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234_hack01.p1", 0x000000, 0x100000, CRC(cf17f773) SHA1(41d93be04ead541b9193c6bed07a8ecc6f7f3238) )
	ROM_LOAD16_WORD_SWAP( "234.p2", 0x100000, 0x400000, CRC(0fdc289e) SHA1(1ff31c0b0f4f9ddbedaf4bcf927faaae81892ec7) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20a1f4833e5e29ba0073c1712d7a17ab7a2a035c) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234.c5", 0x2000000, 0x400000, CRC(1ba80cee) SHA1(0c59057183b5279b747e73213b4cd3c6d7ad9eb1) )
	ROM_LOAD16_BYTE( "234.c6", 0x2000001, 0x400000, CRC(beafd091) SHA1(55df9cc128eb0f00856de3996c946e3efe8f09a5) )
ROM_END

ROM_START( lastbladhack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234_hack02.p1",    0x000000, 0x100000, CRC(33abcf22) SHA1(d08a8c244f6c57e2a4a39b252828ea718b22da36) )
	ROM_LOAD16_WORD_SWAP( "234.p2", 0x100000, 0x400000, CRC(0fdc289e) SHA1(1ff31c0b0f4f9ddbedaf4bcf927faaae81892ec7) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20A1F4833E5E29BA0073C1712D7A17AB7A2A035C) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234.c5", 0x2000000, 0x400000, CRC(1ba80cee) SHA1(0c59057183b5279b747e73213b4cd3c6d7ad9eb1) )
	ROM_LOAD16_BYTE( "234.c6", 0x2000001, 0x400000, CRC(beafd091) SHA1(55df9cc128eb0f00856de3996c946e3efe8f09a5) )
ROM_END

ROM_START( lastbladhack03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234_hack03.p1", 0x000000, 0x100000, CRC(0244d72b) SHA1(08681bd32c1c169d1e14fc0d9dbaec32d592c1c5) )
	ROM_LOAD16_WORD_SWAP( "234.p2", 0x100000, 0x400000, CRC(0fdc289e) SHA1(1ff31c0b0f4f9ddbedaf4bcf927faaae81892ec7) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20a1f4833e5e29ba0073c1712d7a17ab7a2a035c) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234.c5", 0x2000000, 0x400000, CRC(1ba80cee) SHA1(0c59057183b5279b747e73213b4cd3c6d7ad9eb1) )
	ROM_LOAD16_BYTE( "234.c6", 0x2000001, 0x400000, CRC(beafd091) SHA1(55df9cc128eb0f00856de3996c946e3efe8f09a5) )
ROM_END

ROM_START( lastbladhack04 )
	ROM_REGION( 0x700000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234_hack04.p1",  0x000000, 0x100000, CRC(f8adc621) SHA1(d41921f6e2957b1d1241a5fff099be9764c0efc0) )
	ROM_LOAD16_WORD_SWAP( "234_hack04.p2",  0x100000, 0x600000, CRC(8ff3fb6d) SHA1(dc36bddc086cb8877945f907b81a340bfba72e3c) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "234_hack04.p3", 0x000000, 0x020000, CRC(3c1770df) SHA1(b773dcfcb7656166afe4c6e52219f3083c851460) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20A1F4833E5E29BA0073C1712D7A17AB7A2A035C) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234_hack04.c5",  0x2000000, 0x400000, CRC(4ea22fe0) SHA1(e72d75111d82ee387c5cbfddcaf2e0c13371e455) )
	ROM_LOAD16_BYTE( "234_hack04.c6",  0x2000001, 0x400000, CRC(a863c882) SHA1(92a3baeea3991c4e0d3fd771d3355a379759c4b2) )
ROM_END

ROM_START( lastbladhack05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234_hack05.p1",  0x000000, 0x100000, CRC(abf1ed53) SHA1(70c0117b2bbb8974b4adecfabff242fc4a05f9f3) )
	ROM_LOAD16_WORD_SWAP( "234_hack05.p2",  0x100000, 0x400000, CRC(f80aeee8) SHA1(8a10da46be8ac8fff7ec18940c8bb912bd48d8cd) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20a1f4833e5e29ba0073c1712d7a17ab7a2a035c) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234.c5", 0x2000000, 0x400000, CRC(1ba80cee) SHA1(0c59057183b5279b747e73213b4cd3c6d7ad9eb1) )
	ROM_LOAD16_BYTE( "234.c6", 0x2000001, 0x400000, CRC(beafd091) SHA1(55df9cc128eb0f00856de3996c946e3efe8f09a5) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( lba )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "234a.p1", 0x000000, 0x080000, CRC(3893d38b) SHA1(204d3f61dbeb82b1c942cda7089f24f997925adb) )
	ROM_LOAD16_WORD_SWAP( "234a.p2", 0x080000, 0x080000, CRC(9470c788) SHA1(41671491cef3020b895e42935208e92abec09ed9) )
	ROM_LOAD16_WORD_SWAP( "234a.p3", 0x100000, 0x200000, CRC(b45e631c) SHA1(5c88e6f6c1e343df24f2acf8bb4aed77954f7b36) )
	ROM_LOAD16_WORD_SWAP( "234a.p4", 0x300000, 0x200000, CRC(aa34e149) SHA1(113930343f8efa603823a619964e90f1b06df2ab) )

	NEO_SFIX_128K( "234.s1", CRC(95561412) SHA1(995de272f572fd08d909d3d0af4251b9957b3640) )

	NEO_BIOS_AUDIO_128K( "234.m1", CRC(087628ea) SHA1(48dcf739bb16699af4ab8ed632b7dcb25e470e06) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "234.v1", 0x000000, 0x400000, CRC(ed66b76f) SHA1(8a05ff06d9b6f01c6c16b3026282eaabb0e25b44) )
	ROM_LOAD( "234.v2", 0x400000, 0x400000, CRC(a0e7f6e2) SHA1(753ff74fa9294f695aae511ae01ead119b114a57) )
	ROM_LOAD( "234.v3", 0x800000, 0x400000, CRC(a506e1e2) SHA1(b3e04ba1a5cb50b77c6fbe9fe353b9b64b6f3f74) )
	ROM_LOAD( "234.v4", 0xc00000, 0x400000, CRC(0e34157f) SHA1(20A1F4833E5E29BA0073C1712D7A17AB7A2A035C) )

	ROM_REGION( 0x2800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "234.c1", 0x0000000, 0x800000, CRC(9f7e2bd3) SHA1(2828aca0c0f5802110f10453c1cf640f69736554) )
	ROM_LOAD16_BYTE( "234.c2", 0x0000001, 0x800000, CRC(80623d3c) SHA1(ad460615115ec8fb25206f012da59ecfc8059b64) )
	ROM_LOAD16_BYTE( "234.c3", 0x1000000, 0x800000, CRC(91ab1a30) SHA1(e3cf9133784bef2c8f1bfe45f277ccf82cc6f6a1) )
	ROM_LOAD16_BYTE( "234.c4", 0x1000001, 0x800000, CRC(3d60b037) SHA1(78a50233bcd19e92c7b6f7ee1a53417d9db21f6a) )
	ROM_LOAD16_BYTE( "234.c5", 0x2000000, 0x400000, CRC(1ba80cee) SHA1(0c59057183b5279b747e73213b4cd3c6d7ad9eb1) )
	ROM_LOAD16_BYTE( "234.c6", 0x2000001, 0x400000, CRC(beafd091) SHA1(55df9cc128eb0f00856de3996c946e3efe8f09a5) )
ROM_END

 /****************
 The Last Blade 2
*******************/

ROM_START( lastbld2hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "243_hack01.p1",  0x000000, 0x100000, CRC(f6380375) SHA1(d9bc21138148b5d5b3139f0c3ac663cf83c6e12f) )
	ROM_LOAD16_WORD_SWAP( "243_hack01.p2",  0x100000, 0x400000, CRC(EEF07572) SHA1(ba20c498dbdc2a1d664e5a3aeb3ffcd128c2a248) )

	NEO_SFIX_128K( "243.s1", CRC(c9cd2298) SHA1(a9a18b5347f9dbe29a2ccb63fd4c8fd19537bf8b) )

	NEO_BIOS_AUDIO_128K( "243.m1", CRC(acf12d10) SHA1(6e6b98cc1fa44f24a5168877559b0055e6957b60) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "243.v1", 0x000000, 0x400000, CRC(f7ee6fbb) SHA1(55137bcabeeb590e40a9b8a7c07dd106e4d12a90) )
	ROM_LOAD( "243.v2", 0x400000, 0x400000, CRC(aa9e4df6) SHA1(a0b91f63e2552a8ad9e0d1af00e2c38288637161) )
	ROM_LOAD( "243.v3", 0x800000, 0x400000, CRC(4ac750b2) SHA1(585a154acc67bd84ea5b944686b78ed082b768d9) )
	ROM_LOAD( "243.v4", 0xc00000, 0x400000, CRC(f5c64ba6) SHA1(2eac455def8c27090862cc042f65a3a8aad88283) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "243.c1", 0x0000000, 0x800000, CRC(5839444d) SHA1(0616921c4cce20422563578bd0e806d359508599) )
	ROM_LOAD16_BYTE( "243.c2", 0x0000001, 0x800000, CRC(dd087428) SHA1(ca27fdb60425664956a18c021ea465f452fb1527) )
	ROM_LOAD16_BYTE( "243.c3", 0x1000000, 0x800000, CRC(6054cbe0) SHA1(ec2f65e9c930250ee25fd064ee5ae76a7a9c61d9) )
	ROM_LOAD16_BYTE( "243.c4", 0x1000001, 0x800000, CRC(8bd2a9d2) SHA1(0935df65cd2b0891a708bcc0f1c188148058d4b5) )
	ROM_LOAD16_BYTE( "243.c5", 0x2000000, 0x800000, CRC(6a503dcf) SHA1(23241b16d7e20f923d41186b29487ab922c7f530) )
	ROM_LOAD16_BYTE( "243.c6", 0x2000001, 0x800000, CRC(ec9c36d0) SHA1(e145e9e359000dda6e1dfe95a996bc6d29cfca21) )
ROM_END

ROM_START( lastbld2hack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "243_hack02.p1",  0x000000, 0x100000, CRC(211e0912) SHA1(5bf3b690b2d5da4f0e28bb0caf44736d6e3a4549) )
	ROM_LOAD16_WORD_SWAP( "243.p2", 0x100000, 0x400000, CRC(add4a30b) SHA1(7db62564db49fe0218cbb35b119d62582a24d658) )

	NEO_SFIX_128K( "243.s1", CRC(c9cd2298) SHA1(a9a18b5347f9dbe29a2ccb63fd4c8fd19537bf8b) )

	NEO_BIOS_AUDIO_128K( "243.m1", CRC(acf12d10) SHA1(6e6b98cc1fa44f24a5168877559b0055e6957b60) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "243.v1", 0x000000, 0x400000, CRC(f7ee6fbb) SHA1(55137bcabeeb590e40a9b8a7c07dd106e4d12a90) )
	ROM_LOAD( "243.v2", 0x400000, 0x400000, CRC(aa9e4df6) SHA1(a0b91f63e2552a8ad9e0d1af00e2c38288637161) )
	ROM_LOAD( "243.v3", 0x800000, 0x400000, CRC(4ac750b2) SHA1(585a154acc67bd84ea5b944686b78ed082b768d9) )
	ROM_LOAD( "243.v4", 0xc00000, 0x400000, CRC(f5c64ba6) SHA1(2eac455def8c27090862cc042f65a3a8aad88283) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "243.c1", 0x0000000, 0x800000, CRC(5839444d) SHA1(0616921c4cce20422563578bd0e806d359508599) )
	ROM_LOAD16_BYTE( "243.c2", 0x0000001, 0x800000, CRC(dd087428) SHA1(ca27fdb60425664956a18c021ea465f452fb1527) )
	ROM_LOAD16_BYTE( "243.c3", 0x1000000, 0x800000, CRC(6054cbe0) SHA1(ec2f65e9c930250ee25fd064ee5ae76a7a9c61d9) )
	ROM_LOAD16_BYTE( "243.c4", 0x1000001, 0x800000, CRC(8bd2a9d2) SHA1(0935df65cd2b0891a708bcc0f1c188148058d4b5) )
	ROM_LOAD16_BYTE( "243.c5", 0x2000000, 0x800000, CRC(6a503dcf) SHA1(23241b16d7e20f923d41186b29487ab922c7f530) )
	ROM_LOAD16_BYTE( "243.c6", 0x2000001, 0x800000, CRC(ec9c36d0) SHA1(e145e9e359000dda6e1dfe95a996bc6d29cfca21) )
ROM_END

ROM_START( lastbld2hack03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "243_hack03.p1",  0x000000, 0x100000, CRC(b4820565) SHA1(b3d036e5a320edce4f186b66adb2e4b9e95dc70b) )
	ROM_LOAD16_WORD_SWAP( "243.p2", 0x100000, 0x400000, CRC(add4a30b) SHA1(7db62564db49fe0218cbb35b119d62582a24d658) )

	NEO_SFIX_128K( "243.s1", CRC(c9cd2298) SHA1(a9a18b5347f9dbe29a2ccb63fd4c8fd19537bf8b) )

	NEO_BIOS_AUDIO_128K( "243.m1", CRC(acf12d10) SHA1(6e6b98cc1fa44f24a5168877559b0055e6957b60) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "243.v1", 0x000000, 0x400000, CRC(f7ee6fbb) SHA1(55137bcabeeb590e40a9b8a7c07dd106e4d12a90) )
	ROM_LOAD( "243.v2", 0x400000, 0x400000, CRC(aa9e4df6) SHA1(a0b91f63e2552a8ad9e0d1af00e2c38288637161) )
	ROM_LOAD( "243.v3", 0x800000, 0x400000, CRC(4ac750b2) SHA1(585a154acc67bd84ea5b944686b78ed082b768d9) )
	ROM_LOAD( "243.v4", 0xc00000, 0x400000, CRC(f5c64ba6) SHA1(2eac455def8c27090862cc042f65a3a8aad88283) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "243.c1", 0x0000000, 0x800000, CRC(5839444d) SHA1(0616921c4cce20422563578bd0e806d359508599) )
	ROM_LOAD16_BYTE( "243.c2", 0x0000001, 0x800000, CRC(dd087428) SHA1(ca27fdb60425664956a18c021ea465f452fb1527) )
	ROM_LOAD16_BYTE( "243.c3", 0x1000000, 0x800000, CRC(6054cbe0) SHA1(ec2f65e9c930250ee25fd064ee5ae76a7a9c61d9) )
	ROM_LOAD16_BYTE( "243.c4", 0x1000001, 0x800000, CRC(8bd2a9d2) SHA1(0935df65cd2b0891a708bcc0f1c188148058d4b5) )
	ROM_LOAD16_BYTE( "243.c5", 0x2000000, 0x800000, CRC(6a503dcf) SHA1(23241b16d7e20f923d41186b29487ab922c7f530) )
	ROM_LOAD16_BYTE( "243.c6", 0x2000001, 0x800000, CRC(ec9c36d0) SHA1(e145e9e359000dda6e1dfe95a996bc6d29cfca21) )
ROM_END

ROM_START( lastbld2hack04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "243_hack04.p1", 0x000000, 0x100000, CRC(8f9a24bf) SHA1(f7005ed9ab77d9991babfa72a68a2f535ac01064) )
	ROM_LOAD16_WORD_SWAP( "243_hack04.p2", 0x100000, 0x400000, CRC(37aaffa0) SHA1(4f16686956d2435d6c4339a5c49b352d0a98ecb7) )

	NEO_SFIX_128K( "243.s1", CRC(c9cd2298) SHA1(a9a18b5347f9dbe29a2ccb63fd4c8fd19537bf8b) )

	NEO_BIOS_AUDIO_128K( "243.m1", CRC(acf12d10) SHA1(6e6b98cc1fa44f24a5168877559b0055e6957b60) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "243.v1", 0x000000, 0x400000, CRC(f7ee6fbb) SHA1(55137bcabeeb590e40a9b8a7c07dd106e4d12a90) )
	ROM_LOAD( "243.v2", 0x400000, 0x400000, CRC(aa9e4df6) SHA1(a0b91f63e2552a8ad9e0d1af00e2c38288637161) )
	ROM_LOAD( "243.v3", 0x800000, 0x400000, CRC(4ac750b2) SHA1(585a154acc67bd84ea5b944686b78ed082b768d9) )
	ROM_LOAD( "243.v4", 0xc00000, 0x400000, CRC(f5c64ba6) SHA1(2eac455def8c27090862cc042f65a3a8aad88283) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "243.c1", 0x0000000, 0x800000, CRC(5839444d) SHA1(0616921c4cce20422563578bd0e806d359508599) )
	ROM_LOAD16_BYTE( "243.c2", 0x0000001, 0x800000, CRC(dd087428) SHA1(ca27fdb60425664956a18c021ea465f452fb1527) )
	ROM_LOAD16_BYTE( "243.c3", 0x1000000, 0x800000, CRC(6054cbe0) SHA1(ec2f65e9c930250ee25fd064ee5ae76a7a9c61d9) )
	ROM_LOAD16_BYTE( "243.c4", 0x1000001, 0x800000, CRC(8bd2a9d2) SHA1(0935df65cd2b0891a708bcc0f1c188148058d4b5) )
	ROM_LOAD16_BYTE( "243.c5", 0x2000000, 0x800000, CRC(6a503dcf) SHA1(23241b16d7e20f923d41186b29487ab922c7f530) )
	ROM_LOAD16_BYTE( "243.c6", 0x2000001, 0x800000, CRC(ec9c36d0) SHA1(e145e9e359000dda6e1dfe95a996bc6d29cfca21) )
ROM_END

 /***********
 Last Resort
**************/

ROM_START( lresorthack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "024_hack01.p1", 0x000000, 0x080000, CRC(78c4e582) SHA1(5b802797ad0916a1ccac316a661120dc4dce0b74) )

	NEO_SFIX_128K( "024.s1", CRC(5cef5cc6) SHA1(9ec305007bdb356e9f8f279beae5e2bcb3f2cf7b) )

	NEO_BIOS_AUDIO_128K( "024.m1", CRC(cec19742) SHA1(ab6c6ba7737e68d2420a0617719c6d4c89039c45) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "024.v1", 0x000000, 0x100000, CRC(efdfa063) SHA1(e4609ecbcc1c820758f229da5145f51285b50555) )
	ROM_LOAD( "024.v2", 0x100000, 0x100000, CRC(3c7997c0) SHA1(8cb7e8e69892b19d318978370dbc510d51b06a69) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "024.c1", 0x000000, 0x100000, CRC(3617c2dc) SHA1(8de2643a618272f8aa1c705363edb007f4a5f5b7) )
	ROM_LOAD16_BYTE( "024.c2", 0x000001, 0x100000, CRC(3f0a7fd8) SHA1(d0c9c7a9dde9ce175fb243d33ec11fa719d0158c) )
	ROM_LOAD16_BYTE( "024.c3", 0x200000, 0x080000, CRC(e9f745f8) SHA1(bbe6141da28b0db7bf5cf321d69b7e613e2414d7) )
	ROM_LOAD16_BYTE( "024.c4", 0x200001, 0x080000, CRC(7382fefb) SHA1(e916dec5bb5462eb9ae9711f08c7388937abb980) )
ROM_END

 /****************
 Magical Drop III
*******************/

ROM_START( magdrop3hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "233_hack01.p1", 0x000000, 0x100000, CRC(80bfe2a9) SHA1(12caa131ccdc6e1a54914440022556921fab8a5e) )

	NEO_SFIX_128K( "233.s1", CRC(7399e68a) SHA1(b535ee56a0f0995f04674e676f6aa636ffad26aa) )

	NEO_BIOS_AUDIO_128K( "233.m1", CRC(5beaf34e) SHA1(2905d26945932cddc2dd3a1dc5abba8aa3baee14) )

	ROM_REGION( 0x480000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "233.v1", 0x000000, 0x400000, CRC(58839298) SHA1(18cae7bba997c52780761cbf119c4e4b34397a61) )
	ROM_LOAD( "233.v2", 0x400000, 0x080000, CRC(d5e30df4) SHA1(bbbc0ff5b975471bd682f85976ac4a93f6d44f2e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "233.c1", 0x000000, 0x400000, CRC(65e3f4c4) SHA1(a6deb75d802225327f8f1c2733a7f2b47e722e59) )
	ROM_LOAD16_BYTE( "233.c2", 0x000001, 0x400000, CRC(35dea6c9) SHA1(ea133bf947f950236f49d0ae0d1a9af3bc1a9a50) )
	ROM_LOAD16_BYTE( "233.c3", 0x800000, 0x400000, CRC(0ba2c502) SHA1(8e0f1e553aef04758aaaa14d5115f0ecace4391e) )
	ROM_LOAD16_BYTE( "233.c4", 0x800001, 0x400000, CRC(70dbbd6d) SHA1(32dd6a04c6329e89f4878e7a56f0d172a6388eea) )
ROM_END

ROM_START( magdrop3hack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "233_hack02.p1", 0x000000, 0x100000, CRC(ed90bb10) SHA1(eb22b362475ef294307357abf6c31a1ff2229f4a) )

	NEO_SFIX_128K( "233.s1", CRC(7399e68a) SHA1(b535ee56a0f0995f04674e676f6aa636ffad26aa) )

	NEO_BIOS_AUDIO_128K( "233.m1", CRC(5beaf34e) SHA1(2905d26945932cddc2dd3a1dc5abba8aa3baee14) )

	ROM_REGION( 0x480000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "233.v1", 0x000000, 0x400000, CRC(58839298) SHA1(18cae7bba997c52780761cbf119c4e4b34397a61) )
	ROM_LOAD( "233.v2", 0x400000, 0x080000, CRC(d5e30df4) SHA1(bbbc0ff5b975471bd682f85976ac4a93f6d44f2e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "233_hack02.c1",  0x000000, 0x400000, CRC(b10223f2) SHA1(d99565751bdde47cae4c5bb5fac32a0b80bae2b9) )
	ROM_LOAD16_BYTE( "233_hack02.c2",  0x000001, 0x400000, CRC(39c0f4b8) SHA1(a09c20c12d418e938ce84ea87d3d8bbb3007563c) )
	ROM_LOAD16_BYTE( "233.c3", 0x800000, 0x400000, CRC(0ba2c502) SHA1(8e0f1e553aef04758aaaa14d5115f0ecace4391e) )
	ROM_LOAD16_BYTE( "233.c4", 0x800001, 0x400000, CRC(70dbbd6d) SHA1(32dd6a04c6329e89f4878e7a56f0d172a6388eea) )
ROM_END

/**************
 Magician Lord
****************/

ROM_START( maglordhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "005_hack01.p1", 0x000000, 0x080000, CRC(8d3e68d1) SHA1(9f1b5d78138feb9d372a7b7788cb2b52f3fb8c5a) )

	NEO_SFIX_128K( "005.s1", CRC(1c5369a2) SHA1(db0dba0a7dced6c9ca929c5abda491b05d84199c) )

	NEO_BIOS_AUDIO_256K( "005.m1", CRC(26259f0f) SHA1(4f3e500093d61585048767dbd9fa09b3911a05d6) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "005.v11", 0x000000, 0x080000, CRC(cc0455fd) SHA1(a8ff4270e7705e263d25ff0b301f503bccea7e59) )

	ROM_REGION( 0x100000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "005.v21", 0x000000, 0x080000, CRC(f94ab5b7) SHA1(2c16985102e3585e08622d8c54ac5c60425b9ff8) )
	ROM_LOAD( "005.v22", 0x080000, 0x080000, CRC(232cfd04) SHA1(61b66a9decbbd1f500a8c186615e7fd077c6861e) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "005.c1", 0x000000, 0x80000, CRC(806aee34) SHA1(3c32a0edbbddb694495b510c13979c44b83de8bc) )
	ROM_LOAD16_BYTE( "005.c2", 0x000001, 0x80000, CRC(34aa9a86) SHA1(cec97e1ff7f91158040c629ba75742db82c4ae5e) )
	ROM_LOAD16_BYTE( "005.c3", 0x100000, 0x80000, CRC(c4c2b926) SHA1(478bfafca21f5a1338808251a06ab405e6a9e65f) )
	ROM_LOAD16_BYTE( "005.c4", 0x100001, 0x80000, CRC(9c46dcf4) SHA1(4c05f3dc25777a87578ce09a6cefb3a4cebf3266) )
	ROM_LOAD16_BYTE( "005.c5", 0x200000, 0x80000, CRC(69086dec) SHA1(7fa47f4a765948813ebf366168275dcc3c42e951) )
	ROM_LOAD16_BYTE( "005.c6", 0x200001, 0x80000, CRC(ab7ac142) SHA1(e6ad2843947d35d8e913d2666f87946c1ba7944f) )
ROM_END

/***********
 Matrimelee
*************/

ROM_START( matrimhack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266_hack01.p1",   0x000000, 0x100000, CRC(5f7b6942) SHA1(35fe5d59407b79725698fa62a870518ecc930b34) )
	ROM_LOAD16_WORD_SWAP( "266.p2", 0x100000, 0x400000, CRC(a14b1906) SHA1(1daa14d73512f760ef569b06f9facb279437d1db) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "266.m1", CRC(456c3e6c) SHA1(5a07d0186198a18d2dda1331093cf29b0b9b2984) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266.v1", 0x000000, 0x800000, CRC(a4f83690) SHA1(200d683d8c30ebc6d0ed331aa5bbba39b4e07160) )
	ROM_LOAD( "266.v2", 0x800000, 0x800000, CRC(d0f69eda) SHA1(9d7e98976ad433ed8a35d7afffa38130444ba7db) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266.c1", 0x0000000, 0x800000, CRC(505f4e30) SHA1(f22b6f76fc0cad963555dc89d072967c8dc8b79a) )
	ROM_LOAD16_BYTE( "266.c2", 0x0000001, 0x800000, CRC(3cb57482) SHA1(dab15bc24391f9a5173de76af48b612fb9636ccf) )
	ROM_LOAD16_BYTE( "266.c3", 0x1000000, 0x800000, CRC(f1cc6ad0) SHA1(66c1cccc0332ffd2d3064f06330c41f95ca09ced) )
	ROM_LOAD16_BYTE( "266.c4", 0x1000001, 0x800000, CRC(45b806b7) SHA1(c2bb866fded53d62fad0fc88d89d5e7d4cb1894f) )
	ROM_LOAD16_BYTE( "266.c5", 0x2000000, 0x800000, CRC(9a15dd6b) SHA1(194a6973a7a9e3847efe1bdbaeaeb16e74aff2dd) )
	ROM_LOAD16_BYTE( "266.c6", 0x2000001, 0x800000, CRC(281cb939) SHA1(bdb7766cfde581ccfaee2be7fe48445f360a2301) )
	ROM_LOAD16_BYTE( "266.c7", 0x3000000, 0x800000, CRC(4b71f780) SHA1(d5611a6f6b730db58613b48f2b0174661ccfb7bb) )
	ROM_LOAD16_BYTE( "266.c8", 0x3000001, 0x800000, CRC(29873d33) SHA1(dc77f129ed49b8d40d0d4241feef3f6c2f19a987) )
ROM_END

ROM_START( matrimhack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266_hack02.p1",  0x000000, 0x100000, CRC(cd86e143) SHA1(64189aeabc5ec0f38bc286b6c88a57b30d3f8c30) )
	ROM_LOAD16_WORD_SWAP( "266.p2", 0x100000, 0x400000, CRC(a14b1906) SHA1(1daa14d73512f760ef569b06f9facb279437d1db) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "266.m1", CRC(456c3e6c) SHA1(5a07d0186198a18d2dda1331093cf29b0b9b2984) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266.v1", 0x000000, 0x800000, CRC(a4f83690) SHA1(200d683d8c30ebc6d0ed331aa5bbba39b4e07160) )
	ROM_LOAD( "266.v2", 0x800000, 0x800000, CRC(d0f69eda) SHA1(9d7e98976ad433ed8a35d7afffa38130444ba7db) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266.c1", 0x0000000, 0x800000, CRC(505f4e30) SHA1(f22b6f76fc0cad963555dc89d072967c8dc8b79a) )
	ROM_LOAD16_BYTE( "266.c2", 0x0000001, 0x800000, CRC(3cb57482) SHA1(dab15bc24391f9a5173de76af48b612fb9636ccf) )
	ROM_LOAD16_BYTE( "266.c3", 0x1000000, 0x800000, CRC(f1cc6ad0) SHA1(66c1cccc0332ffd2d3064f06330c41f95ca09ced) )
	ROM_LOAD16_BYTE( "266.c4", 0x1000001, 0x800000, CRC(45b806b7) SHA1(c2bb866fded53d62fad0fc88d89d5e7d4cb1894f) )
	ROM_LOAD16_BYTE( "266.c5", 0x2000000, 0x800000, CRC(9a15dd6b) SHA1(194a6973a7a9e3847efe1bdbaeaeb16e74aff2dd) )
	ROM_LOAD16_BYTE( "266.c6", 0x2000001, 0x800000, CRC(281cb939) SHA1(bdb7766cfde581ccfaee2be7fe48445f360a2301) )
	ROM_LOAD16_BYTE( "266.c7", 0x3000000, 0x800000, CRC(4b71f780) SHA1(d5611a6f6b730db58613b48f2b0174661ccfb7bb) )
	ROM_LOAD16_BYTE( "266.c8", 0x3000001, 0x800000, CRC(29873d33) SHA1(dc77f129ed49b8d40d0d4241feef3f6c2f19a987) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( matrimd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266.p1",  0x000000, 0x100000, CRC(5d4c2dc7) SHA1(8d723b0d28ec344eef26009b361a2b97d300dd51) )
	ROM_LOAD16_WORD_SWAP( "266.p2", 0x100000, 0x400000, CRC(a14b1906) SHA1(1daa14d73512f760ef569b06f9facb279437d1db) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "266d.m1", CRC(d2f3742d) SHA1(dd42f8a301f07e079fbc2bfd23e788baa03ca72d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266.v1", 0x000000, 0x800000, CRC(a4f83690) SHA1(200d683d8c30ebc6d0ed331aa5bbba39b4e07160) )
	ROM_LOAD( "266.v2", 0x800000, 0x800000, CRC(d0f69eda) SHA1(9d7e98976ad433ed8a35d7afffa38130444ba7db) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266d.c1", 0x0000000, 0x800000, CRC(0ab54c54) SHA1(31bca460462b8545b3e5264804987ab951e9b202) )
	ROM_LOAD16_BYTE( "266d.c2", 0x0000001, 0x800000, CRC(b834dd89) SHA1(579470839a8d23b2d22cf727d2b43c6c3c113e1f) )
	ROM_LOAD16_BYTE( "266d.c3", 0x1000000, 0x800000, CRC(f31d68d4) SHA1(a6b8aaf11d25f871b52e9626fe0e4ae0683c3c52) )
	ROM_LOAD16_BYTE( "266d.c4", 0x1000001, 0x800000, CRC(58cfb6e9) SHA1(106875db996ec29b0254b34f9d935c761cde20c4) )
	ROM_LOAD16_BYTE( "266d.c5", 0x2000000, 0x800000, CRC(f3c0e3fd) SHA1(3a2f14517617fb98a202ceec88d52891e1438c8e) )
	ROM_LOAD16_BYTE( "266d.c6", 0x2000001, 0x800000, CRC(b53c8dcf) SHA1(9f4b0bac92262e8c1c1055a70f13c7c326367f73) )
	ROM_LOAD16_BYTE( "266d.c7", 0x3000000, 0x800000, CRC(3c1fb02a) SHA1(43e170e6d48c01d8c323f9309f97e6038bab20d7) )
	ROM_LOAD16_BYTE( "266d.c8", 0x3000001, 0x800000, CRC(5ee31f80) SHA1(3cdd8037f5004df92f4c19ba0746c8bccb4e2473) )
ROM_END

ROM_START( matrima )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266a.p1", 0x000000, 0x100000, CRC(1e59c746) SHA1(9a6cdb6c56d5a80e373bb5ec476f1e7c3b3fbba9) )
	ROM_LOAD16_WORD_SWAP( "266a.p2", 0x100000, 0x400000, CRC(b0132cef) SHA1(3780db1b8947a8371a46b67ca1a14a12bf97986b) )

	NEO_SFIX_128K( "266a.s1", CRC(aff2a3be) SHA1(57aba048573eacf8fbdd228f04a7f30da64cebe5) )

	NEO_BIOS_AUDIO_128K( "266d.m1", CRC(d2f3742d) SHA1(dd42f8a301f07e079fbc2bfd23e788baa03ca72d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266a.v1", 0x000000, 0x400000, CRC(352b0a07) SHA1(19f7cc12f3f6d0fda9c7449816c4c32367447897) )
	ROM_LOAD( "266a.v2", 0x400000, 0x400000, CRC(945f8831) SHA1(6ac9bd41e0bb33601c82ebeb2aa5b1b8b91fada1) )
	ROM_LOAD( "266a.v3", 0x800000, 0x400000, CRC(e8362fcc) SHA1(42d558fd80cabe22a1c09a1fa75741afbcf46b7c) )
	ROM_LOAD( "266a.v4", 0xc00000, 0x400000, CRC(393c9a62) SHA1(0eaee0007557e59e64edcdbca229e6f776de9aef) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266a.c1", 0x0000000, 0x800000, CRC(f2240352) SHA1(3bb6763ebbc33db172d413276213eb3f2c7093c0) )
	ROM_LOAD16_BYTE( "266a.c2", 0x0000001, 0x800000, CRC(338a9142) SHA1(eb589b4141c394ca78c2f5eb1b510cde982f3225) )
	ROM_LOAD16_BYTE( "266a.c3", 0x1000000, 0x800000, CRC(6233412d) SHA1(b473cddc0a4343dffc4e35df68cd1bf9b163bd64) )
	ROM_LOAD16_BYTE( "266a.c4", 0x1000001, 0x800000, CRC(304a2d9f) SHA1(29d2e50a64476640f4bff68227d6b1fdb45530c1) )
	ROM_LOAD16_BYTE( "266a.c5", 0x2000000, 0x800000, CRC(18382650) SHA1(df10fb0b43f0dabdc57b55e923536b6c215678ff) )
	ROM_LOAD16_BYTE( "266a.c6", 0x2000001, 0x800000, CRC(c5e7a840) SHA1(a1336c5c1284221d0b420bb44dd7c462d3e83ca7) )
	ROM_LOAD16_BYTE( "266a.c7", 0x3000000, 0x800000, CRC(bfaa985c) SHA1(3ff7d56d8caaa1a34d0dcbc16bfc97df4afeab48) )
	ROM_LOAD16_BYTE( "266a.c8", 0x3000001, 0x800000, CRC(efaf3f3c) SHA1(9374eb39fe30ba679878729992f136fdce1d92fc) )
ROM_END

ROM_START( matrimeh )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266eh.p1", 0x000000, 0x100000, CRC(e3b05e78) SHA1(8cbf3fe47c8afedb4ea7dcf5809acf7e5e858aba) )
	ROM_LOAD16_WORD_SWAP( "266a.p2",  0x100000, 0x400000, CRC(b0132cef) SHA1(3780db1b8947a8371a46b67ca1a14a12bf97986b) )

	NEO_SFIX( 0x40000, "266eh.s1", CRC(31c02ff8) SHA1(f4096d0b39a2fd682c0b3be435098ed8de990f8b) )

	NEO_BIOS_AUDIO_128K( "266eh.m1", CRC(f692d84b) SHA1(a3737c574e779e0330916a2eea6ad5b3839f9538) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266nd.v1", 0x000000, 0x800000, CRC(328b2194) SHA1(6734da4f1e882a8524c04ff375567ed96a2dae5e) )
	ROM_LOAD( "266nd.v2", 0x800000, 0x800000, CRC(2a13562d) SHA1(dc13f1af6afaf43f2ac20b7cf1c751a047450312) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266d.c1", 0x0000000, 0x800000, CRC(0ab54c54) SHA1(31bca460462b8545b3e5264804987ab951e9b202) )
	ROM_LOAD16_BYTE( "266d.c2", 0x0000001, 0x800000, CRC(b834dd89) SHA1(579470839a8d23b2d22cf727d2b43c6c3c113e1f) )
	ROM_LOAD16_BYTE( "266d.c3", 0x1000000, 0x800000, CRC(f31d68d4) SHA1(a6b8aaf11d25f871b52e9626fe0e4ae0683c3c52) )
	ROM_LOAD16_BYTE( "266d.c4", 0x1000001, 0x800000, CRC(58cfb6e9) SHA1(106875db996ec29b0254b34f9d935c761cde20c4) )
	ROM_LOAD16_BYTE( "266d.c5", 0x2000000, 0x800000, CRC(f3c0e3fd) SHA1(3a2f14517617fb98a202ceec88d52891e1438c8e) )
	ROM_LOAD16_BYTE( "266d.c6", 0x2000001, 0x800000, CRC(b53c8dcf) SHA1(9f4b0bac92262e8c1c1055a70f13c7c326367f73) )
	ROM_LOAD16_BYTE( "266d.c7", 0x3000000, 0x800000, CRC(3c1fb02a) SHA1(43e170e6d48c01d8c323f9309f97e6038bab20d7) )
	ROM_LOAD16_BYTE( "266d.c8", 0x3000001, 0x800000, CRC(5ee31f80) SHA1(3cdd8037f5004df92f4c19ba0746c8bccb4e2473) )
ROM_END

ROM_START( matrimnd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "266nd.p1", 0x000000, 0x100000, CRC(d07b4032) SHA1(d9878ac13c8c374d496317359038b4a53c6f1872) )
	ROM_LOAD16_WORD_SWAP( "266a.p2",  0x100000, 0x400000, CRC(b0132cef) SHA1(3780db1b8947a8371a46b67ca1a14a12bf97986b) )

	NEO_SFIX_128K( "266a.s1", CRC(aff2a3be) SHA1(57aba048573eacf8fbdd228f04a7f30da64cebe5) )

	NEO_BIOS_AUDIO_128K( "266eh.m1", CRC(f692d84b) SHA1(a3737c574e779e0330916a2eea6ad5b3839f9538) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "266nd.v1", 0x000000, 0x800000, CRC(328b2194) SHA1(6734da4f1e882a8524c04ff375567ed96a2dae5e) )
	ROM_LOAD( "266nd.v2", 0x800000, 0x800000, CRC(2a13562d) SHA1(dc13f1af6afaf43f2ac20b7cf1c751a047450312) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "266d.c1", 0x0000000, 0x800000, CRC(0ab54c54) SHA1(31bca460462b8545b3e5264804987ab951e9b202) )
	ROM_LOAD16_BYTE( "266d.c2", 0x0000001, 0x800000, CRC(b834dd89) SHA1(579470839a8d23b2d22cf727d2b43c6c3c113e1f) )
	ROM_LOAD16_BYTE( "266d.c3", 0x1000000, 0x800000, CRC(f31d68d4) SHA1(a6b8aaf11d25f871b52e9626fe0e4ae0683c3c52) )
	ROM_LOAD16_BYTE( "266d.c4", 0x1000001, 0x800000, CRC(58cfb6e9) SHA1(106875db996ec29b0254b34f9d935c761cde20c4) )
	ROM_LOAD16_BYTE( "266d.c5", 0x2000000, 0x800000, CRC(f3c0e3fd) SHA1(3a2f14517617fb98a202ceec88d52891e1438c8e) )
	ROM_LOAD16_BYTE( "266d.c6", 0x2000001, 0x800000, CRC(b53c8dcf) SHA1(9f4b0bac92262e8c1c1055a70f13c7c326367f73) )
	ROM_LOAD16_BYTE( "266d.c7", 0x3000000, 0x800000, CRC(3c1fb02a) SHA1(43e170e6d48c01d8c323f9309f97e6038bab20d7) )
	ROM_LOAD16_BYTE( "266d.c8", 0x3000001, 0x800000, CRC(5ee31f80) SHA1(3cdd8037f5004df92f4c19ba0746c8bccb4e2473) )
ROM_END

 /**********************
 Money Puzzle Exchanger
**************************/

ROM_START( miexchnghack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "231_hack01.p1", 0x000000, 0x80000, CRC(a107101b) SHA1(f1a165779a4a26d7c9067b649fdcb7da40f3aea6) )

	NEO_SFIX_128K( "231.s1", CRC(fe0c0c53) SHA1(54d56d4463db193e504658f4f6f4997a62ae3d95) )

	NEO_BIOS_AUDIO_128K( "231.m1", CRC(de41301b) SHA1(59ce3836ac8f064d56a446c9374f05bcb40fcfd8) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "231.v1", 0x000000, 0x400000, CRC(113fb898) SHA1(9168ba90c4aa969f69eb11ba3f4d76592d81e05a) )

	ROM_REGION( 0x500000, "sprites", 0 )
	ROM_LOAD16_BYTE( "231_hack01.c1", 0x000000, 0x200000, CRC(0764d51c) SHA1(06a63359e5859d30ae5a8e2beed5a7615563298a) )
	ROM_LOAD16_BYTE( "231_hack01.c2", 0x000001, 0x200000, CRC(6a2acd38) SHA1(bc35e7f9898ed6d7fd9ce43d4a4be91a81d0bd04) )
	ROM_LOAD16_BYTE( "231_hack01.c3", 0x400000, 0x080000, CRC(14524eb5) SHA1(e090b99a1ee2cca4a7f7d5262e764d1cab6bada2) )
	ROM_LOAD16_BYTE( "231_hack01.c4", 0x400001, 0x080000, CRC(1694f171) SHA1(a6af5d3f1e0b3f73da275e04d4434c6c9c7b9208) )
ROM_END

/***********
 Metal Slug
*************/

ROM_START( mslughacks01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks01.p1", 0x100000, 0x100000, CRC(61e1ba6e) SHA1(aafc5b39a61d330b95f29c663900fbc02b01bd1d) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks02.p1", 0x100000, 0x100000, CRC(2750fe1b) SHA1(0de9e1cf728cdf21fb6ed243637576644e786fc8) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks03.p1", 0x100000, 0x100000, CRC(2b3d433f) SHA1(94a1a88bff5f1b6bdf8d8a0406004982ef9d7b0c) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks04 ) //mslugdg
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks04.p1", 0x100000, 0x100000, CRC(0a739521) SHA1(74e637a6a77140f8ce1128cb8f456ecae0a7a7ef) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks05 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks05.p1", 0x100000, 0x100000, CRC(582b4d77) SHA1(9de833c621d3b26153e3b22c86d6137beb58e5e4) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks06 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks06.p1", 0x100000, 0x100000, CRC(4d6af5bd) SHA1(2c8d782dba605d9148d8e519c803be2f14145642) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks07 ) //mslugunity
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks07.p1", 0x100000, 0x100000, CRC(a3186dfd) SHA1(9241e8bf40b878f2372d8da9f008c8895a87394b) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks08 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks08.p1", 0x100000, 0x100000, CRC(47ae2445) SHA1(615aeb5ed8f7e0197ed599b3f20eaed88ab1086d) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslughacks09 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_hacks09.p1", 0x100000, 0x100000, CRC(09888e87) SHA1(d61adb16503c30b3290ea41b3955607b7ebba70f) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( msboot )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201.p1", 0x100000, 0x100000, CRC(08d8daa5) SHA1(b888993dbb7e9f0a28a01d7d2e1da00ef9cf6f38) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUGB_YMSND
	
    ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( msluger01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_er01.p1", 0x100000, 0x100000, CRC(3b965025) SHA1(503756f70ccda65fe81d26ebe17319f0fe574bc7) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUG_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslugla01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_la01.p1", 0x100000, 0x100000, CRC(70ca457c) SHA1(a5ae3651f21e5061762f0f21ff5659732a3de9f4) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUGRM_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

ROM_START( mslugrmxe0s01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "201_mxe1s01.p1", 0x100000, 0x100000, CRC(07829c7d) SHA1(d90be05a617d9c3e3f966ca2d5b22c216e447f28) )
	ROM_CONTINUE( 0x000000, 0x100000 )
    MSLUGRMXE_MAINCPU

	NEO_SFIX_128K( "201.s1", CRC(2f55958d) SHA1(550b53628daec9f1e1e11a398854092d90f9505a) )

	NEO_BIOS_AUDIO_128K( "201.m1", CRC(c28b3253) SHA1(fd75bd15aed30266a8b3775f276f997af57d1c06) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG_YMSND

	ROM_REGION( 0x1000000, "sprites", 0 )
    MSLUG_SPRITES
ROM_END

/**************
 Metal Slug 2 
****************/

ROM_START( mslug2hacks01 ) //mslug2r
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks01.p1", 0x000000, 0x100000, CRC(f882d50d) SHA1(77fa3169a5631de0a89ef3170bfbe2b45294e8cf) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks02 ) //mslug21v2
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks02.p1", 0x000000, 0x100000, CRC(c3efed6c) SHA1(ae11a5abf75548271bf26acb645e8b0e5fc22eaa) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks03.p1", 0x000000, 0x100000, CRC(f680d167) SHA1(ebeed513de07928679ac4083fd705af51e21078d) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks04 ) //mslug2fr
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks04.p1", 0x000000, 0x100000, CRC(d1b10ed0) SHA1(db992fa93d1afb4adea30d63903fcd7d0e6ce9dd) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2hacks05 ) //mslug2t
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks05.p1", 0x000000, 0x100000, CRC(df5d6fbc) SHA1(b9cc3e29afc12dc98daac9afb4f94e2cdd8b455c) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks06 ) //mslug2sh
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks06.p1", 0x000000, 0x100000, CRC(4e4c8d1d) SHA1(bdb30f68fa1345511cbe1afcdf04c5f59d8965a1) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks07 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks07.p1", 0x000000, 0x100000, CRC(300d800b) SHA1(3896179061b4025d717cdcc213699a0a9d6183ab) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks08 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks08.p1", 0x000000, 0x100000, CRC(882310fb) SHA1(1ed0d6d3a6d37c95cbc0012bf55d32fc75b4d827) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks09 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks09.p1", 0x000000, 0x100000, CRC(7080ed64) SHA1(f028273a636f3d82481cbb5325657acf59c3b455) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks10 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks10.p1", 0x000000, 0x100000, CRC(6d309ec1) SHA1(a9554d41f8d5bd96530ca441e5b68a3fa1ecb2c8) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks11 ) //mslug2dg
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks11.p1", 0x000000, 0x100000, CRC(00c455e7) SHA1(8a20c06a5f92032c0dc1ebd1e8a2709abd725330) )
	ROM_LOAD16_WORD_SWAP( "241_hacks11.p2", 0x100000, 0x200000, CRC(1bf6b12a) SHA1(bdf1cee93c7cf1e57f61797ed4f176fe2a6ebf73) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks12 ) //mslug2ct
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks12.p1", 0x000000, 0x100000, CRC(78ad6864) SHA1(d84b0b3fc5991d2ab9d04712d4df3b4318e65923) )
	ROM_LOAD16_WORD_SWAP( "241_hacks12.p2", 0x100000, 0x200000, CRC(fe36f353) SHA1(61df20c7cd9f904552ec672dec11b679122bc5fe) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks13 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks13.p1", 0x000000, 0x100000, CRC(2061e49c) SHA1(F2f6069dc8bc3f10b46baf1a9fa19117fe340417) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks14 )
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks14.p1",  0x000000, 0x100000, CRC(aab0c2a7) SHA1(8501f543ec2a26a870195ead3dd531133390f203) )
	ROM_LOAD16_WORD_SWAP( "241_hacks22.p2",  0x100000, 0x300000, CRC(abf37360) SHA1(09347f0502fba4965dc14fd6db89bb3cabdeea1f) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks22.s1", CRC(d983067f) SHA1(b9da5e7e2064bf11baa601773413192c3694710a) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUG2EG_SPRITES
ROM_END

ROM_START( mslug2hacks15 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks15.p1", 0x000000, 0x100000, CRC(34233c1f) SHA1(eb137f71bbbdd06672cbb761bc37d39bc06d3d86) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks16 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks16.p1", 0x000000, 0x100000, CRC(f565b5c5) SHA1(5dba416c97dbd0ff679a1544bc9c43eee3f1c71a) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks17 ) //mslug2fm
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks17.p1", 0x000000, 0x100000, CRC(487173f6) SHA1(7d86f25ea5b99df0fff94954a79793e7b7098f94) )
	ROM_LOAD16_WORD_SWAP( "241_hacks17.p2", 0x100000, 0x200000, CRC(37a118fc) SHA1(37c913c3d6736362ad83b8aaadf94b9112669a52) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks18 ) //mslug2unity
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks18.p1", 0x000000, 0x100000, CRC(1562cf23) SHA1(dfa2d7ea10f9eac6cd3b1a7a2f2c2f867edc28d1) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks19 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241.p1", 0x000000, 0x100000, CRC(2a53c5da) SHA1(5a6aba482cac588a6c2c51179c95b487c6e11899) )
	ROM_LOAD16_WORD_SWAP( "241.p2", 0x100000, 0x200000, CRC(38883f44) SHA1(fcf34b8c6e37774741542393b963635412484a27) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241_hacks19.m1", CRC(aa3f889e) SHA1(c0c51d5f9fbbe96aeca9a352a9b2da42102c3399) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks20 ) //mslug2f1
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks20.p1",  0x000000, 0x100000, CRC(56f2c248) SHA1(688685dc3703885279d2e3f95538272474ee3e08) )
	ROM_LOAD16_WORD_SWAP( "241_hacks17.p2",  0x100000, 0x200000, CRC(37a118fc) SHA1(37c913c3d6736362ad83b8aaadf94b9112669a52) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2hacks21 ) //mslug2dd
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks21.p1",  0x000000, 0x100000, CRC(da22fd32) SHA1(7ecbf665f3d6af1b8f658e22f8df32463d3b9e2d) )
	ROM_LOAD16_WORD_SWAP( "241_hacks21.p2",  0x100000, 0x200000, CRC(89adbf97) SHA1(98b5382d51c7fd0b5ad52dd9649ef847a163a78a) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2DD_SPRITES
ROM_END

ROM_START( mslug2hacks22 ) //mslug2eg
	ROM_REGION( 0x400000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_hacks22.p1",  0x000000, 0x100000, CRC(bb312904) SHA1(08766b1ffb8686aa4d07370dc825c9f09a100fa6) )
	ROM_LOAD16_WORD_SWAP( "241_hacks22.p2",  0x100000, 0x300000, CRC(abf37360) SHA1(09347f0502fba4965dc14fd6db89bb3cabdeea1f) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks22.s1", CRC(d983067f) SHA1(b9da5e7e2064bf11baa601773413192c3694710a) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUG2EG_SPRITES
ROM_END

ROM_START( mslug2er01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_er01.p1", 0x000000, 0x100000, CRC(cc1802f5) SHA1(ed2198d3d0390f597a91b35938ec2fac9bbf289e) )
	ROM_LOAD16_WORD_SWAP( "241_er01.p2", 0x100000, 0x200000, CRC(120b31f0) SHA1(7f76db3a9ff1a515d06cc59a814e4b1b4049baed) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2er02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_er02.p1", 0x000000, 0x100000, CRC(bb25d215) SHA1(e035e63d51d9c55fdf8d4ac31ade41a6b24ce6ef) )
	ROM_LOAD16_WORD_SWAP( "241_er02.p2", 0x100000, 0x200000, CRC(19c995b2) SHA1(76cdd0d7d7cb0888646b3689e71fdb6066c0271a) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2er03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_er03.p1", 0x000000, 0x100000, CRC(c0fa4a7b) SHA1(e5d484d93aaf8fa25bfab280a4c6eb47e9daef8f) )
	ROM_LOAD16_WORD_SWAP( "241_er03.p2", 0x100000, 0x200000, CRC(fc09d7cb) SHA1(4730b18a518029a1736b57d7ab4bb72653404085) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2at01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_at01.p1",     0x000000, 0x100000, CRC(0f8db35d) SHA1(ae65513586703ef373fcef45368549404e8422f0) )
	ROM_LOAD16_WORD_SWAP( "241_at01.p2",     0x100000, 0x200000, CRC(83931ab6) SHA1(bd5b527180b3a01eda89d862c08f5414d538fab9) )
    MSLUG2_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2la01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_la01.p1", 0x000000, 0x100000, CRC(43d90a8f) SHA1(28e3a19f3e312fec7c01335949c665183dfea787) )
	ROM_LOAD16_WORD_SWAP( "241_la01.p2", 0x100000, 0x200000, CRC(3a55eb87) SHA1(b82c41988f3d6752bd5de78eef254f924785f6a1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2la02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_la02.p1", 0x000000, 0x100000, CRC(0c8d3fe2) SHA1(7b6e686eb01ea36ea9a2d8d67f3863c85b9b31a4) )
	ROM_LOAD16_WORD_SWAP( "241_la02.p2", 0x100000, 0x200000, CRC(192b65e9) SHA1(40b8380306f96a95abe6b638eb786400655535b1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2la03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_la03.p1", 0x000000, 0x100000, CRC(15299911) SHA1(413427710210a263c93ab985a455832d2b4b5400) )
	ROM_LOAD16_WORD_SWAP( "241_la03.p2", 0x100000, 0x200000, CRC(fceb2790) SHA1(6ed39a7b521ee32df92ac7ec580734128b8ad5ee) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2la04 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_la04.p1", 0x000000, 0x100000, CRC(dfb83ffb) SHA1(1c88d3690cd8886226e0a87b34f6ffb97ee2a8f8) )
	ROM_LOAD16_WORD_SWAP( "241_la03.p2", 0x100000, 0x200000, CRC(fceb2790) SHA1(6ed39a7b521ee32df92ac7ec580734128b8ad5ee) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2la05 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_la05.p1", 0x000000, 0x100000, CRC(ec3b7d94) SHA1(68934a1748acac8d6f6376fb3615c7bf95c0b283) )
	ROM_LOAD16_WORD_SWAP( "241_la01.p2", 0x100000, 0x200000, CRC(3a55eb87) SHA1(b82c41988f3d6752bd5de78eef254f924785f6a1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2lb01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_lb01.p1", 0x000000, 0x100000, CRC(67737321) SHA1(3cd316d7b2eb08943010b2b24973516c1af7112e) )
	ROM_LOAD16_WORD_SWAP( "241_la01.p2", 0x100000, 0x200000, CRC(3a55eb87) SHA1(b82c41988f3d6752bd5de78eef254f924785f6a1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2lb02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_lb02.p1", 0x000000, 0x100000, CRC(d9200f50) SHA1(fa1f6c0f94130517f513f86a8fe30961c6509bb4) )
	ROM_LOAD16_WORD_SWAP( "241_la02.p2", 0x100000, 0x200000, CRC(192b65e9) SHA1(40b8380306f96a95abe6b638eb786400655535b1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2lb03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_lb03.p1", 0x000000, 0x100000, CRC(3183e0bf) SHA1(9a098652664ea707d2764056fcbfe0e96df1ef95) )
	ROM_LOAD16_WORD_SWAP( "241_la03.p2", 0x100000, 0x200000, CRC(fceb2790) SHA1(6ed39a7b521ee32df92ac7ec580734128b8ad5ee) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2lb04 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_lb04.p1", 0x000000, 0x100000, CRC(9f2b60ed) SHA1(824e3ef25ad6b50c4ce05db4ba99ca378426749d) )
	ROM_LOAD16_WORD_SWAP( "241_la03.p2", 0x100000, 0x200000, CRC(fceb2790) SHA1(6ed39a7b521ee32df92ac7ec580734128b8ad5ee) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2lb05 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_lb05.p1", 0x000000, 0x100000, CRC(d6e6ef92) SHA1(8d601ca57168e20b5e19a06fe9d801cb1cbab44a) )
	ROM_LOAD16_WORD_SWAP( "241_la01.p2", 0x100000, 0x200000, CRC(3a55eb87) SHA1(b82c41988f3d6752bd5de78eef254f924785f6a1) )
    MSLUG2RM_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe0s01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s01.p1", 0x000000, 0x100000, CRC(3084b681) SHA1(30e504d52baa6dafaed99d78e895be7b9a062a18) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s01.p2", 0x100000, 0x200000, CRC(287f541b) SHA1(3f30846026edc499b0c6126f117e1cb859976dd4) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe0s02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s02.p1", 0x000000, 0x100000, CRC(3888fd3e) SHA1(fdb44079e3d77385a662ad1125d622cf95d7cb26) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s02.p2", 0x100000, 0x200000, CRC(0b01da75) SHA1(f7b658fddae967a0ddd3a1f71235824c62bc53de) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe0s03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s03.p1", 0x000000, 0x100000, CRC(9904a213) SHA1(9af7eb0ea16ce82cd3498ae547143def4800f09a) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s03.p2", 0x100000, 0x200000, CRC(eec1980c) SHA1(557529010fba8ed728aa0a89474d0af3b9fd2b50) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2rmxe0s04 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s04.p1", 0x000000, 0x100000, CRC(3f3b5d28) SHA1(051b2d2ccb333de7f4808e3de39bd487991c28eb) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s03.p2", 0x100000, 0x200000, CRC(eec1980c) SHA1(557529010fba8ed728aa0a89474d0af3b9fd2b50) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2rmxe0s05 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s05.p1", 0x000000, 0x100000, CRC(8ce50c6b) SHA1(1374d95d71abfbece81115b2a9d932765ce39fe2) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s01.p2", 0x100000, 0x200000, CRC(287f541b) SHA1(3f30846026edc499b0c6126f117e1cb859976dd4) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe1s01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe1s01.p1", 0x000000, 0x100000, CRC(142ecf2f) SHA1(b30a3d7dd0370e0a627c7262ea0d5491ac31358e) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s01.p2", 0x100000, 0x200000, CRC(287f541b) SHA1(3f30846026edc499b0c6126f117e1cb859976dd4) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
	MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe1s02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe1s02.p1", 0x000000, 0x100000, CRC(ed25cd8c) SHA1(1615799f7931bb8480763ee21e6763906359d929) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s02.p2", 0x100000, 0x200000, CRC(0b01da75) SHA1(f7b658fddae967a0ddd3a1f71235824c62bc53de) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2FR_SPRITES
ROM_END

ROM_START( mslug2rmxe1s03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe1s03.p1", 0x000000, 0x100000, CRC(bdaedbbd) SHA1(295d326c684707449e3aa791e612b9b97228dc01) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s03.p2", 0x100000, 0x200000, CRC(eec1980c) SHA1(557529010fba8ed728aa0a89474d0af3b9fd2b50) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2rmxe1s04 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe1s04.p1", 0x000000, 0x100000, CRC(05e6cf2e) SHA1(ce9f9172d5f3fa00dc6c87dde105db416fde2fc9) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s03.p2", 0x100000, 0x200000, CRC(eec1980c) SHA1(557529010fba8ed728aa0a89474d0af3b9fd2b50) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241.s1", CRC(f3d32f0f) SHA1(2dc38b7dfd3ff14f64d5c0733c510b6bb8c692d0) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2_SPRITES
ROM_END

ROM_START( mslug2rmxe1s05 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "241_rmxe1s05.p1", 0x000000, 0x100000, CRC(b6389e6d) SHA1(9c4db358f9714b733e0ed2a186aac8ce73e97777) )
	ROM_LOAD16_WORD_SWAP( "241_rmxe0s01.p2", 0x100000, 0x200000, CRC(287f541b) SHA1(3f30846026edc499b0c6126f117e1cb859976dd4) )
    MSLUG2RMXE_MAINCPU

	NEO_SFIX_128K( "241_hacks04.s1", CRC(e22801a7) SHA1(24120adfbc5e35efcea98cfb30a936b6630da7c5) )

	NEO_BIOS_AUDIO_128K( "241.m1", CRC(94520ebd) SHA1(f8a1551cebcb91e416f30f50581feed7f72899e9) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
    MSLUG2_YMSND

	ROM_REGION( 0x2000000, "sprites", 0 )
    MSLUG2FR_SPRITES
ROM_END

 /************
 Metal Slug 3
***************/

ROM_START( mslug3hacks01 ) //mslg3eha
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks01.p1", 0x000000, 0x100000, CRC(e04c6624) SHA1(de684f346ab9a2e5730a6753de6d53801a702cc8) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks02.p1", 0x000000, 0x100000, CRC(b981e587) SHA1(7e193b1e73fe4e921d5809f942df2da41f34d177) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks03.p1", 0x000000, 0x100000, CRC(2591b1f7) SHA1(3af19aad19d741a342d83bf20c564d8294a8daca) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MSLUG( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks04.p1", 0x000000, 0x100000, CRC(248123f3) SHA1(0a99b5f69766240883a98cef00a6f917462eaf9f) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks05.p1", 0x000000, 0x100000, CRC(a92df7c4) SHA1(19ed3ee71e63d8648c057718c38654a064f8942d) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks06 ) //mslug3se
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks06.p1", 0x000000, 0x100000, CRC(46330db5) SHA1(7d1df5cb86da40b11465e6e017670365cbe234ac) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hacks07 ) //mslug3eb
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma_hacks07.sma", 0x0c0000, 0x040000, CRC(2a9935c2) SHA1(f39d47b0b6fbe0e741777d47fbaf37008c4e0599) )
	ROM_LOAD16_WORD_SWAP( "256_hacks07.p1", 0x100000, 0x400000, CRC(6ec2e58d) SHA1(d83dd522a5c8a8b4bd32722967e3d934d2e73e08) )
	ROM_LOAD16_WORD_SWAP( "256_hacks07.p2", 0x500000, 0x400000, CRC(ca4eb0d3) SHA1(eafd000ae27c0b904aea50f681f98ab67068da20) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks08 ) //mslug3sd
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "256_hacks08.p1", 0x000000, 0x100000, CRC(e1e21cc4) SHA1(c70ef18dfe8edbadadff6004508e838b246b88a5) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "256_hacks08.p1", 0x000000, 0x100000, CRC(0d697262) SHA1(278fe0d916618707d9268fae2c306e2d090a0779) )
	ROM_LOAD16_WORD_SWAP( "256_hacks08.p2", 0x100000, 0x400000, CRC(7343335b) SHA1(2d694af0c876eb8b4844e918cb707ce011c61c5d) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks09 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks09.p1", 0x000000, 0x100000, CRC(6e9e2cdd) SHA1(3ed78c9d6adcf605432ecb2905fd39bba7356f08) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks10 ) //mslug3zh
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks10.p1", 0x000000, 0x100000, CRC(257fa6b9) SHA1(c9ccc0a42a5a34a08316a76dc977ccee74d91245) )
	ROM_LOAD16_WORD_SWAP( "256_hacks10.p2", 0x100000, 0x400000, CRC(badc753c) SHA1(60eae0e02c05448c33cde2666a8b565ee6835216) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks11 ) //mslug3se
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks11.p1", 0x000000, 0x100000, CRC(4e6753ee) SHA1(2df01bda8ed450761c5ed24a9ef1a4dfe324ab08) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks12 ) //mslug3fr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks12.p1", 0x000000, 0x100000, CRC(5716aac8) SHA1(749cec11d0e8cb8406afc386f97af101947b7389) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU
	
	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3hacks13 ) // mslug3gw
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "256_hacks13.p1",   0x000000, 0x100000, CRC(fafde0b4) SHA1(3eee501ddebda761d93c49cdea135fdf0aa0dc9c) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "256_hacks13.p1", 0x000000, 0x100000, CRC(fb23d70d) SHA1(b3589a37aa92dd73babe69a4292f6ba3449c619b) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256_hacks17.m1", CRC(c84d22f2) SHA1(069d5eafa859acbebc35394344df78ea84055cf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3GW_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hacks14 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks14.p1", 0x000000, 0x100000, CRC(9c2063d2) SHA1(ded2a73388d9f50ade36c04625d62cd5ec26a244) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks15 ) //mslug3v
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks15.p1", 0x000000, 0x100000, CRC(47f9aeea) SHA1(db1c65681ac12b146e2fa21a8f28000f41a11fe2) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MSLUG( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks16 )  // mslug3c+eb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p1", 0x000000, 0x100000, CRC(e575a406) SHA1(fcfab75f53c13353fcdd85293dee5bd71f614bb0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p2", 0x100000, 0x400000, CRC(64ae37fc) SHA1(9a82b407f29986138a109d4620fc0a113049047a) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks17 ) //mslug3es
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks17.p1", 0x000000, 0x100000, CRC(caac7e33) SHA1(098505f23e3a56e2e0cc0ae855e2457061138c9e) )
	ROM_LOAD16_WORD_SWAP( "256_hacks17.p2", 0x100000, 0x400000, CRC(62e699b3) SHA1(eac088b21d2243198034a9796aa0815d686ccfac) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256_hacks17.m1", CRC(c84d22f2) SHA1(069d5eafa859acbebc35394344df78ea84055cf7) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3GW_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hacks18 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks18.p1", 0x000000, 0x100000, CRC(ea7d570f) SHA1(d2419b96347f75fdd386880bc5eaa5766fe07b0c) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks19 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks19.p1", 0x000000, 0x100000, CRC(3a80d207) SHA1(e1ac055d2316ba754678d119890742643a29bc40) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks20 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p1", 0x000000, 0x100000, CRC(e575a406) SHA1(fcfab75f53c13353fcdd85293dee5bd71f614bb0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks20.p2", 0x100000, 0x400000, CRC(d53d178c) SHA1(98f0cd74b18d0d63b74fd6d3830548f4c2ce401b) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks21 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p1", 0x000000, 0x100000, CRC(e575a406) SHA1(fcfab75f53c13353fcdd85293dee5bd71f614bb0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks21.p2", 0x100000, 0x400000, CRC(f55ccb93) SHA1(c52e83c848ba63ca0323aa99f85e8278b6fe4cda) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks22 ) //mslug3lw
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p1", 0x000000, 0x100000, CRC(e575a406) SHA1(fcfab75f53c13353fcdd85293dee5bd71f614bb0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks22.p2", 0x100000, 0x400000, CRC(7eed7c81) SHA1(1d9a18178b14226be60af683c9c4a7cfada7f0bb) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks23 ) //mslug3i
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks23.p1", 0x000000, 0x100000, CRC(6ea79265) SHA1(1930a3ff1260d56fdc7559bd5bb56f0ca5479588) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks24 ) //mslug3c
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "256_hacks24.p1", 0x000000, 0x100000, CRC(05c99714) SHA1(da2e5b959d9327bce7b244629abaa8784af767a6) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "256_hacks16.p1", 0x000000, 0x100000, CRC(e575a406) SHA1(fcfab75f53c13353fcdd85293dee5bd71f614bb0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks24.p2", 0x100000, 0x400000, CRC(b948a472) SHA1(e0135911f7d7e21285e6965cc6b4446277eea405) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks25 ) //mslug3hacksr, mslug3nsj
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "256_hacks25.p1", 0x000000, 0x100000, CRC(f804b927) SHA1(a1a6bcc0fe65416c3ab04f84513da2138515fed0) ) //CRC32 SHA1 Official Rom Hack
	//ROM_LOAD16_WORD_SWAP( "256_hacks25.p2", 0x100000, 0x400000, CRC(94dcc314) SHA1(9d3990eef3ca3d46981495b83729479045963f51) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "256_hacks25.p1", 0x000000, 0x100000, CRC(22e48b3d) SHA1(1a35a0b9dac0000e01dabb369a6a7cadb6d733c2) )
	ROM_LOAD16_WORD_SWAP( "256_hacks25.p2", 0x100000, 0x400000, CRC(c34322ae) SHA1(774cc4e4e6fb4596c3e4daf18e7d8b6be2e43ea7) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hacks26 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "256_hacks26.p2", 0x100000, 0x400000, CRC(02b6dfbe) SHA1(a1a12d3da272732f128e31d474d47ac25cf701e7) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "256_hacks26.p1", 0x000000, 0x100000, CRC(80398884) SHA1(4564d952b2a7cbecad1a143e7568ec065b2f05bb) )
	ROM_LOAD16_WORD_SWAP( "256_hacks26.p2", 0x100000, 0x400000, CRC(55293e04) SHA1(ec018fa657a12f1445bc9824cabfe429e39fa857) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hacks27 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks27.p1", 0x000000, 0x100000, CRC(3a13832f) SHA1(2ade55c2104aaaff0ad23e662188ecbb4cf7ecf9) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks28 ) //mslug3g
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks28.p1", 0x000000, 0x100000, CRC(b23bd9b7) SHA1(4a5e877bc0d4853dc9c5a2c179049fbdd5285239) )
	ROM_LOAD16_WORD_SWAP( "256_hacks28.p2", 0x100000, 0x400000, CRC(8053a3fb) SHA1(baf40ca915d30f3dbf6cc440131e824e889940e7) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3hacks29 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks29.p1", 0x000000, 0x100000, CRC(bb448975) SHA1(3b9565ad6f2468ee95b953161d1701c88c9568e5) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks30 ) //mslug31v2
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma_hacks30.sma", 0x0c0000, 0x040000, CRC(1c090500) SHA1(d4ca78c9848418ff89a3d756051051a6c1125b52) )
	ROM_LOAD16_WORD_SWAP( "256.p1", 0x100000, 0x400000, CRC(b07edfd5) SHA1(dcbd9e500bfae98d754e55cdbbbbf9401013f8ee) )
	ROM_LOAD16_WORD_SWAP( "256_hacks30.p2", 0x500000, 0x400000, CRC(45db3f1d) SHA1(3d07c5f0eb3914acb443e151048318e498be2186) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks31 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks31.p1", 0x000000, 0x100000, CRC(ec09383d) SHA1(6574976d540b776c6c1db0dbacc9c9e084207324) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks32 ) //mslug3unity
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma_hacks26.sma", 0x0c0000, 0x040000, CRC(3c6c536b) SHA1(e841b56e5151cf3118b7ac7f5e1495ed51378031) )
	ROM_LOAD16_WORD_SWAP( "256.p1", 0x100000, 0x400000, CRC(b07edfd5) SHA1(dcbd9e500bfae98d754e55cdbbbbf9401013f8ee) )
	ROM_LOAD16_WORD_SWAP( "256_hacks32.p2", 0x500000, 0x400000, CRC(ea0c22b6) SHA1(564bd4609eb273a5eb7e6269ca3650fc0e1f1446) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks33 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks33.p1", 0x000000, 0x100000, CRC(bcfde666) SHA1(243a960000f77da5fbf79de0f9b14d6f78dbcdfe) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks34 ) //mslug3i
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "neo-sma_hacks34.sma", 0x0c0000, 0x040000, CRC(3a631f79) SHA1(47dff1a509f15a1115547453992e6d10f86851a1) )
	ROM_LOAD16_WORD_SWAP( "256.p1", 0x100000, 0x400000, CRC(b07edfd5) SHA1(dcbd9e500bfae98d754e55cdbbbbf9401013f8ee) )
	ROM_LOAD16_WORD_SWAP( "256_hacks34.p2", 0x500000, 0x400000, CRC(3a040bcc) SHA1(509f7308ab5ecff1b3d925798667e9c59981310a) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3_SPRITES
ROM_END

ROM_START( mslug3hacks35 ) //mslug3dd
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks35.p1", 0x000000, 0x100000, CRC(9a83cb7a) SHA1(07f2c4602d7c4cc011a5271d39af92a329fbcff0) )
	ROM_LOAD16_WORD_SWAP( "256_hacks35.p2", 0x100000, 0x400000, CRC(e82fc07a) SHA1(3fa3da5aff90229a1bbd636f761d4eb4a66958d8) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3DD_SPRITES
ROM_END

ROM_START( mslug3hacks36 ) //mslug3ki
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_hacks36.p1", 0x000000, 0x100000, CRC(16a83b0a) SHA1(a898890bcd13d194333271c0d445913ba8d2cda7) )
	ROM_LOAD16_WORD_SWAP( "256_hacks36.p2", 0x100000, 0x400000, CRC(57f01937) SHA1(da9e106619c2fb264a2ba492b78828468a7265d6) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3DD_SPRITES
ROM_END

/**********************
 Decrypted And Bootleg
************************/

ROM_START( mslug3d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256hd.p1", 0x000000, 0x100000, CRC(9c42ca85) SHA1(7a8f77a89867b889295ae9b9dfd4ba28f02d234d) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3hd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256hd.p1", 0x000000, 0x100000, CRC(9c42ca85) SHA1(7a8f77a89867b889295ae9b9dfd4ba28f02d234d) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3nd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256nd.p1", 0x000000, 0x100000, CRC(c871c036) SHA1(4d1cca29dc27300d52739bee183fa4edf3d36afd) )
	ROM_LOAD16_WORD_SWAP( "256h.p2", 0x100000, 0x400000, CRC(1f3d8ce8) SHA1(08b05a8abfb86ec09a5e758d6273acf1489961f9) )
    MSLUG3H_MAINCPU

	NEO_SFIX_512K( "263nd.s1", CRC(2628d663) SHA1(a976bdac2d7908e3203c7ffdc6a103cd327c656e) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3D_SPRITES
ROM_END

/****************************************************
 Metal Slug 6" is a hack/bootleg of Metal Slug 3
*******************************************************/

ROM_START( mslug6hacks01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "299_hacks01.p1", 0x000000, 0x200000, CRC(6d89096c) SHA1(97c436657512576a954690385c58f08f6510f655) )
	ROM_LOAD16_WORD_SWAP( "299_hacks01.p2", 0x100000, 0x400000, CRC(bf4a80af) SHA1(514f97892375a636b84fb8388261eea645a8da1a) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3N6P_SPRITES
ROM_END

ROM_START( mslug6hacks02 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "299_hacks01.p1", 0x000000, 0x200000, CRC(6d89096c) SHA1(97c436657512576a954690385c58f08f6510f655) )
	ROM_LOAD16_WORD_SWAP( "299_hacks02.p2", 0x100000, 0x400000, CRC(78ef585c) SHA1(89c1d918902d3d2d15d289be3066c35600a331eb) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3N6P_SPRITES
ROM_END

ROM_START( mslug6hacks03 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "299_hacks01.p1", 0x000000, 0x200000, CRC(6d89096c) SHA1(97c436657512576a954690385c58f08f6510f655) )
	ROM_LOAD16_WORD_SWAP( "299_hacks03.p2", 0x100000, 0x400000, CRC(d33f3351) SHA1(e76fa46dcbade4c86f1c7e89b86e75e9c0d6b3b2) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3N6P_SPRITES
ROM_END

ROM_START( mslug6hacks04 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "299_hacks01.p1", 0x000000, 0x200000, CRC(6d89096c) SHA1(97c436657512576a954690385c58f08f6510f655) )
	ROM_LOAD16_WORD_SWAP( "299_hacks04.p2", 0x100000, 0x400000, CRC(f35eef4e) SHA1(569867c16142bbcb4153bf437407d9d266a3bffc) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3N6P_SPRITES
ROM_END

ROM_START( mslug6hacks05 ) //mslug3n6p
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "299_hacks05.p1", 0x000000, 0x100000, CRC(1f1079a2) SHA1(5d99358f0e513e2087c7594bab09c8d33db40f1f) )
	ROM_LOAD16_WORD_SWAP( "299_hacks05.p2", 0x100000, 0x400000, CRC(62ac1321) SHA1(49462d899cc4d5006f0d6bc8735e9aa04d36f7ed) )
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "299-s1.bin", CRC(6f8b9635) SHA1(86b0c8c0ccac913c6192ed6a96c35d4e1a5e8061) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3N6P_SPRITES
ROM_END

 /**************
 Only exclusive
*****************/
 /*********************
  Version Remastering
*************************/

ROM_START( mslug3er01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er01.p1", 0x000000, 0x100000, CRC(2a041686) SHA1(de3232f7fd7d1e513b45c2300f77289acf3cd66b) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er02.p1", 0x000000, 0x100000, CRC(21d927f4) SHA1(0b880ebb63e84459e6053ea8ef009132d4fd03c4) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er03.p1", 0x000000, 0x100000, CRC(b9dae7e5) SHA1(87d3264a4fd11ead8cddbf767b4e4ba403e9a6b2) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er04.p1", 0x000000, 0x100000, CRC(6e68ba55) SHA1(ae345d2476745574b17576959d8feafac163fa22) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er05.p1", 0x000000, 0x100000, CRC(a7138759) SHA1(f4ffbf250bf98b6e450df3fffb67d672e83a21c8) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er06 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er06.p1", 0x000000, 0x100000, CRC(7a09cf8e) SHA1(0976d854e56b7502cafb51766f660944f73a0da6) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3er07 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_er07.p1", 0x000000, 0x100000, CRC(8f30a9d0) SHA1(7a54b637394199723b835e78f277605a8b37fcec) )
	ROM_LOAD16_WORD_SWAP( "256_er01.p2", 0x100000, 0x400000, CRC(f2feb36f) SHA1(45308efb107f4ae744f88c2e8e8f74ab0f9b2b0d) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3H_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

 /*******************************
 Revised Remix Standard Edition
***********************************/

ROM_START( mslug3la01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_la01.p1", 0x000000, 0x100000, CRC(f97319d7) SHA1(2bd8c0d52810f9b89be255a452cf39bed5cda756) )
	ROM_LOAD16_WORD_SWAP( "256_la01.p2", 0x100000, 0x400000, CRC(f9123ba1) SHA1(86e1647ebde622445813b3b09a86bdc5c09b465c) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3la02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_la02.p1", 0x000000, 0x100000, CRC(78641e8d) SHA1(988b1d8cb7c84a583116ae2c377dc778e7edb0e3) )
	ROM_LOAD16_WORD_SWAP( "256_la02.p2", 0x100000, 0x400000, CRC(6ea68fd8) SHA1(5caefc3c950fd5c2cd8a24315cdac8f5404d0d74) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3la03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_la03.p1", 0x000000, 0x100000, CRC(8993dbb3) SHA1(71a83ee9d5e83bf68fc97e183513656d0096eddd) )
	ROM_LOAD16_WORD_SWAP( "256_la02.p2", 0x100000, 0x400000, CRC(6ea68fd8) SHA1(5caefc3c950fd5c2cd8a24315cdac8f5404d0d74) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3la04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_la04.p1", 0x000000, 0x100000, CRC(02c6e4f1) SHA1(900cdd93ea7d531d8a0b5c46e57b454a94793f11) )
	ROM_LOAD16_WORD_SWAP( "256_la02.p2", 0x100000, 0x400000, CRC(6ea68fd8) SHA1(5caefc3c950fd5c2cd8a24315cdac8f5404d0d74) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3la05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_la05.p1", 0x000000, 0x100000, CRC(6894d604) SHA1(49f7974f1f926724aaa6a6c51e41c7f6ed942b52) )
	ROM_LOAD16_WORD_SWAP( "256_la02.p2", 0x100000, 0x400000, CRC(6ea68fd8) SHA1(5caefc3c950fd5c2cd8a24315cdac8f5404d0d74) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lb01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lb01.p1", 0x000000, 0x100000, CRC(c3e2c30e) SHA1(fc1941ac00e4f864c061a9b0002357a7400644c0) )
	ROM_LOAD16_WORD_SWAP( "256_lb01.p2", 0x100000, 0x400000, CRC(b1662b21) SHA1(0c9e46276f3e61b87e402a7b010c6efbf48bd34c) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lb02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lb02.p1", 0x000000, 0x100000, CRC(42f5c454) SHA1(6204f03da9da324f6f2e1712f287b24769f91ee8) )
	ROM_LOAD16_WORD_SWAP( "256_lb02.p2", 0x100000, 0x400000, CRC(cffda8f2) SHA1(ef9f0fc13fa34aa09d6ec0ff52dfd92627c09f03) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lb03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lb03.p1", 0x000000, 0x100000, CRC(b302016a) SHA1(321de3e90dd98bfad39ddff02b20dbf0f78fe7a2) )
	ROM_LOAD16_WORD_SWAP( "256_lb02.p2", 0x100000, 0x400000, CRC(cffda8f2) SHA1(ef9f0fc13fa34aa09d6ec0ff52dfd92627c09f03) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lb04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lb04.p1", 0x000000, 0x100000, CRC(38573e28) SHA1(6c227a51d83eeddb633cbd072215753ca5473349) )
	ROM_LOAD16_WORD_SWAP( "256_lb02.p2", 0x100000, 0x400000, CRC(cffda8f2) SHA1(ef9f0fc13fa34aa09d6ec0ff52dfd92627c09f03) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lb05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lb05.p1", 0x000000, 0x100000, CRC(52050cdd) SHA1(b3a2aa846a358db16588d2817d2d325feb1eea34) )
	ROM_LOAD16_WORD_SWAP( "256_lb02.p2", 0x100000, 0x400000, CRC(cffda8f2) SHA1(ef9f0fc13fa34aa09d6ec0ff52dfd92627c09f03) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lc01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lc01.p1", 0x000000, 0x100000, CRC(c83ff27c) SHA1(6721ff5d463b59dfcf725d1e009d30dc64cb5bde) )
	ROM_LOAD16_WORD_SWAP( "256_lc01.p2", 0x100000, 0x400000, CRC(5cfe69c7) SHA1(e77caa4b5c52b5780bf5afbcc376b6465a32de55) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lc02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lc02.p1", 0x000000, 0x100000, CRC(4928f526) SHA1(20873f68408824ae8161f3a3f16f035c31aef1d1) )
	ROM_LOAD16_WORD_SWAP( "256_lc02.p2", 0x100000, 0x400000, CRC(2265ea14) SHA1(db84d5cbc756a13fb1f6286acf3db4d7a3a7d6f4) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lc03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lc03.p1", 0x000000, 0x100000, CRC(b8df3018) SHA1(76b7a80834a5cf394957d00b673ad8cb6ac9d972) )
	ROM_LOAD16_WORD_SWAP( "256_lc02.p2", 0x100000, 0x400000, CRC(2265ea14) SHA1(db84d5cbc756a13fb1f6286acf3db4d7a3a7d6f4) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lc04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lc04.p1", 0x000000, 0x100000, CRC(338a0f5a) SHA1(060f83d9646532fe01fa1cbe373403a984adb32b) )
	ROM_LOAD16_WORD_SWAP( "256_lc02.p2", 0x100000, 0x400000, CRC(2265ea14) SHA1(db84d5cbc756a13fb1f6286acf3db4d7a3a7d6f4) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lc05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lc05.p1", 0x000000, 0x100000, CRC(e50144d6) SHA1(19e4cb1b42d04ff6d95e3875f0fee85c4ce6a9ac) )
	ROM_LOAD16_WORD_SWAP( "256_lc02.p2", 0x100000, 0x400000, CRC(2265ea14) SHA1(db84d5cbc756a13fb1f6286acf3db4d7a3a7d6f4) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3ld01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_ld01.p1", 0x000000, 0x100000, CRC(503c326d) SHA1(c124d71f771914b5d3388865afcb754e2b828b6e) )
	ROM_LOAD16_WORD_SWAP( "256_ld01.p2", 0x100000, 0x400000, CRC(7357f3d5) SHA1(3d3e0a69a9d83142c42ac9bae8a70dee284fd111) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3ld02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_ld02.p1", 0x000000, 0x100000, CRC(d12b3537) SHA1(e7223e6cfd58cb26b434bf1c0de1771e0be072bd) )
	ROM_LOAD16_WORD_SWAP( "256_ld02.p2", 0x100000, 0x400000, CRC(0dcc7006) SHA1(0f66901f670a3c1c62389f18755aaf0fc57e3f30) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3ld03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_ld03.p1", 0x000000, 0x100000, CRC(20dcf009) SHA1(dbf21e1106661272684acebef45e70d69d04e2a1) )
	ROM_LOAD16_WORD_SWAP( "256_ld02.p2", 0x100000, 0x400000, CRC(0dcc7006) SHA1(0f66901f670a3c1c62389f18755aaf0fc57e3f30) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3ld04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_ld04.p1", 0x000000, 0x100000, CRC(ab89cf4b) SHA1(85ba40ec836071098276e968fe58ec347dacab06) )
	ROM_LOAD16_WORD_SWAP( "256_ld02.p2", 0x100000, 0x400000, CRC(0dcc7006) SHA1(0f66901f670a3c1c62389f18755aaf0fc57e3f30) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3ld05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_ld05.p1", 0x000000, 0x100000, CRC(aa7ff736) SHA1(050eb55ad4ddb9566fff86936d300c229ce75f4b) )
	ROM_LOAD16_WORD_SWAP( "256_ld02.p2", 0x100000, 0x400000, CRC(0dcc7006) SHA1(0f66901f670a3c1c62389f18755aaf0fc57e3f30) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3le01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_le01.p1", 0x000000, 0x100000, CRC(878e6fdd) SHA1(ecb4aac4930fff8eedc07e69ac33b8dc73bd35a2) )
	ROM_LOAD16_WORD_SWAP( "256_le01.p2", 0x100000, 0x400000, CRC(86abbe29) SHA1(0a1401e65910da82f5ef0f525908fa87d1143499) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3le02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_le02.p1", 0x000000, 0x100000, CRC(06996887) SHA1(0ded81c0c4fbea14f44b11c321eac6bcf42bf3b5) )
	ROM_LOAD16_WORD_SWAP( "256_le02.p2", 0x100000, 0x400000, CRC(f8303dfa) SHA1(0566807d0d45e5c214a570856eb483b43f2540ff) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3le03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_le03.p1", 0x000000, 0x100000, CRC(f76eadb9) SHA1(e4cd31c7b0d888c811bc70bb6aa59a12f18d189a) )
	ROM_LOAD16_WORD_SWAP( "256_le02.p2", 0x100000, 0x400000, CRC(f8303dfa) SHA1(0566807d0d45e5c214a570856eb483b43f2540ff) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3le04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_le04.p1", 0x000000, 0x100000, CRC(7c3b92fb) SHA1(078940612d712d8898a0e024fc58441db0413370) )
	ROM_LOAD16_WORD_SWAP( "256_le02.p2", 0x100000, 0x400000, CRC(f8303dfa) SHA1(0566807d0d45e5c214a570856eb483b43f2540ff) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3le05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_le05.p1", 0x000000, 0x100000, CRC(1669a00e) SHA1(491fd81d2987f25e7675d35697cad2a96f09206d) )
	ROM_LOAD16_WORD_SWAP( "256_le02.p2", 0x100000, 0x400000, CRC(f8303dfa) SHA1(0566807d0d45e5c214a570856eb483b43f2540ff) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lf01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lf01.p1", 0x000000, 0x100000, CRC(4ef552d1) SHA1(8d2a8bf7594035eb20a81a186a003331310e18cf) )
	ROM_LOAD16_WORD_SWAP( "256_lf01.p2", 0x100000, 0x400000, CRC(d5603907) SHA1(f751e2b0bd940f298da9b8f1b47ea2fc0ebda8d9) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lf02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lf02.p1", 0x000000, 0x100000, CRC(cfe2558b) SHA1(6e4188ba9aeec8e81fc05a6d24380e9caa80c42d) )
	ROM_LOAD16_WORD_SWAP( "256_lf02.p2", 0x100000, 0x400000, CRC(abfbbad4) SHA1(45edb9e1aa2af12be0561953bd8f4a12cf7654a3) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lf03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lf03.p1", 0x000000, 0x100000, CRC(3e1590b5) SHA1(0e39f8eb4b0323df1187312cc7f9e0e90cde46bc) )
	ROM_LOAD16_WORD_SWAP( "256_lf02.p2", 0x100000, 0x400000, CRC(abfbbad4) SHA1(45edb9e1aa2af12be0561953bd8f4a12cf7654a3) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lf04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lf04.p1", 0x000000, 0x100000, CRC(b540aff7) SHA1(73f26e09f664ecfc21261108633df9c6410fb019) )
	ROM_LOAD16_WORD_SWAP( "256_lf02.p2", 0x100000, 0x400000, CRC(abfbbad4) SHA1(45edb9e1aa2af12be0561953bd8f4a12cf7654a3) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lf05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lf05.p1", 0x000000, 0x100000, CRC(df129d02) SHA1(378427eb2dfcabe87ad00f2c91bd566f4250e11a) )
	ROM_LOAD16_WORD_SWAP( "256_lf02.p2", 0x100000, 0x400000, CRC(abfbbad4) SHA1(45edb9e1aa2af12be0561953bd8f4a12cf7654a3) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lg01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lg01.p1", 0x000000, 0x100000, CRC(93ef1a06) SHA1(8e73a71a029aa0682f5de424ec8286e1446ea6fa) )
	ROM_LOAD16_WORD_SWAP( "256_lg01.p2", 0x100000, 0x400000, CRC(c4663428) SHA1(9866d961d52fdc3ed7332e87a5a338b7874f6342) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lg02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lg02.p1", 0x000000, 0x100000, CRC(12f81d5c) SHA1(f26a331f4c1015b096d12515cd44a2557b983c9c) )
	ROM_LOAD16_WORD_SWAP( "256_lg02.p2", 0x100000, 0x400000, CRC(bafdb7fb) SHA1(99ff24484805435ca88a97ec068593e76d561326) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lg03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lg03.p1", 0x000000, 0x100000, CRC(e30fd862) SHA1(8501443a2253133696fb62a5f53858ef87b2e559) )
	ROM_LOAD16_WORD_SWAP( "256_lg02.p2", 0x100000, 0x400000, CRC(bafdb7fb) SHA1(99ff24484805435ca88a97ec068593e76d561326) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lg04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lg04.p1", 0x000000, 0x100000, CRC(685ae720) SHA1(ff5530cd5a1ec24cd5930814f9b062f63252d4c9) )
	ROM_LOAD16_WORD_SWAP( "256_lg02.p2", 0x100000, 0x400000, CRC(bafdb7fb) SHA1(99ff24484805435ca88a97ec068593e76d561326) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lg05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lg05.p1", 0x000000, 0x100000, CRC(bed1acac) SHA1(5bf2e8b3d4ef571c83bd7ffc8eeedfe2b19946e8) )
	ROM_LOAD16_WORD_SWAP( "256_lg02.p2", 0x100000, 0x400000, CRC(bafdb7fb) SHA1(99ff24484805435ca88a97ec068593e76d561326) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lh01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lh01.p1", 0x000000, 0x100000, CRC(66d67c58) SHA1(453f1c6e07957ebe1de9a022e930f3006d6a4798) )
	ROM_LOAD16_WORD_SWAP( "256_lh01.p2", 0x100000, 0x400000, CRC(b021a698) SHA1(fcaca2c50c9ffe1dc7565a7b9d5606f050515a12) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lh02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lh02.p1", 0x000000, 0x100000, CRC(e7c17b02) SHA1(ee86901f0c9bcc0baf2237598e1b12ff63c9699d) )
	ROM_LOAD16_WORD_SWAP( "256_lh02.p2", 0x100000, 0x400000, CRC(ceba254b) SHA1(9ec91440f3b4e97a904a8935d660e4beb258fc67) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lh03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lh03.p1", 0x000000, 0x100000, CRC(1636be3c) SHA1(2e35cf2cf150a95cfad66a8ef42de8e99ddada0d) )
	ROM_LOAD16_WORD_SWAP( "256_lh02.p2", 0x100000, 0x400000, CRC(ceba254b) SHA1(9ec91440f3b4e97a904a8935d660e4beb258fc67) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lh04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lh04.p1", 0x000000, 0x100000, CRC(9d63817e) SHA1(7883aba24bd575b3f6215772bf86c22935a1655d) )
	ROM_LOAD16_WORD_SWAP( "256_lh02.p2", 0x100000, 0x400000, CRC(ceba254b) SHA1(9ec91440f3b4e97a904a8935d660e4beb258fc67) )
    ROM_DEFAULT_BIOS("euro")
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks28.s1", CRC(af90d166) SHA1(2e925b0bfbbf57aa5bf3522939502bfd97b381df) )

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3lh05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_lh05.p1", 0x000000, 0x100000, CRC(9c95b903) SHA1(23b1ddaca05822a2e0467af1320f3890ee42c4ea) )
	ROM_LOAD16_WORD_SWAP( "256_lh02.p2", 0x100000, 0x400000, CRC(ceba254b) SHA1(9ec91440f3b4e97a904a8935d660e4beb258fc67) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3G_SPRITES
ROM_END

ROM_START( mslug3li01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_li01.p1", 0x000000, 0x100000, CRC(c62cae37) SHA1(c9feb09d312c3e943d81a49e99311e559fd93b2d) )
	ROM_LOAD16_WORD_SWAP( "256_li01.p2", 0x100000, 0x400000, CRC(4f04faa7) SHA1(44769d46237c9db8920548e64ed67d2b4bcbc042) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3li02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_li02.p1", 0x000000, 0x100000, CRC(1aeee8cf) SHA1(da2f816e815fa032d9ae332dcbb7ba01680e06e8) )
	ROM_LOAD16_WORD_SWAP( "256_li01.p2", 0x100000, 0x400000, CRC(4f04faa7) SHA1(44769d46237c9db8920548e64ed67d2b4bcbc042) )
    MSLUG3RM_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

/*************************
 Extreme MegaMods Edition
****************************/

ROM_START( mslug3rmxe0s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s01.p1", 0x000000, 0x100000, CRC(4bb6770f) SHA1(fec25b5bb1f31de36236794a2d4ee955da4bc46a) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s01.p2", 0x100000, 0x400000, CRC(7a0efee4) SHA1(1ddb4636a1ed14df8c3bb7423adae67376729566) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe0s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s02.p1", 0x000000, 0x100000, CRC(8b049116) SHA1(55e09f354458acd89c961ec0efee90494d8100d6) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s02.p2", 0x100000, 0x400000, CRC(edba4a9d) SHA1(fb00a40c8e47ecb23e279b4c61b450ca8ef1720e) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe0s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s03.p1", 0x000000, 0x100000, CRC(7af35428) SHA1(cf528ca564827fcd6d66c9cd305a4863da12a63d) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s02.p2", 0x100000, 0x400000, CRC(edba4a9d) SHA1(fb00a40c8e47ecb23e279b4c61b450ca8ef1720e) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe0s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s04.p1", 0x000000, 0x100000, CRC(5fbbb40e) SHA1(58cde13956b5f0f609768a62d9ffa2643e60743d) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe0s02.p2", 0x100000, 0x400000, CRC(edba4a9d) SHA1(fb00a40c8e47ecb23e279b4c61b450ca8ef1720e) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe1s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s01.p1", 0x000000, 0x100000, CRC(6e8fb793) SHA1(e10b7822bd008becea8d4b8e96abc6f4c09184b4) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s01.p2", 0x100000, 0x400000, CRC(fe3cc308) SHA1(052f4841010658a5bfe54f39ddb5826e869e844e) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe1s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s02.p1", 0x000000, 0x100000, CRC(ae3d518a) SHA1(c5962af0e716b018fb932da0b7b946bddcef45de) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s02.p2", 0x100000, 0x400000, CRC(80a740db) SHA1(58b4d629647e1f592ba9ef23ca8ba90bbc170241) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe1s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s03.p1", 0x000000, 0x100000, CRC(5fca94b4) SHA1(861f07fce35b0a459233a6b5c160200341f8708f) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s02.p2", 0x100000, 0x400000, CRC(80a740db) SHA1(58b4d629647e1f592ba9ef23ca8ba90bbc170241) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe1s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s04.p1", 0x000000, 0x100000, CRC(7a827492) SHA1(2576e72b45793a5ea377c93e4e3ccf1d14e24a80) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe1s02.p2", 0x100000, 0x400000, CRC(80a740db) SHA1(58b4d629647e1f592ba9ef23ca8ba90bbc170241) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe2s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s01.p1", 0x000000, 0x100000, CRC(655286e1) SHA1(5f23246ee7a0d362810746cbe36931f837ba262a) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s01.p2", 0x100000, 0x400000, CRC(748fe101) SHA1(23322fcbcca3dc6f6da15d15a99b42e407fd02e7) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe2s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s02.p1", 0x000000, 0x100000, CRC(a5e060f8) SHA1(ec9536108b7906983e11996d240d1548c16f2a04) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s02.p2", 0x100000, 0x400000, CRC(0a1462d2) SHA1(b3695ec0ab7b0e4510c965b09de9b57e0bb76bd6) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe2s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s03.p1", 0x000000, 0x100000, CRC(5417a5c6) SHA1(24d0a6df55e210b59f47103ed759fed9eeb6beb6) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s02.p2", 0x100000, 0x400000, CRC(0a1462d2) SHA1(b3695ec0ab7b0e4510c965b09de9b57e0bb76bd6) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe2s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s04.p1", 0x000000, 0x100000, CRC(715f45e0) SHA1(afc98673f9d2bb1cfefb043fc97a4b551169e6d6) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe2s02.p2", 0x100000, 0x400000, CRC(0a1462d2) SHA1(b3695ec0ab7b0e4510c965b09de9b57e0bb76bd6) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe3s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s01.p1", 0x000000, 0x100000, CRC(fd5146f0) SHA1(e4c89aabe283423083a60df3014558206bc54614) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s01.p2", 0x100000, 0x400000, CRC(855848d2) SHA1(c0850675674519b32b7d906d46780ce1fcf478f4) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe3s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s02.p1", 0x000000, 0x100000, CRC(3de3a0e9) SHA1(535e4d6958bd29c4e5d5925bfa0d513778f794ab) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s02.p2", 0x100000, 0x400000, CRC(fbc3cb01) SHA1(ac96ec4b6a44a9b0618481ada3718a082c713ec4) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe3s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s03.p1", 0x000000, 0x100000, CRC(cc1465d7) SHA1(e086cddf4174776841a3ffc31e6137f510c93e8c) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s02.p2", 0x100000, 0x400000, CRC(fbc3cb01) SHA1(ac96ec4b6a44a9b0618481ada3718a082c713ec4) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe3s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s04.p1", 0x000000, 0x100000, CRC(e95c85f1) SHA1(00a22364bcad085fcd4c7c6054b67202f4770706) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe3s02.p2", 0x100000, 0x400000, CRC(fbc3cb01) SHA1(ac96ec4b6a44a9b0618481ada3718a082c713ec4) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe4s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s01.p1", 0x000000, 0x100000, CRC(2ae31b40) SHA1(14374db24cf5251ad5735d2e415a8f8803692aa2) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s01.p2", 0x100000, 0x400000, CRC(8eb1bb97) SHA1(fa629aec7f9c6a66db0f3b7674bd69ec6e6cdf18) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe4s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s02.p1", 0x000000, 0x100000, CRC(ea51fd59) SHA1(3d50a0e1c4800ddcb8f5f9f41d35a538d4539465) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s02.p2", 0x100000, 0x400000, CRC(f02a3844) SHA1(fe8ed797e893fd3dc47627f97e33080636bc038b) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe4s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s03.p1", 0x000000, 0x100000, CRC(1ba63867) SHA1(eee2fc541f1aa4d89d720d5c230f0fa708f13bdc) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s02.p2", 0x100000, 0x400000, CRC(f02a3844) SHA1(fe8ed797e893fd3dc47627f97e33080636bc038b) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe4s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s04.p1", 0x000000, 0x100000, CRC(3eeed841) SHA1(796aa58cfa9941b8ee83fba231c8e8e6705a45b4) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe4s02.p2", 0x100000, 0x400000, CRC(f02a3844) SHA1(fe8ed797e893fd3dc47627f97e33080636bc038b) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe5s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s01.p1", 0x000000, 0x100000, CRC(e398264c) SHA1(25a5d6c90d6bcf273dd1c170cdc5276c2601ffa2) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s01.p2", 0x100000, 0x400000, CRC(787b762a) SHA1(e5ad2a818881e8abf23c124aaccb3ad7a8c727c6) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe5s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s02.p1", 0x000000, 0x100000, CRC(232ac055) SHA1(5ee9c179cd0a0cb509dc27f9811e05bb676bc4ae) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s02.p2", 0x100000, 0x400000, CRC(06e0f5f9) SHA1(702c0442a8e2f4d06d47cee99a4628413217ca57) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe5s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s03.p1", 0x000000, 0x100000, CRC(d2dd056b) SHA1(9975bd4bde9137dba971ea105c3bf5227d7f4290) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s02.p2", 0x100000, 0x400000, CRC(06e0f5f9) SHA1(702c0442a8e2f4d06d47cee99a4628413217ca57) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe5s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s04.p1", 0x000000, 0x100000, CRC(f795e54d) SHA1(acc40e1ee666531548868641b8207da2f32c5338) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe5s02.p2", 0x100000, 0x400000, CRC(06e0f5f9) SHA1(702c0442a8e2f4d06d47cee99a4628413217ca57) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe6s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s01.p1", 0x000000, 0x100000, CRC(3e826e9b) SHA1(8ff1d5d2e9cc163deabe35a5ee029f5078619c01) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s01.p2", 0x100000, 0x400000, CRC(c795c2d3) SHA1(dc9c4c7fc75472efcf7581dc60baa21c88e31f69) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe6s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s02.p1", 0x000000, 0x100000, CRC(fe308882) SHA1(120c7451fd7ad5baf0e7ee684b82dead401b2bb8) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s02.p2", 0x100000, 0x400000, CRC(b90e4100) SHA1(7d7010599b87d82bd4c3a4f4887a65537f441993) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe6s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s03.p1", 0x000000, 0x100000, CRC(0fc74dbc) SHA1(4e0e95fafc6958e64ee6f371a72dd2d932b8c19e) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s02.p2", 0x100000, 0x400000, CRC(b90e4100) SHA1(7d7010599b87d82bd4c3a4f4887a65537f441993) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe6s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s04.p1", 0x000000, 0x100000, CRC(2a8fad9a) SHA1(cd3ab08faee862a833e93a73c07286dfd69dd724) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe6s02.p2", 0x100000, 0x400000, CRC(b90e4100) SHA1(7d7010599b87d82bd4c3a4f4887a65537f441993) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe7s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s01.p1", 0x000000, 0x100000, CRC(cbbb08c5) SHA1(3ca001eda4c95629c21265cff02b5c1c5050b536) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s01.p2", 0x100000, 0x400000, CRC(b38ab55a) SHA1(da22fe3575229177ca6da470764bf9afafc95f31) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe7s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s02.p1", 0x000000, 0x100000, CRC(0b09eedc) SHA1(ddebb4601be049b292f6a1848b357e9798befa90) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s02.p2", 0x100000, 0x400000, CRC(cd113689) SHA1(c3e807a26676f25c8271b8736007c2b34122e67c) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe7s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s03.p1", 0x000000, 0x100000, CRC(fafe2be2) SHA1(39ca26ea9119a92cc912b005b931dc5840163be2) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s02.p2", 0x100000, 0x400000, CRC(cd113689) SHA1(c3e807a26676f25c8271b8736007c2b34122e67c) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe7s04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s04.p1", 0x000000, 0x100000, CRC(dfb6cbc4) SHA1(3fe139b140a07e61c2f839faabad7951b0b5a876) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe7s02.p2", 0x100000, 0x400000, CRC(cd113689) SHA1(c3e807a26676f25c8271b8736007c2b34122e67c) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_512K( "256.m1", CRC(eaeec116) SHA1(54419dbb21edc8c4b37eaac2e7ad9496d2de037a) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG3FR_SPRITES
ROM_END

ROM_START( mslug3rmxe8s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe8s01.p1", 0x000000, 0x100000, CRC(bac59055) SHA1(9e36f2536b0aa914ee902820772c5ce275976010) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe8s01.p2", 0x100000, 0x400000, CRC(4586dcb0) SHA1(021adc091b26662c3fb0bc79aa8fa456ebbc7377) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

ROM_START( mslug3rmxe8s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "256_rmxe8s02.p1", 0x000000, 0x100000, CRC(6607d6ad) SHA1(163bd788d3f59e74ed9e7e4ea21dc21b8dd8ac02) )
	ROM_LOAD16_WORD_SWAP( "256_rmxe8s01.p2", 0x100000, 0x400000, CRC(4586dcb0) SHA1(021adc091b26662c3fb0bc79aa8fa456ebbc7377) )
    MSLUG3RMXE_MAINCPU

	NEO_SFIX_128K( "256_hacks06.s1", CRC(8458fff9) SHA1(b120d096deae8abf0ad98362ce230b66e7315a14) )

	NEO_BIOS_AUDIO_256K( "256_hacks25.m1", CRC(2f88e0c0) SHA1(bf95881d1c8861cba1a154eb26552520a764c5ba) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG3_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
    MSLUG3D_SPRITES
ROM_END

/*************
 Metal Slug 4 
***************/
ROM_START( mslug4hacks01 ) //mslug4sd
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks01.p1", 0x000000, 0x100000, CRC(af69d959) SHA1(2bc9796003c8742d681e11ad842978d918fbae40) )
	ROM_LOAD16_WORD_SWAP( "263_hacks01.p2", 0x100000, 0x400000, CRC(2b863b30) SHA1(b5edc43d87faf6540e3154fcbab42efb148cbd04) )
    MSLUG4_MAINCPU

	NEO_SFIX_MSLUG( "263_hacks01.s1", CRC(c4f4ed0e) SHA1(bbf31c879d72cc843229b03db01399af98e02f01) )

	NEO_BIOS_AUDIO_64K( "263d.m1", CRC(0c4c42f8) SHA1(b7be47d909f5e4fcfe769ead0653d40e1e708d36) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS4BOOT_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4D_SPRITES
ROM_END

ROM_START( mslug4hacks02 ) //mslug4fr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks02.p1", 0x000000, 0x100000, CRC(8901649d) SHA1(fa7f9a7166668c3e4f2dbcc0989eb966757ca05d) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_64K( "263d.m1", CRC(0c4c42f8) SHA1(b7be47d909f5e4fcfe769ead0653d40e1e708d36) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4hacks03 ) //mslug4i
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks03.p1", 0x000000, 0x100000, CRC(ffb20602) SHA1(4d3103d613a2452c95ae15b70670664354dce67b) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MSLUG( "263_hacks03.s1", CRC(1f67801e) SHA1(87c33e74599ca063da459f3c8562447bccc104e3) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks04.p1", 0x000000, 0x100000, CRC(e034a441) SHA1(54cda2ae0a998e9a5f64e207f53d681ed5dea186) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks05 ) //mslug4lw
	ROM_REGION( 0x900000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "263_hacks05.p1", 0x000000, 0x100000, CRC(ad713169) SHA1(51c10dfa1b8fc3b0e7850c71920091dbd91d1ba5) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "263_hacks05.p1", 0x000000, 0x100000, CRC(0012756e) SHA1(2dde1e2cc6fba5c50c2832b63b6fd58e025bdd29) )
	ROM_LOAD16_WORD_SWAP( "263_hacks05.p2", 0x100000, 0x800000, CRC(427eed0b) SHA1(6062f783d63748ec9fccad414d3c0a830295aad8) )
	MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4hacks06 ) //mslug4ammor
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks06.p1", 0x000000, 0x100000, CRC(21b68d31) SHA1(97b287e2cbd4eb20fe65339c5d1d42b864e8a440) )
	ROM_LOAD16_WORD_SWAP( "263_hacks05.p2", 0x100000, 0x800000, CRC(427eed0b) SHA1(6062f783d63748ec9fccad414d3c0a830295aad8) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4hacks07 ) //mslug4c
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks07.p1", 0x000000, 0x100000, CRC(f8145f8d) SHA1(ba1df84c199e5f552c80ad7725f9cae5512fbb20) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4hacks08 ) //mslug4dg
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks08.p1", 0x000000, 0x100000, CRC(36dfa877) SHA1(dd49b6b40d3c2cabac8d5efa71cceea8cf9fc4f8) )
	ROM_LOAD16_WORD_SWAP( "263_hacks08.p2", 0x100000, 0x400000, CRC(cf6feb75) SHA1(133e1d97a3ae14835dcb7b6a642c3968cd174547) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks09 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks09.p1", 0x000000, 0x100000, CRC(25f5f99f) SHA1(5cad2f17bfaff789de36e02a3b2e92d353924247) )
	ROM_LOAD16_WORD_SWAP( "263_hacks08.p2", 0x100000, 0x400000, CRC(cf6feb75) SHA1(133e1d97a3ae14835dcb7b6a642c3968cd174547) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks10 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks10.p1", 0x000000, 0x100000, CRC(6bb85c5b) SHA1(98c338826a8bc8c593a49525aeb3474208335763) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks11 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks11.p1", 0x000000, 0x100000, CRC(9a524c97) SHA1(6c2dd09f73e41a18a7e1a3290756d7fcfc8ec6b9) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks12 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks12.p1", 0x000000, 0x100000, CRC(414416d1) SHA1(78d3f7bc4a5e603c8035a2c9ece3969f9ee1b567) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_64K( "263d.m1", CRC(0c4c42f8) SHA1(b7be47d909f5e4fcfe769ead0653d40e1e708d36) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks13 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks13.p1", 0x000000, 0x100000, CRC(3f7ce3c5) SHA1(A0a9cf5d887f2ff9c048d2bb22cfba168b393df0) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks14 ) //mslug41v2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks14.p1", 0x000000, 0x100000, CRC(dddca463) SHA1(d93ca67aed8cdf0765713056a5b6a9a77f09c848) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks15 ) //mslug4lwq
	ROM_REGION( 0x900000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "263_hacks15.p1", 0x000000, 0x100000, CRC(9ebe335e) SHA1(7af10e13b84494393c0e32d01e881060236724db) ) //CRC32 SHA1 Official Rom Hack
	ROM_LOAD16_WORD_SWAP( "263_hacks15.p1", 0x000000, 0x100000, CRC(17ff4563) SHA1(8bd00630233d4256522a719b8d958fe2b903378e) )
	ROM_LOAD16_WORD_SWAP( "263_hacks05.p2", 0x100000, 0x800000, CRC(427eed0b) SHA1(6062f783d63748ec9fccad414d3c0a830295aad8) )
	MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4hacks16 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks16.p1", 0x000000, 0x100000, CRC(851ed337) SHA1(3c7560089461471b971ab9be38f7fce5752f9ef4) )
	ROM_LOAD16_WORD_SWAP( "263_hacks16.p2", 0x100000, 0x400000, CRC(70c810a1) SHA1(7595af7bee4dfe9c664f6bdda2e579385e9fea14) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4hacks17 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks17.p1", 0x000000, 0x100000, CRC(5337b2d8) SHA1(8f84557c36fcda31ad661db5427c5ca5be56793d) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks18 ) //mslug4unity
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks18.p1", 0x000000, 0x100000, CRC(ef3501b6) SHA1(ffbf98407e9646590a59887c42aab47f3a2d67dd) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks19 ) //mslug4a
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks19.p1", 0x000000, 0x100000, CRC(0f2b0fc2) SHA1(8f12a7e703f331ad2e80ac2d2d9c5e7645066468) )
	ROM_LOAD16_WORD_SWAP( "263_hacks19.p2", 0x100000, 0x400000, CRC(87dc01b9) SHA1(ddad7d72e53e0c6273384119f96b3b7f324ca237) )
	MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( mslug4hacks20 ) //mslug4dd
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks20.p1", 0x000000, 0x100000, CRC(02ce0fcd) SHA1(67534a14ae4455f7b4f8456b395a6a648803fbdc) )
	ROM_LOAD16_WORD_SWAP( "263_hacks20.p2", 0x100000, 0x400000, CRC(4569a092) SHA1(98cbc3a554a810c105a3044f09fa732ebc56cb4a) )
    MSLUG4_MAINCPU

	NEO_SFIX_512K( "263d.s1", CRC(1eaa05e0) SHA1(73a7e158c3c4fa602e17dcb517190fd91a9dbab8) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DDH_SPRITES
ROM_END

ROM_START( mslug4hacks21 ) //mslug4ki
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks21.p1", 0x000000, 0x100000, CRC(4e2363d3) SHA1(f574745c5c385d7bb753539d8dbeff657c268ea2) )
	ROM_LOAD16_WORD_SWAP( "263_hacks21.p2", 0x100000, 0x400000, CRC(e95f6bd8) SHA1(4aa293f984b5767bf2a640a08cc36a7cb13a9cfb) )
    MSLUG4_MAINCPU

	NEO_SFIX_512K( "263d.s1", CRC(1eaa05e0) SHA1(73a7e158c3c4fa602e17dcb517190fd91a9dbab8) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DDH_SPRITES
ROM_END

ROM_START( mslug4hacks22 ) //mslug4q
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_hacks22.p1", 0x000000, 0x100000, CRC(461579ae) SHA1(fe7da7ec3e4ee2e0b4281ea76a130a7fe3cac549) )
	ROM_LOAD16_WORD_SWAP( "263_hacks22.p2", 0x100000, 0x400000, CRC(a4d2e871) SHA1(646ac0117a832f1642cffe17478b94635cf294a2) )
	MSLUG4_MAINCPU

	NEO_SFIX_512K( "263d.s1", CRC(1eaa05e0) SHA1(73a7e158c3c4fa602e17dcb517190fd91a9dbab8) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4_SPRITES
ROM_END

ROM_START( ms4boot )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263nd.p1", 0x000000, 0x100000, CRC(4D7E6624) SHA1(125d5203e89cce23a851fa74cc8cbe003ef978f3) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263nd.m1", CRC(ef5db532) SHA1(4aeba9e206b8f309610eb7e1891644f39aa61830) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS4BOOT_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4ND_SPRITES
ROM_END

ROM_START( mslug4d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263.p1", 0x000000, 0x100000, CRC(27e4def3) SHA1(a08785e8145981bb6b5332a3b2df7eb321253cca) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_64K( "263d.m1", CRC(0c4c42f8) SHA1(b7be47d909f5e4fcfe769ead0653d40e1e708d36) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4D_SPRITES
ROM_END

ROM_START( mslug4hd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263hd.p1", 0x000000, 0x100000, CRC(c67f5c8d) SHA1(12af74964843f103520d9f0825069ea2f67eeb2f) )
	ROM_LOAD16_WORD_SWAP( "263hd.p2", 0x100000, 0x400000, CRC(bc3ec89e) SHA1(2cb0626bc4fa57e1d25f208e04532b570d87b3fb) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "263.m1", CRC(46ac8228) SHA1(5aeea221050c98e4bb0f16489ce772bf1c80f787) )
	
	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG4_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4HD_SPRITES
ROM_END

ROM_START( mslug4nd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263nd.p1", 0x000000, 0x100000, CRC(4D7E6624) SHA1(125d5203e89cce23a851fa74cc8cbe003ef978f3) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263nd.m1", CRC(ef5db532) SHA1(4aeba9e206b8f309610eb7e1891644f39aa61830) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4ND_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4ND_SPRITES
ROM_END

ROM_START( mslug4dd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263nd.p1", 0x000000, 0x100000, CRC(4D7E6624) SHA1(125d5203e89cce23a851fa74cc8cbe003ef978f3) )
	ROM_LOAD16_WORD_SWAP( "263.p2", 0x100000, 0x400000, CRC(fdb7aed8) SHA1(dbeaec38f44e58ffedba99e70fa1439c2bf0dfa3) )
    MSLUG4_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263nd.m1", CRC(ef5db532) SHA1(4aeba9e206b8f309610eb7e1891644f39aa61830) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4DD_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DD_SPRITES
ROM_END

ROM_START( mslug4er01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_er01.p1", 0x000000, 0x100000, CRC(401e3ec7) SHA1(ef53b8a42944951e1b37aafd53dc3457cedf377c) )
	ROM_LOAD16_WORD_SWAP( "263_er01.p2", 0x100000, 0x400000, CRC(3902e9fb) SHA1(3523c4060404ea930d0b8f0a49be8a38ce62056b) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4er02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_er02.p1", 0x000000, 0x100000, CRC(28f9c1aa) SHA1(f0e3108f96df35a18ba353e49324152a4062cca0) )
	ROM_LOAD16_WORD_SWAP( "263_er02.p2", 0x100000, 0x400000, CRC(f4732a52) SHA1(cf219c0bdd911f066de047be3f99f797193af923) )
    MSLUG4_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4la01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_la01.p1", 0x000000, 0x100000, CRC(053f7934) SHA1(15baa223565b11cb47a5a4767bf1bd117d7e75c7) )
	ROM_LOAD16_WORD_SWAP( "263_la01.p2", 0x100000, 0x400000, CRC(1782d963) SHA1(207d0c1384db302b61a3efb025d677cdef253049) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4la02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_la02.p1", 0x000000, 0x100000, CRC(d388d7a4) SHA1(3cdf51d8b20b1be747718edcc797866983615f54) )
	ROM_LOAD16_WORD_SWAP( "263_la02.p2", 0x100000, 0x400000, CRC(3842f3f5) SHA1(7d519ac718b0558f534c3dc0dfb7503cb8a592e2) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4la03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_la03.p1", 0x000000, 0x100000, CRC(8ed45af3) SHA1(140d2cbb4dfea4674332ceebab24e7d2092f5745) )
	ROM_LOAD16_WORD_SWAP( "263_la03.p2", 0x100000, 0x400000, CRC(4724c370) SHA1(e6d3890ffeaff36c85b6d92f32220082d2d164cc) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4la04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_la04.p1", 0x000000, 0x100000, CRC(17265486) SHA1(51a962236606d8a124b88c050aab0d472f482ba3) )
	ROM_LOAD16_WORD_SWAP( "263_la04.p2", 0x100000, 0x400000, CRC(fb4ce2cc) SHA1(9da1cee1833e694b37b8a1fd52963e50f456c25f) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4lb01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lb01.p1", 0x000000, 0x100000, CRC(fc19c94e) SHA1(8d7c16888f4c4e8d63e3b72ec3b5ad21209417d8) )
	ROM_LOAD16_WORD_SWAP( "263_la01.p2", 0x100000, 0x400000, CRC(1782d963) SHA1(207d0c1384db302b61a3efb025d677cdef253049) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4lb02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lb02.p1", 0x000000, 0x100000, CRC(2aae67de) SHA1(3ff585a59e701dd1995d5c9f1d3d33cdc5bf61f9) )
	ROM_LOAD16_WORD_SWAP( "263_la02.p2", 0x100000, 0x400000, CRC(3842f3f5) SHA1(7d519ac718b0558f534c3dc0dfb7503cb8a592e2) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4lb03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lb03.p1", 0x000000, 0x100000, CRC(77f2ea89) SHA1(ef26fb368f8a8ce3245c484c28099cbc762a9b95) )
	ROM_LOAD16_WORD_SWAP( "263_la03.p2", 0x100000, 0x400000, CRC(4724c370) SHA1(e6d3890ffeaff36c85b6d92f32220082d2d164cc) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4lb04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lb04.p1", 0x000000, 0x100000, CRC(ee00e4fc) SHA1(77801652f7f7ada6a4f8f7b6c55d5841003d354c) )
	ROM_LOAD16_WORD_SWAP( "263_la04.p2", 0x100000, 0x400000, CRC(fb4ce2cc) SHA1(9da1cee1833e694b37b8a1fd52963e50f456c25f) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4lc01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lc01.p1", 0x000000, 0x100000, CRC(b779c33b) SHA1(ee7ae05901363fcbc4432841f785663328eb090b) )
	ROM_LOAD16_WORD_SWAP( "263_lc01.p2", 0x100000, 0x400000, CRC(daf31aca) SHA1(9e807c89733ff1e77c38436d44430cd65f1b5d29) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4lc02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lc02.p1", 0x000000, 0x100000, CRC(02dc42c7) SHA1(835c694ea0a3cec8e685548d68cf6cddd9368549) )
	ROM_LOAD16_WORD_SWAP( "263_lc02.p2", 0x100000, 0x400000, CRC(f533305c) SHA1(50c63f30a419c080d0d9c51e1befb332b8a03147) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4lc03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lc03.p1", 0x000000, 0x100000, CRC(5f80cf90) SHA1(9d8a32d3165fe5dc31ba745c8f0375e131574be3) )
	ROM_LOAD16_WORD_SWAP( "263_lc03.p2", 0x100000, 0x400000, CRC(8a5500d9) SHA1(7c96c092b72faf688a14e64ef2561bd5a5a74355) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4lc04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_lc04.p1", 0x000000, 0x100000, CRC(c672c1e5) SHA1(28ca2dad7268c88c26a0dc989437675ee4280601) )
	ROM_LOAD16_WORD_SWAP( "263_lc04.p2", 0x100000, 0x400000, CRC(363d2165) SHA1(ff395dcc4ec9bf556062bfd9d834eee30f7356bd) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4ld01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_ld01.p1", 0x000000, 0x100000, CRC(eb1fea53) SHA1(da634576b818c8bc2394554b8b7cfe068c7e6793) )
	ROM_LOAD16_WORD_SWAP( "263_lc01.p2", 0x100000, 0x400000, CRC(daf31aca) SHA1(9e807c89733ff1e77c38436d44430cd65f1b5d29) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4ld02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_ld02.p1", 0x000000, 0x100000, CRC(35f26faf) SHA1(4ced71b45ddc20643381faf7afbab60592ac7960) )
	ROM_LOAD16_WORD_SWAP( "263_lc02.p2", 0x100000, 0x400000, CRC(f533305c) SHA1(50c63f30a419c080d0d9c51e1befb332b8a03147) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4ld03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_ld03.p1", 0x000000, 0x100000, CRC(68aee2f8) SHA1(4edfeff70a0c78ce7cc0695949283de9b514d515) )
	ROM_LOAD16_WORD_SWAP( "263_lc03.p2", 0x100000, 0x400000, CRC(8a5500d9) SHA1(7c96c092b72faf688a14e64ef2561bd5a5a74355) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4ld04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_ld04.p1", 0x000000, 0x100000, CRC(f15cec8d) SHA1(2990f1b6cbbaa9c87ef45c6fe319ae2e077e1a95) )
	ROM_LOAD16_WORD_SWAP( "263_lc04.p2", 0x100000, 0x400000, CRC(363d2165) SHA1(ff395dcc4ec9bf556062bfd9d834eee30f7356bd) )
    MSLUG4RM_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4rmxe0s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_rmxe0s01.p1", 0x000000, 0x100000, CRC(261920ab) SHA1(5e80ca5b3a12387a8eca99a81325b54769405693) )
	ROM_LOAD16_WORD_SWAP( "263_rmxe0s01.p2", 0x100000, 0x400000, CRC(dcc2cc7d) SHA1(70e2be80829f078d4494aa3a173592ae5fcc5c20) )
    MSLUG4RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4rmxe1s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_rmxe1s01.p1", 0x000000, 0x100000, CRC(0d646a0f) SHA1(6ca6caae3cbea88d199393e8f4db320eecae5323) )
	ROM_LOAD16_WORD_SWAP( "263_rmxe1s01.p2", 0x100000, 0x400000, CRC(c86b7563) SHA1(27b4c17cca341dc049ee003dcc4067aab6a491ff) )
    MSLUG4RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4LA_SPRITES
ROM_END

ROM_START( mslug4rmxe2s01 )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_rmxe2s01.p1", 0x000000, 0x100000, CRC(1dc62090) SHA1(f37285175e98a9aaaab60be6384f8a1e5c95175b) )
	ROM_LOAD16_WORD_SWAP( "263_rmxe2s01.p2", 0x100000, 0x400000, CRC(6982e3a2) SHA1(dedca273c1b05325f6054db389d34eecf604ccb2) )
    MSLUG4RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4rmxe3s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_rmxe3s01.p1", 0x000000, 0x100000, CRC(bd7cc317) SHA1(c6db9ead81c3e2c963b55fc2b493fd4a10eb4918) )
	ROM_LOAD16_WORD_SWAP( "263_rmxe3s01.p2", 0x100000, 0x400000, CRC(359e2ebe) SHA1(9e897bf804acfd8e8a3d6eff33ae00e82c54351c) )
    MSLUG4RMXE_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4FR_SPRITES
ROM_END

ROM_START( mslug4rmxe4s01 )
	ROM_REGION( 0x900000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "263_rmxe4s01.p1", 0x000000, 0x100000, CRC(eb7c5890) SHA1(4692275ea7ce80688e392d9c8f69788257bb04e9) )
	ROM_LOAD16_WORD_SWAP( "263_rmxe4s01.p2", 0x100000, 0x800000, CRC(39f9dc62) SHA1(ec2c39f4c1884d2cef0e8b192a7dd9b7be28d63c) )
    MSLUG4RMXE_MAINCPU

	NEO_SFIX_128K( "263hd.s1", CRC(a9446774) SHA1(c5a309fd8ee6d6750a15c82e710218a3755e38b2) )

	NEO_BIOS_AUDIO_128K( "263_hacks05.m1", CRC(49b1453e) SHA1(e6296e1d6bb604f1a15cf8d1b273e97b6eb7cced) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG4LW_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUG4DD_SPRITES
ROM_END

/*************
 Metal Slug 5
***************/

ROM_START( mslug5hacks01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks01.p1", 0x000000, 0x100000, CRC(3c8588d7) SHA1(5dada21b9530586f8941e027d3f2195e72a3ca5d) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
    MSLUG5_YMSND
	
	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5hacks02 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks02.p1", 0x000000, 0x600000, CRC(faeffb08) SHA1(9fe401d8252c5d46f3a84f59e1e182eecaa4c069) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND
	
	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks03 ) //mslug5fr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks03.p1", 0x000000, 0x100000, CRC(63cc3c2d) SHA1(b4543c65427d15f784641617dcab53663031a9f3) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks03.s1", CRC(77b18feb) SHA1(2d7cb5838730bfcb45eceeef11557cbcab8aa9da) )

	NEO_BIOS_AUDIO_64K( "268d.m1", CRC(3c0655a7) SHA1(ae839d4c2b87a7aa3dd8e5caddc43eb75ee9b732) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5PLUSD_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5FR_SPRITES
ROM_END

ROM_START( mslug5hacks04 ) //mslug5w
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks04.p1", 0x000000, 0x100000, CRC(982c0e73) SHA1(ffc2cd64114d42b76ba1c6f7d02c15581d12a957) )
	ROM_LOAD16_WORD_SWAP( "268_hacks04.p2", 0x100000, 0x400000, CRC(f06c589a) SHA1(0882ee5221ab873f87a719e34bd5250b79b82730) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5W_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5W_SPRITES
ROM_END

ROM_START( mslug5hacks05 ) //mslug5sgf
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks05.p1", 0x000000, 0x600000, CRC(b1fbb850) SHA1(4fc556a11107c2ff9bf1c0620fa635ae206f46d7) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks06 ) //mslug5f
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks06.p1", 0x000000, 0x100000, CRC(430a2c94) SHA1(ec7de4d0c15d262e0fec34c1764e849b7dc0dc83) )
	ROM_LOAD16_WORD_SWAP( "268_hacks04.p2", 0x100000, 0x400000, CRC(f06c589a) SHA1(0882ee5221ab873f87a719e34bd5250b79b82730) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5F_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5F_SPRITES
ROM_END

ROM_START( mslug5hacks07 ) //mslug5fe
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks07.p1", 0x000000, 0x600000, CRC(88787ee2) SHA1(7172fd1890957b428504e5cf00e9721d7aaed4a1) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks08 ) //mslug5unity
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "268_hacks08.p1", 0x000000, 0x400000, CRC(ced9f5a8) SHA1(c1e6c8c9a7d2220b956486f2154540fcda38e650) )
	ROM_LOAD32_WORD_SWAP( "268_hacks08.p2", 0x000002, 0x400000, CRC(1c08dbb6) SHA1(8796efebcfabb9bb5444ed0264d327abf7c32b98) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND
	
	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5_SPRITES
ROM_END

ROM_START( mslug5hacks09 ) //mslug5x 
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks09.p1", 0x000000, 0x600000, CRC(afffcd5b) SHA1(4383bd6903e49a5c0a390b77b746b9f53509542d) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks10 ) //mslug5es
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks10.p1", 0x000000, 0x600000, CRC(74216018) SHA1(a606f9b3630f2243a446274bbbacb145d1522c5c) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks11 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks11.p1", 0x000000, 0x100000, CRC(f1f02331) SHA1(d768c70f86032b53aa1c3d55138c6c5ebc735ea2) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5hacks12 ) //mslug5c
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks12.p1", 0x000000, 0x600000, CRC(e876d1e7) SHA1(01508fc02789603e85ac13e0d5fcddda6fadc346) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks13 ) //mslug5dbj
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks13.p1", 0x000000, 0x600000, CRC(1527c4dd) SHA1(5a83fa57e09b0529f30ee7f70e0b488dde3ba203) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks14 ) //mslug5mg
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks14.p1", 0x000000, 0x600000, CRC(1fc7de70) SHA1(c9f8f70706db9832dacb70723d5525aab4bfb9ac) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks15 ) //mslug51v2
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks15.p1", 0x000000, 0x600000, CRC(953d6e11) SHA1(d4faa3c515c7c4337e89513ea2bda13685b38a06) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks16 ) //mslug5zh
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks16.p1", 0x000000, 0x600000, CRC(dc057a7a) SHA1(54ca7d543916424fad5dab82c67bb70cb4b5dc11) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks17 ) //mslug5g
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks17.p1", 0x000000, 0x600000, CRC(3be747ab) SHA1(48ab8b6bedbd854a74b7fe4055d1a5c7a4b9cda8) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks18 ) //mslug5sg
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks18.p1", 0x000000, 0x600000, CRC(59402234) SHA1(95dc2f340ef3b02642d87d4c50d2f1984266e4a3) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks19 ) //mslug5pv
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks19.p1", 0x000000, 0x600000, CRC(5288f47b) SHA1(137ca41e06f787204c50b2e6f1ceee91ffef3310) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks20 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks20.p1", 0x000000, 0x100000, CRC(ce788ab1) SHA1(89061d49e40e8774ff337ae4601f5c7676eac583) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5hacks21 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks21.p1", 0x000000, 0x100000, CRC(de60d4a5) SHA1(54a157cdb98dac018dae5f2ddae4fbcdf04857f2) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5hacks22 ) //mslug5dd
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks22.p1", 0x000000, 0x600000, CRC(40917fba) SHA1(7e8e407c2936435be2b1909f582ae9025b1c7a8d) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks23 ) //mslug5ki
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks23.p1", 0x000000, 0x600000, CRC(846bc220) SHA1(d0a6ac7a5c04adc5bec65313f632f16b83b70628) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( mslug5hacks24 ) //mslug5ext
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_hacks24.p1", 0x000000, 0x600000, CRC(7ff5364b) SHA1(698373c6c3fb37fe20c4194c5c51251f6a251c18) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_128K( "268n.m1", CRC(6fa01c9a) SHA1(3dab7593bfcce318d22ec3df672ee3b4ab73dcf5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5EXT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( ms5boot )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268nd.p1", 0x000000, 0x100000, CRC(ca50afdf) SHA1(e3780b77f20d139a0dcaa2ded2c6ee323b8b4279) )
	ROM_LOAD16_WORD_SWAP( "268boot.p2", 0x100000, 0x100000, CRC(3fc46cfa) SHA1(f20d3d359f6cdbf6aabb6920020621b02bafee91) )
	ROM_CONTINUE(0x300000, 0x100000 )
	ROM_LOAD16_WORD_SWAP( "268boot.p3", 0x200000, 0x100000, CRC(742c955a) SHA1(96c0f08b1f2f6877f5169a96c13b67f3be6082c6) )
	ROM_CONTINUE(0x400000, 0x100000 )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K
	ROM_LOAD( "268boot.s1", 0x10000, 0x10000, CRC(52a8c09b) SHA1(3a90d8e44aa7cbc79945f8ece0e2fb9ce3e5a4c7) )
	ROM_CONTINUE(           0x00000, 0x10000 )

	NEO_BIOS_AUDIO_256K( "268boot.m1", CRC(792e07c1) SHA1(117516e8ec9026c7682ab27857aab6639bef5835) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5BOOT_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5ND_SPRITES
ROM_END

ROM_START( ms5pcbd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "268.p1", 0x000000, 0x400000, CRC(d0466792) SHA1(880819933d997fab398f91061e9dbccb959ae8a1) )
	ROM_LOAD32_WORD_SWAP( "268.p2", 0x000002, 0x400000, CRC(fbf6b61e) SHA1(9ec743d5988b5e3183f37f8edf45c72a8c0c893e) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_512K

	ROM_REGION16_BE( 0x80000, "mainbios", 0 )
	/* this contains both an ASIA and JAPAN bios, HARDDIP3 on the PCB selects which to use */
	ROM_LOAD16_WORD_SWAP( "sp-4x.sp1", 0x00000, 0x80000, CRC(b4590283) SHA1(47047ed5b6062babc0a0bebcc30e4b3f021e115a) )

	/* Encrypted, we load it here for reference and replace with decrypted ROM */
	// ROM_LOAD( "268-m1.bin", 0x00000, 0x10000, CRC(58b107d0) SHA1(cc7fe66ff4f9c026cde4df06f86c848eb21f7af8) )
	//NEO_BIOS_AUDIO_64K( "268d.m1", CRC(3c0655a7) SHA1(ae839d4c2b87a7aa3dd8e5caddc43eb75ee9b732) )
	ROM_REGION( 0x80000, "audiocrypt", 0 )
	ROM_LOAD( "268.m1", 0x00000, 0x80000, CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )
	ROM_REGION( 0x90000, "audiocpu", ROMREGION_ERASEFF )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5PCBD_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MS5PCBD_SPRITES
ROM_END

ROM_START( mslug5nd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268nd.p1", 0x000000, 0x100000, CRC(ca50afdf) SHA1(e3780b77f20d139a0dcaa2ded2c6ee323b8b4279) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "268nd.m1", CRC(2f621a2b) SHA1(e88f645b4935157608c32843f203180c796f5304) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5ND_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5d )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268nd.p1", 0x000000, 0x100000, CRC(ca50afdf) SHA1(e3780b77f20d139a0dcaa2ded2c6ee323b8b4279) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_64K( "268d.m1", CRC(3c0655a7) SHA1(ae839d4c2b87a7aa3dd8e5caddc43eb75ee9b732) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D_SPRITES
ROM_END

ROM_START( mslug5d1 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268nd.p1", 0x000000, 0x100000, CRC(ca50afdf) SHA1(e3780b77f20d139a0dcaa2ded2c6ee323b8b4279) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x100000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_64K( "268d.m1", CRC(3c0655a7) SHA1(ae839d4c2b87a7aa3dd8e5caddc43eb75ee9b732) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5b1 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268b.p1", 0x000000, 0x100000, CRC(d05853ea) SHA1(8bb284d8f937b1b50d28d207f5fd262e4ed1d96c) )
	ROM_CONTINUE( 0x200000, 0x100000 )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_CONTINUE( 0x300000, 0x100000 )
	ROM_CONTINUE( 0x500000, 0x100000 )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5_SPRITES
ROM_END

ROM_START( ms5plusd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ms5-p1psd.p1", 0x000000, 0x100000, CRC(76af334f) SHA1(cbd890a9c14d42acd1923bb5074fb560a306cce3) )
	ROM_LOAD16_WORD_SWAP( "ms5-p2p.bin", 0x100000, 0x200000, CRC(d6a458e8) SHA1(c0a8bdae06d62859fb6734766ccc190eb2a809a4) )
	ROM_LOAD16_WORD_SWAP( "ms5-p3p.bin", 0x300000, 0x200000, CRC(439ec031) SHA1(f0ad8f9be7d26bc504593c1321bd23c286a221f0) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K("268sd.s1", CRC(99ef577b) SHA1(82f30b61ea4439a6673e0b70b7a9aceaaafd8943) )

	NEO_BIOS_AUDIO_512K( "268sd.m1", CRC(39f3cbba) SHA1(56f9ba6a1ecfc28733b7b88c9796415cba0461f2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MS5PLUSD_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5n )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268n.p1", 0x000000, 0x600000, CRC(975eb06a) SHA1(d3d4824a0b9f077c6503959da54edb53820e6a8d) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5b3 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268ba.p1", 0x000000, 0x100000, CRC(3f1cf3d2) SHA1(2c85eb5acdede4816675b91f8989c9e13c1573f8) )
	ROM_IGNORE( 0x300000 )
	ROM_CONTINUE( 0x200000, 0x100000 )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_CONTINUE( 0x300000, 0x100000 )
	ROM_CONTINUE( 0x500000, 0x100000 )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5_SPRITES
ROM_END

ROM_START( mslug5n2 ) //Decrypted version of the roms mslug5n
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268n2.p1", 0x000000, 0x200000, CRC(f23968bf) SHA1(ed4caee7caf1b2b06a5a0c76f48952d883ae6922) )
	ROM_LOAD16_WORD_SWAP( "268nd.p2", 0x200000, 0x400000, CRC(768ee64a) SHA1(76a65a69aee749758a2101aabdd44f3404838b54) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( mslug5hd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "268h.p1", 0x000000, 0x400000, CRC(3636690a) SHA1(e0da714b4bdc6efffe1250ded02ebddb3ab6d7b3) )
	ROM_LOAD32_WORD_SWAP( "268h.p2", 0x000002, 0x400000, CRC(8dfc47a2) SHA1(27d618cfbd0107a4d2a836797e967b39d2eb4851) )
    MSLUG5H_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5D1_SPRITES
ROM_END

ROM_START( ms5plusc )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ms5-p1pc.bin", 0x000000, 0x100000, CRC(c61e6444) SHA1(9aec69201472080d2205af14ecc3f9a30ab4c6c2) )
	ROM_LOAD16_WORD_SWAP( "ms5-p2p.bin", 0x100000, 0x200000, CRC(d6a458e8) SHA1(c0a8bdae06d62859fb6734766ccc190eb2a809a4) )
	ROM_LOAD16_WORD_SWAP( "ms5-p3p.bin", 0x300000, 0x200000, CRC(439ec031) SHA1(f0ad8f9be7d26bc504593c1321bd23c286a221f0) )
    MSLUG5H_MAINCPU

	NEO_SFIX_128K("ms5-s1p.bin", CRC(21e04432) SHA1(10057a2aa487087f7143d1d69fdad978a6bef0f7) )

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "268.m1", CRC(4a5a6e0e) SHA1(df0f660f2465e1db7be5adfcaf5e88ad61a74a42) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5_SPRITES
ROM_END

ROM_START( mslug5la01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_la01.p1", 0x000000, 0x200000, CRC(ea94e683) SHA1(a9ada760fbe765426e935c2282004a121e8d8c54) )
	ROM_LOAD16_WORD_SWAP( "268_la01.p2", 0x200000, 0x400000, CRC(20dd03ac) SHA1(2edfbe5975b43a9b304ec4f113e8de31d96b0ae6) )
    MSLUG5RM_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5LA_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5LA_SPRITES
ROM_END

ROM_START( mslug5lb01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_lb01.p1", 0x000000, 0x200000, CRC(e033f078) SHA1(3fcb2efe25264b8399e5169109fba016e7b88f39) )
	ROM_LOAD16_WORD_SWAP( "268_lb01.p2", 0x200000, 0x400000, CRC(b3110397) SHA1(57d8f35d46475d95ee843adfd9387d863cc6718e) )
    MSLUG5RM_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5LA_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5LA_SPRITES
ROM_END

ROM_START( mslug5lc01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_lc01.p1", 0x000000, 0x200000, CRC(5370db6d) SHA1(8f391e317ba14dd9e1cff63048f6c66906657153) )
	ROM_LOAD16_WORD_SWAP( "268_lc01.p2", 0x200000, 0x400000, CRC(131013dc) SHA1(52f4cb85ebd395fdf0aa19fbd66e3c39e3d08689) )
    MSLUG5RM_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5LA_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5LA_SPRITES
ROM_END

ROM_START( mslug5rmxe0s01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe0s01.p1", 0x000000, 0x200000, CRC(5de79634) SHA1(f2b4b902beed777c3a18b9bb1e581c781b99113f) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe0s01.p2", 0x200000, 0x400000, CRC(b2671f9d) SHA1(6fc8834183813dcfe649c93c1ac407fc5342bf36) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5RMXE_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

ROM_START( mslug5rmxe1s01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe1s01.p1", 0x000000, 0x200000, CRC(91fb1ddc) SHA1(210a631bab699ff1c0ab5f550f04ef1b7033eea2) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe1s01.p2", 0x200000, 0x400000, CRC(81aa0fed) SHA1(9bdeb734d16f8fd33e0ed0b07facd28ada383190) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5RMXE_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

ROM_START( mslug5rmxe2s01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe2s01.p1", 0x000000, 0x200000, CRC(7d2bc8e6) SHA1(4f670378f3f7d3fb1f3b88b165fab43d78388382) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe2s01.p2", 0x200000, 0x400000, CRC(2aa0f0a1) SHA1(1f91c6274b9aa59a4f5944293da2a866e6f1029b) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5RMXE_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

ROM_START( mslug5rmxe3s01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe3s01.p1", 0x000000, 0x200000, CRC(3663f06b) SHA1(524a74edcf4e67609a7eb4bbcf9259432700bb04) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe3s01.p2", 0x200000, 0x400000, CRC(1d956047) SHA1(535be6dad8201e25207e6ffb5d45685f8d4d2558) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5RMXE_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

ROM_START( mslug5rmxe4s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe4s01.p1", 0x000000, 0x100000, CRC(f8fcf723) SHA1(bc0bc6b257a42ad9857c9d062e20d494d4532dc8) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe4s01.p2", 0x100000, 0x400000, CRC(c850f9c0) SHA1(bf997c7bc02c83e98176e572e6733f8788f17ff4) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5LA_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

ROM_START( mslug5rmxe5s01 )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "268_rmxe5s01.p1", 0x000000, 0x200000, CRC(dbe27f4f) SHA1(568236c3e48f38e54bc34d5abc3e3b9457133155) )
	ROM_LOAD16_WORD_SWAP( "268_rmxe0s01.p2", 0x200000, 0x400000, CRC(b2671f9d) SHA1(6fc8834183813dcfe649c93c1ac407fc5342bf36) )
    MSLUG5RMXE_MAINCPU

	NEO_SFIX_128K( "268_hacks02.s1", CRC(64952683) SHA1(88ec728c2fe18a11fdd218bed5d73bb3affe2ec1) )

	NEO_BIOS_AUDIO_512K( "268_hacks04.m1", CRC(464c72ad) SHA1(9e8a01c2bad897e2bbf32563a30f0474f8fcd2b5) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	MSLUG5RMXE_YMSND

	ROM_REGION( 0x4000000, "sprites", 0 )
	MSLUG5RMXE_SPRITES
ROM_END

/************
 Metal Slug X 
**************/

ROM_START( mslugxhacks01 ) //mslugxc2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks01.p1", 0x000000, 0x100000, CRC(944ac109) SHA1(4bba27627a565d60f807993e96efae2f256d1007) )
	ROM_LOAD16_WORD_SWAP( "250_hacks01.p2", 0x100000, 0x400000, CRC(5d1c52cd) SHA1(b2b131fa845b0098cbf0750fee20b402e276483b) )
	MSLUGX_MAINCPU
	
	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
    MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
    MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks02.p1", 0x000000, 0x100000, CRC(27d836c8) SHA1(4b110e1f1094cea607914156c69298017153a9d8) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks03 ) //mslugxsrf
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks03.p1", 0x000000, 0x100000, CRC(aed327fe) SHA1(3a8888dd7752be94bce9f3f0df942d7feeff4bae) )
	ROM_LOAD16_WORD_SWAP( "250_hacks03.p2", 0x100000, 0x400000, CRC(4389f47d) SHA1(8774ed04cb5d66e6cdda2f27165e0dfcc230bd8a) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks04 ) //mslugxunity
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks04.p1", 0x000000, 0x100000, CRC(36102d34) SHA1(b60d41e7cd3395910e7aa5cc0a843d68bfcc2ac6) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks05 ) //mslugxeb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks05.p1", 0x000000, 0x100000, CRC(764d1bb1) SHA1(e46824646aa46121e8591865f609f457fbb6521e) )
	ROM_LOAD16_WORD_SWAP( "250_hacks05.p2", 0x100000, 0x400000, CRC(a51363d1) SHA1(7b6b3490a975ac1e549df60b9d70ccd6d2f2ba6e) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks06 ) //msluxfr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks06.p1", 0x000000, 0x100000, CRC(d8834fcc) SHA1(2391af1873d33b49452fa5178c568bbd7154aaef) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXFR_SPRITES
ROM_END

ROM_START( mslugxhacks07 ) //mslugxhp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks01.p1", 0x000000, 0x100000, CRC(944ac109) SHA1(4bba27627a565d60f807993e96efae2f256d1007) )
	ROM_LOAD16_WORD_SWAP( "250_hacks07.p2", 0x100000, 0x400000, CRC(eac6830a) SHA1(27927f5341c128f93d74113514f2d099d01acbdf) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks08 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks08.p1", 0x000000, 0x100000, CRC(c0911075) SHA1(2645c9d59915811addc2ab048afbfb9e605bca24) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks09 ) //mslugxit
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks09.p1", 0x000000, 0x100000, CRC(f3e154ea) SHA1(bb8260deefa7b048772625085e6f41cab717a147) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks10 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks10.p1", 0x000000, 0x100000, CRC(c405753d) SHA1(bed7426145fd15e972334d7ef04759edcccd8864) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks11 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks01.p1", 0x000000, 0x100000, CRC(944ac109) SHA1(4bba27627a565d60f807993e96efae2f256d1007) )
	ROM_LOAD16_WORD_SWAP( "250_hacks11.p2", 0x100000, 0x400000, CRC(8d20ccca) SHA1(53cb23cf489fa29f5b31847c82d51765ce32e633) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks12 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks12.p1", 0x000000, 0x100000, CRC(7b27ae6f) SHA1(9e51a3ea83e482ccde6a898dabcb1db7d2a66206) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks13 ) //mslugxp
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks13.p1", 0x000000, 0x100000, CRC(e65937fc) SHA1(128af1815a23962de02384d61ff52869ae14cea1) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks14 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks14.p1", 0x000000, 0x100000, CRC(a11473d5) SHA1(83f6665e88e8b9d33545e5c57ac8a5e923787bd2) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks15 ) //mslugxh
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks15.p1", 0x000000, 0x100000, CRC(a3764c51) SHA1(791e23b6953aba236fe18182efa7445291c991ba) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks16 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks16.p1", 0x000000, 0x100000, CRC(e75a6d64) SHA1(529130f7ebb43231e6bf1b1703266b6c09356f9c) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks17 ) //mslugx1v2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks17.p1", 0x000000, 0x100000, CRC(0cf1f95b) SHA1(857af8721c5929ba7c60e16223cab4879ec382b9) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks18 ) //mslugxh
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks18.p1", 0x000000, 0x100000, CRC(6a116211) SHA1(92b1d87248e03d21eb87201cbadc5de0eaaf4b47) )
	ROM_LOAD16_WORD_SWAP( "250_hacks18.p2", 0x100000, 0x400000, CRC(91e82e00) SHA1(402790a5f99d4909d472b3ca45c8a27bf36c5de6) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks19 ) //mslugx2r
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks19.p1", 0x000000, 0x100000, CRC(721f11aa) SHA1(49c4c5f86f37d0e33a0ece707608de2ca1855be2) )
	ROM_LOAD16_WORD_SWAP( "250_hacks19.p2", 0x100000, 0x400000, CRC(4f875278) SHA1(ca6e474171e8bebc63033c883a8acfbfbce43f28) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks20 ) //mslugxdd
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks20.p1", 0x000000, 0x100000, CRC(aa09586a) SHA1(246579b58babcf696f988a2d57dc8fab0019b3de) )
	ROM_LOAD16_WORD_SWAP( "250_hacks20.p2", 0x100000, 0x400000, CRC(1791d721) SHA1(cdc6231f1fde1eef90c51ef9a5c22e900055c3f5) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXDD_SPRITES
ROM_END

ROM_START( mslugxhacks21 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks21.p1", 0x000000, 0x100000, CRC(14b48ac5) SHA1(66c777d170bbaa0fb19eaceab0e2fddd9697127c) )
	ROM_LOAD16_WORD_SWAP( "250_hacks21.p2", 0x100000, 0x400000, CRC(69d8cda7) SHA1(e2c316726ebb22dfab8a9ac18fda6775a25d5296) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks22 ) //mslugx2reb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks22.p1", 0x000000, 0x100000, CRC(dba3f1a1) SHA1(2d2a5b67bb1b23470d032dbcb6bd454a4ff86da7) )
	ROM_LOAD16_WORD_SWAP( "250_hacks22.p2", 0x100000, 0x400000, CRC(3866eb68) SHA1(38fb15f428388a17c70841fea9d1503dad710c90) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks23 ) //mslugx2r1v2
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks23.p1", 0x000000, 0x100000, CRC(d13fd368) SHA1(ea2d8a78c23090ba0424ad41fcc9935bc1dcf277) )
	ROM_LOAD16_WORD_SWAP( "250_hacks23.p2", 0x100000, 0x400000, CRC(730e94a2) SHA1(590b5950bce33b24122dcad99c91d9bc436d16d7) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks24 ) //mslugxc1
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks24.p1", 0x000000, 0x100000, CRC(e74f36c2) SHA1(b17d66f36e6ac31433179566ea07a0ed8790b161) )
	ROM_LOAD16_WORD_SWAP( "250_hacks24.p2", 0x100000, 0x400000, CRC(e954b8aa) SHA1(b339b1fd05d50ac3a9d3992bb0fc8e3fd1a95557) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks25 ) //mslugxlb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks25.p1", 0x000000, 0x100000, CRC(41fc71cc) SHA1(82c9f78edb0cba74491f755fc2f349fc7131ade0) )
	ROM_LOAD16_WORD_SWAP( "250_hacks25.p2", 0x100000, 0x400000, CRC(6a114e1a) SHA1(3da62274252d36226dcccda0e05593f7f18c7193) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxhacks26 ) //mslugxst
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks26.p1", 0x000000, 0x100000, CRC(db88d115) SHA1(627d01c4d4bd6c4cfeac5f14859fe9a7026370b0) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks27 ) //mslugxxr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks27.p1", 0x000000, 0x100000, CRC(bea1f7e5) SHA1(52148032328e4f2e4b6fe93f9df816891ebd50bb) )
	ROM_LOAD16_WORD_SWAP( "250_hacks27.p2", 0x100000, 0x400000, CRC(0efc5d67) SHA1(21a370ebc96e1b17ecfb12085732512dc51491d3) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks28 ) //mslugxmax
	ROM_REGION( 0x500000, "maincpu", 0 )
	//ROM_LOAD16_WORD_SWAP( "250_hacks28.p2", 0x100000, 0x400000, CRC(9cf91e0f) SHA1(ed094f66d5b3e231ceb709bd578b47ce436709fe) ) // [UPDATE]
	ROM_LOAD16_WORD_SWAP( "250_hacks28.p1", 0x000000, 0x100000, CRC(1ffd4613) SHA1(796c3f4af17692ef74cc9ddf771b1a23c53b914b) )
	ROM_LOAD16_WORD_SWAP( "250_hacks28.p2", 0x100000, 0x400000, CRC(8d595131) SHA1(523401915e5b751c9d67c169173de7e17fe7ff00) ) // [RECOVERY]
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks29 ) //mslugxr
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks29.p1", 0x000000, 0x100000, CRC(a507546f) SHA1(ab6e9e969c2b3728cb0b13e0b1d62bda09037ac0) )
	ROM_LOAD16_WORD_SWAP( "250_hacks29.p2", 0x100000, 0x400000, CRC(78a66189) SHA1(a0689ea58b12873c939ebe949b42145ef49aff3f) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks30 ) //mslugxsv
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks30.p1", 0x000000, 0x100000, CRC(1b213460) SHA1(23f14b2e97582552e6db1e95f98b12f73140d9a5) )
	ROM_LOAD16_WORD_SWAP( "250_hacks30.p2", 0x100000, 0x400000, CRC(718d6d66) SHA1(d15f0394cab7c84f8434a3095b25014fd9bb3000) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxhacks31 ) //mslugxsc
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks31.p1", 0x000000, 0x100000, CRC(d8c467bd) SHA1(e4c42da247fd21d15700065cd5ef0db04e435690) )
	ROM_LOAD16_WORD_SWAP( "250_hacks31.p2", 0x100000, 0x400000, CRC(d2e02a46) SHA1(8fae243c57fde0c61de48f30fb5c7955e6e646ae) )
    ROM_DEFAULT_BIOS("euro")
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxhacks32 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_hacks32.p1", 0x000000, 0x100000, CRC(b606e809) SHA1(a477647cf470a115b9df1d37ab859188de49f9a9) )
	ROM_LOAD16_WORD_SWAP( "250.p2", 0x100000, 0x400000, CRC(1fda2e12) SHA1(18aaa7a3ba8da99f78c430e9be69ccde04bc04d9) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxer01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er01.p1", 0x000000, 0x100000, CRC(9418f9d0) SHA1(fbe748a88355c10371c7040162ce6f202bfa57ef) )
	ROM_LOAD16_WORD_SWAP( "250_er01.p2", 0x100000, 0x400000, CRC(26c43520) SHA1(0001d35829021df68a65ded4bc2955c3c5d29f15) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er01.p1", 0x000000, 0x100000, CRC(9418f9d0) SHA1(fbe748a88355c10371c7040162ce6f202bfa57ef) )
	ROM_LOAD16_WORD_SWAP( "250_er02.p2", 0x100000, 0x400000, CRC(98cf700a) SHA1(bd95e398f22a5e5df1e402fef1db59a1a3236774) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er01.p1", 0x000000, 0x100000, CRC(9418f9d0) SHA1(fbe748a88355c10371c7040162ce6f202bfa57ef) )
	ROM_LOAD16_WORD_SWAP( "250_er03.p2", 0x100000, 0x400000, CRC(a5a51a43) SHA1(7e8d54b1b67a56c1696bd9fcd8fb862fc879fa7d) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er04.p1", 0x000000, 0x100000, CRC(70afde7f) SHA1(b814d926b62ad449921ebe01c3601f6e0dd9af9f) )
	ROM_LOAD16_WORD_SWAP( "250_er04.p2", 0x100000, 0x400000, CRC(b79b9f68) SHA1(aa33e9e450b8530b31af76aa55f5322ac5888e6e) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er05.p1", 0x000000, 0x100000, CRC(f3b06a59) SHA1(dfc8d1a6b08930a4e26f3f38fa532fe451c6414a) )
	ROM_LOAD16_WORD_SWAP( "250_er05.p2", 0x100000, 0x400000, CRC(8f1c7d24) SHA1(d32127573a7473a16d0b4c0f8e1cbae54aacd699) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer06 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er01.p1", 0x000000, 0x100000, CRC(9418f9d0) SHA1(fbe748a88355c10371c7040162ce6f202bfa57ef) )
	ROM_LOAD16_WORD_SWAP( "250_er06.p2", 0x100000, 0x400000, CRC(9edf51d1) SHA1(8b16e82c5b0c76c33350d3c4d69d5b07f20d1b96) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxer07 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_er01.p1", 0x000000, 0x100000, CRC(9418f9d0) SHA1(fbe748a88355c10371c7040162ce6f202bfa57ef) )
	ROM_LOAD16_WORD_SWAP( "250_er07.p2", 0x100000, 0x400000, CRC(68bd7a19) SHA1(f1c216e6fd23fb1fdfba50650989605f8cea2e2b) )
    ROM_DEFAULT_BIOS("euro")
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250_hacks31.s1", CRC(03bce893) SHA1(cd8dbddf8c173de8cd26566c4451ebc3568496ac) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXSC_SPRITES
ROM_END

ROM_START( mslugxesp01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p1", 0x000000, 0x100000, CRC(20a702e4) SHA1(f54e467785f6fa999c1d0af65e80536319f9051d) )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p2", 0x100000, 0x400000, CRC(c91741ea) SHA1(afe4bb43fc7e1139638d9dc5860a553df425755b) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p1", 0x000000, 0x100000, CRC(20a702e4) SHA1(f54e467785f6fa999c1d0af65e80536319f9051d) )
	ROM_LOAD16_WORD_SWAP( "250_esp02.p2", 0x100000, 0x400000, CRC(d86db57f) SHA1(f71149407ce21aef612ecfbf0f32a63fe7b2dd94) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p1", 0x000000, 0x100000, CRC(20a702e4) SHA1(f54e467785f6fa999c1d0af65e80536319f9051d) )
	ROM_LOAD16_WORD_SWAP( "250_esp03.p2", 0x100000, 0x400000, CRC(c414b0e5) SHA1(5be371a914ca891e34a041d4474ed890545e8435) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p1", 0x000000, 0x100000, CRC(20a702e4) SHA1(f54e467785f6fa999c1d0af65e80536319f9051d) )
	ROM_LOAD16_WORD_SWAP( "250_esp04.p2", 0x100000, 0x400000, CRC(5d641902) SHA1(72a98f498d19766f462cbae40de0ca639c9b2d2d) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp05.p1", 0x000000, 0x100000, CRC(f924e793) SHA1(ce452f222be7ac6c3d25e7a5aa7b6b44cf63d04c) )
	ROM_LOAD16_WORD_SWAP( "250_esp05.p2", 0x100000, 0x400000, CRC(0362bcda) SHA1(63438964dc31debd0ebec87946032f0e895b03ab) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
    MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp06 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp06.p1", 0x000000, 0x100000, CRC(e392d0a7) SHA1(8e28cb60b3814f8be90bcf221b0f76835e0d1244) )
	ROM_LOAD16_WORD_SWAP( "250_esp06.p2", 0x100000, 0x400000, CRC(7c3f1838) SHA1(04694d4f03d4152591013aafa7f1f318042e8e89) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxesp07 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_esp01.p1", 0x000000, 0x100000, CRC(20a702e4) SHA1(f54e467785f6fa999c1d0af65e80536319f9051d) )
	ROM_LOAD16_WORD_SWAP( "250_esp07.p2", 0x100000, 0x400000, CRC(cba0882b) SHA1(d2cc62d3be0df6520ee610c8c74023f977859ab1) )
	MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxtst01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1",  0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_tst01.p2", 0x100000, 0x400000, CRC(9ddd0f21) SHA1(bbeea5ed3c4021bd14249b416e25ff88731b8a65) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxtao01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1",  0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_ao01.p2",  0x100000, 0x400000, CRC(ee8f9d31) SHA1(d99ca53fc94b869b4ddd2d4de633a0a78827d01e) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1",  0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at01.p2",  0x100000, 0x400000, CRC(94fce740) SHA1(9cb3ac56df2104eb2b623eb238fd3734df46e52d) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at02.p2", 0x100000, 0x400000, CRC(0da76952) SHA1(d2544a4153fa8e6edd593bd9b672f76c73a9f123) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at03.p2", 0x100000, 0x400000, CRC(97839838) SHA1(1186ba34ca4423ee0fa19db19d9ae79811c8117d) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at04.p2", 0x100000, 0x400000, CRC(1ad67b97) SHA1(70ec0cc7f4316027dcd4108bcbc06780df69067a) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at05.p2", 0x100000, 0x400000, CRC(8713362a) SHA1(52f4fd9f34aa9738f30fec6e418163ebf3cf8417) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat06 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at06.p2", 0x100000, 0x400000, CRC(e17e2ba5) SHA1(5ded8fc351926555c71edbab66e7b6e8a82729a0) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat07 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at01.p1", 0x000000, 0x100000, CRC(5acf6748) SHA1(47e6694c409f2de0081fb5867692836271626dcf) )
	ROM_LOAD16_WORD_SWAP( "250_at07.p2", 0x100000, 0x400000, CRC(67e0d521) SHA1(aac12a294c031fe8b06228ee2a01b87537d5b49a) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat08 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at08.p1", 0x000000, 0x100000, CRC(420ae33b) SHA1(aea6349bd782e580b9044f637c01611d87acb65e) )
	ROM_LOAD16_WORD_SWAP( "250_at08.p2", 0x100000, 0x400000, CRC(96cda2b9) SHA1(d9eff5de6578c04d05110239c61b3390f35e2547) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGX_SPRITES
ROM_END

ROM_START( mslugxat09 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at09.p1", 0x000000, 0x100000, CRC(ab80c720) SHA1(d868164c5ced1a6e3f61f95bc60ea384fcadafe5) )
	ROM_LOAD16_WORD_SWAP( "250_at09.p2", 0x100000, 0x400000, CRC(9d8ec80f) SHA1(9b75d57a7e71574144936c05073e8538ad2fed83) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxat10 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_at10.p1", 0x000000, 0x100000, CRC(d3c7a785) SHA1(5c2b6ccf0cbfb9daa5e23516c222f41ec64860b1) )
	ROM_LOAD16_WORD_SWAP( "250_at10.p2", 0x100000, 0x400000, CRC(fd7f67ae) SHA1(2f60ae3a130d766acf21c89d6b557018e30c5252) )
    MSLUGX_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxla01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la01.p1", 0x000000, 0x100000, CRC(69cb78db) SHA1(dea90391d6d73fbd88cbae39bcc8e0de0be14d17) )
	ROM_LOAD16_WORD_SWAP( "250_la01.p2", 0x100000, 0x400000, CRC(a67301ac) SHA1(6ee37e924db019f7b3d5ffc5e8eeeaa1376ca86e) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxla02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la02.p1", 0x000000, 0x100000, CRC(6e558590) SHA1(4993372f60f792f352779c7b0e047b479fcbcc15) )
	ROM_LOAD16_WORD_SWAP( "250_la01.p2", 0x100000, 0x400000, CRC(a67301ac) SHA1(6ee37e924db019f7b3d5ffc5e8eeeaa1376ca86e) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxla03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la03.p1", 0x000000, 0x100000, CRC(81ea65d6) SHA1(2af69efc6ba86cbdf94114c3d02837db11231626) )
	ROM_LOAD16_WORD_SWAP( "250_la01.p2", 0x100000, 0x400000, CRC(a67301ac) SHA1(6ee37e924db019f7b3d5ffc5e8eeeaa1376ca86e) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlb01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lb01.p1", 0x000000, 0x100000, CRC(1f04d4f0) SHA1(3019a9b2d97e952073fa83f2472c8090e685dab3) )
	ROM_LOAD16_WORD_SWAP( "250_lb01.p2", 0x100000, 0x400000, CRC(b709f539) SHA1(59a1fb08724957354c885d7160e6aad1dd2b7130) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlb02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la02.p1", 0x000000, 0x100000, CRC(6e558590) SHA1(4993372f60f792f352779c7b0e047b479fcbcc15) )
	ROM_LOAD16_WORD_SWAP( "250_lb01.p2", 0x100000, 0x400000, CRC(b709f539) SHA1(59a1fb08724957354c885d7160e6aad1dd2b7130) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlb03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la03.p1", 0x000000, 0x100000, CRC(81ea65d6) SHA1(2af69efc6ba86cbdf94114c3d02837db11231626) )
	ROM_LOAD16_WORD_SWAP( "250_lb01.p2", 0x100000, 0x400000, CRC(b709f539) SHA1(59a1fb08724957354c885d7160e6aad1dd2b7130) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlc01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lc01.p1", 0x000000, 0x100000, CRC(5947628f) SHA1(ec1c19238f51df6475f1fe356fdda34f46e01dc5) )
	ROM_LOAD16_WORD_SWAP( "250_lc01.p2", 0x100000, 0x400000, CRC(ab70f0a3) SHA1(7de44785b78d0fbb7b606578b710783cd1928953) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlc02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la02.p1", 0x000000, 0x100000, CRC(6e558590) SHA1(4993372f60f792f352779c7b0e047b479fcbcc15) )
	ROM_LOAD16_WORD_SWAP( "250_lc01.p2", 0x100000, 0x400000, CRC(ab70f0a3) SHA1(7de44785b78d0fbb7b606578b710783cd1928953) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlc03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la03.p1", 0x000000, 0x100000, CRC(81ea65d6) SHA1(2af69efc6ba86cbdf94114c3d02837db11231626) )
	ROM_LOAD16_WORD_SWAP( "250_lc01.p2", 0x100000, 0x400000, CRC(ab70f0a3) SHA1(7de44785b78d0fbb7b606578b710783cd1928953) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxld01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_ld01.p1", 0x000000, 0x100000, CRC(1927b712) SHA1(a5f0a66a76874a08881b9121bd7764e002ac801a) )
	ROM_LOAD16_WORD_SWAP( "250_ld01.p2", 0x100000, 0x400000, CRC(d30ef51a) SHA1(737c1aad94d17477206b1379080f3e3ec3fc345b) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxld02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_ld02.p1", 0x000000, 0x100000, CRC(0f29ae77) SHA1(62619fcd1f59ec14bc1d4fb8aa230bcc1c11c87b) )
	ROM_LOAD16_WORD_SWAP( "250_ld01.p2", 0x100000, 0x400000, CRC(d30ef51a) SHA1(737c1aad94d17477206b1379080f3e3ec3fc345b) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxld03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_ld03.p1", 0x000000, 0x100000, CRC(aacc2a32) SHA1(57f6fe2b7ba6d89f906ea4c9c3ef2982450c645a) )
	ROM_LOAD16_WORD_SWAP( "250_ld01.p2", 0x100000, 0x400000, CRC(d30ef51a) SHA1(737c1aad94d17477206b1379080f3e3ec3fc345b) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxle01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_le01.p1", 0x000000, 0x100000, CRC(0493c774) SHA1(b217c90977d46bf4a4bfaf6d27a846398cbb0fc0) )
	ROM_LOAD16_WORD_SWAP( "250_le01.p2", 0x100000, 0x400000, CRC(32005944) SHA1(9acdbae063c1f780e9873b24ad2f164318bf339d) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxle02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_le02.p1", 0x000000, 0x100000, CRC(012afb27) SHA1(fe5a1c0cfd9f1486d0c9534156b2dbbe1040a430) )
	ROM_LOAD16_WORD_SWAP( "250_le01.p2", 0x100000, 0x400000, CRC(32005944) SHA1(9acdbae063c1f780e9873b24ad2f164318bf339d) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxle03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_le03.p1", 0x000000, 0x100000, CRC(ee951b61) SHA1(f7fefe25d84ac78171e20a9758d06d80b4d5a4f2) )
	ROM_LOAD16_WORD_SWAP( "250_le01.p2", 0x100000, 0x400000, CRC(32005944) SHA1(9acdbae063c1f780e9873b24ad2f164318bf339d) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlf01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lf01.p1", 0x000000, 0x100000, CRC(b0489dac) SHA1(50d29bf8035702c3e35697aa301e93ca90ef657b) )
	ROM_LOAD16_WORD_SWAP( "250_lf01.p2", 0x100000, 0x400000, CRC(6c06fc9c) SHA1(c6df84e0027a605a7b693c14fdb1b64ae7bb33d3) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlf02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lf02.p1", 0x000000, 0x100000, CRC(b7d660e7) SHA1(689dd53a1c835dcc1bd51ada2a6c1733b9c166fd) )
	ROM_LOAD16_WORD_SWAP( "250_lf01.p2", 0x100000, 0x400000, CRC(6c06fc9c) SHA1(c6df84e0027a605a7b693c14fdb1b64ae7bb33d3) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlf03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lf03.p1", 0x000000, 0x100000, CRC(586980a1) SHA1(cd1a0b4819b6ace132aa36d407922eafe9a65b34) )
	ROM_LOAD16_WORD_SWAP( "250_lf01.p2", 0x100000, 0x400000, CRC(6c06fc9c) SHA1(c6df84e0027a605a7b693c14fdb1b64ae7bb33d3) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlg01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lg01.p1", 0x000000, 0x100000, CRC(537e01c2) SHA1(5b910e6d87e914f8c48737fcd268cd3babd2c313) )
	ROM_LOAD16_WORD_SWAP( "250_lg01.p2", 0x100000, 0x400000, CRC(42a873bf) SHA1(bc06115386798aea79d9f803bc2960ba50dc8001) )
    MSLUGXRM_MAINCPU
	
	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlg02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la02.p1", 0x000000, 0x100000, CRC(6e558590) SHA1(4993372f60f792f352779c7b0e047b479fcbcc15) )
	ROM_LOAD16_WORD_SWAP( "250_lg01.p2", 0x100000, 0x400000, CRC(42a873bf) SHA1(bc06115386798aea79d9f803bc2960ba50dc8001) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlg03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la03.p1", 0x000000, 0x100000, CRC(81ea65d6) SHA1(2af69efc6ba86cbdf94114c3d02837db11231626) )
	ROM_LOAD16_WORD_SWAP( "250_lg01.p2", 0x100000, 0x400000, CRC(42a873bf) SHA1(bc06115386798aea79d9f803bc2960ba50dc8001) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlh01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_lh01.p1", 0x000000, 0x100000, CRC(3444a087) SHA1(f5dbf2167aa3174bcf45b089b9c168ef217dfdb2) )
	ROM_LOAD16_WORD_SWAP( "250_lh01.p2", 0x100000, 0x400000, CRC(a4c4c86d) SHA1(172c68613fe04fcf20f08c0528981d80b4e5974a) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlh02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la02.p1", 0x000000, 0x100000, CRC(6e558590) SHA1(4993372f60f792f352779c7b0e047b479fcbcc15) )
	ROM_LOAD16_WORD_SWAP( "250_lh01.p2", 0x100000, 0x400000, CRC(a4c4c86d) SHA1(172c68613fe04fcf20f08c0528981d80b4e5974a) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxlh03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_la03.p1", 0x000000, 0x100000, CRC(81ea65d6) SHA1(2af69efc6ba86cbdf94114c3d02837db11231626) )
	ROM_LOAD16_WORD_SWAP( "250_lh01.p2", 0x100000, 0x400000, CRC(a4c4c86d) SHA1(172c68613fe04fcf20f08c0528981d80b4e5974a) )
    MSLUGXRM_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe0s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p1", 0x000000, 0x100000, CRC(e10b07d0) SHA1(eb7ec1fa10bcec15e5117d88144ca03da0ba53af) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p2", 0x100000, 0x400000, CRC(228c9f6d) SHA1(e1a298ee901bf7ce8077da6038d9f03d213e61b6) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe0s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s02.p1", 0x000000, 0x100000, CRC(caed963b) SHA1(54e7e464744e2ee2cdf66b73a4092b8c6a15a09b) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p2", 0x100000, 0x400000, CRC(228c9f6d) SHA1(e1a298ee901bf7ce8077da6038d9f03d213e61b6) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe0s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s03.p1", 0x000000, 0x100000, CRC(d7f96b62) SHA1(166415cc1c2b3321fcd71cd7fa8adc3cfda2df6f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p2", 0x100000, 0x400000, CRC(228c9f6d) SHA1(e1a298ee901bf7ce8077da6038d9f03d213e61b6) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe1s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p1", 0x000000, 0x100000, CRC(e10b07d0) SHA1(eb7ec1fa10bcec15e5117d88144ca03da0ba53af) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe1s01.p2", 0x100000, 0x400000, CRC(33f66bf8) SHA1(a0f3adef3521537a63908ceb8ef765573e878f91) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe1s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s02.p1", 0x000000, 0x100000, CRC(caed963b) SHA1(54e7e464744e2ee2cdf66b73a4092b8c6a15a09b) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe1s01.p2", 0x100000, 0x400000, CRC(33f66bf8) SHA1(a0f3adef3521537a63908ceb8ef765573e878f91) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe1s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s03.p1", 0x000000, 0x100000, CRC(d7f96b62) SHA1(166415cc1c2b3321fcd71cd7fa8adc3cfda2df6f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe1s01.p2", 0x100000, 0x400000, CRC(33f66bf8) SHA1(a0f3adef3521537a63908ceb8ef765573e878f91) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe2s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p1", 0x000000, 0x100000, CRC(e10b07d0) SHA1(eb7ec1fa10bcec15e5117d88144ca03da0ba53af) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe2s01.p2", 0x100000, 0x400000, CRC(2f8f6e62) SHA1(c64f52d9a20eb50b6edd388dd384d015f958ba9f) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe2s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s02.p1", 0x000000, 0x100000, CRC(caed963b) SHA1(54e7e464744e2ee2cdf66b73a4092b8c6a15a09b) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe2s01.p2", 0x100000, 0x400000, CRC(2f8f6e62) SHA1(c64f52d9a20eb50b6edd388dd384d015f958ba9f) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe2s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s03.p1", 0x000000, 0x100000, CRC(d7f96b62) SHA1(166415cc1c2b3321fcd71cd7fa8adc3cfda2df6f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe2s01.p2", 0x100000, 0x400000, CRC(2f8f6e62) SHA1(c64f52d9a20eb50b6edd388dd384d015f958ba9f) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe3s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s01.p1", 0x000000, 0x100000, CRC(1934b131) SHA1(b68ebd76fa5bd64c9e1b0aaa3daba0b23f6c256f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s01.p2", 0x100000, 0x400000, CRC(57f16bdb) SHA1(6b4f07d5d6a2668e25598f64a8a6eb29b5046ea3) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe3s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s02.p1", 0x000000, 0x100000, CRC(2ee46c37) SHA1(7c29e5f4344b831ddfaa727fdc3bb34ccf38ebb4) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s01.p2", 0x100000, 0x400000, CRC(57f16bdb) SHA1(6b4f07d5d6a2668e25598f64a8a6eb29b5046ea3) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe3s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s03.p1", 0x000000, 0x100000, CRC(7869b951) SHA1(1445c4fc8ec6fc89cdf768f38098cb7f0cf7a0b9) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe3s01.p2", 0x100000, 0x400000, CRC(57f16bdb) SHA1(6b4f07d5d6a2668e25598f64a8a6eb29b5046ea3) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe4s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s01.p1", 0x000000, 0x100000, CRC(8e747967) SHA1(08629a42833005b670f50b27faafa829c606df20) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s01.p2", 0x100000, 0x400000, CRC(b6ffc785) SHA1(caea53d1c35a5cd724680581094fb37bafa93d47) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe4s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s02.p1", 0x000000, 0x100000, CRC(a592e88c) SHA1(d318562e9b067d54849cf3a70e97e9168d83e843) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s01.p2", 0x100000, 0x400000, CRC(b6ffc785) SHA1(caea53d1c35a5cd724680581094fb37bafa93d47) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe4s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s03.p1", 0x000000, 0x100000, CRC(b88615d5) SHA1(38036a9eb8464642d9ef521f1b5b3e4a285523ce) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe4s01.p2", 0x100000, 0x400000, CRC(b6ffc785) SHA1(caea53d1c35a5cd724680581094fb37bafa93d47) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe5s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s01.p1", 0x000000, 0x100000, CRC(3888e2a7) SHA1(1dd3cbc8312703369d4af5d55327280091728318) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s01.p2", 0x100000, 0x400000, CRC(e8f9625d) SHA1(ac6b2304140d928b2c9212f1a68f0d861a904ca1) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe5s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s02.p1", 0x000000, 0x100000, CRC(136e734c) SHA1(0cb01f1292939ede58dc252cf9045c708f1e11b0) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s01.p2", 0x100000, 0x400000, CRC(e8f9625d) SHA1(ac6b2304140d928b2c9212f1a68f0d861a904ca1) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe5s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s03.p1", 0x000000, 0x100000, CRC(0e7a8e15) SHA1(8aea257e5ebaae47b19e8b4a77784bdc9c9060c3) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe5s01.p2", 0x100000, 0x400000, CRC(e8f9625d) SHA1(ac6b2304140d928b2c9212f1a68f0d861a904ca1) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250_hacks06.s1", CRC(a2bb2551) SHA1(fa22568bf15ac41afdf5e2bd7dc2dec4fe813f02) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe6s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p1", 0x000000, 0x100000, CRC(e10b07d0) SHA1(eb7ec1fa10bcec15e5117d88144ca03da0ba53af) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe6s01.p2", 0x100000, 0x400000, CRC(2be58ede) SHA1(12556c287be74be3ebdcb7698964d51ce7ce5d2a) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe6s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s02.p1", 0x000000, 0x100000, CRC(caed963b) SHA1(54e7e464744e2ee2cdf66b73a4092b8c6a15a09b) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe6s01.p2", 0x100000, 0x400000, CRC(2be58ede) SHA1(12556c287be74be3ebdcb7698964d51ce7ce5d2a) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe6s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s03.p1", 0x000000, 0x100000, CRC(d7f96b62) SHA1(166415cc1c2b3321fcd71cd7fa8adc3cfda2df6f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe6s01.p2", 0x100000, 0x400000, CRC(2be58ede) SHA1(12556c287be74be3ebdcb7698964d51ce7ce5d2a) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe7s01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s01.p1", 0x000000, 0x100000, CRC(e10b07d0) SHA1(eb7ec1fa10bcec15e5117d88144ca03da0ba53af) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe7s01.p2", 0x100000, 0x400000, CRC(203b56ac) SHA1(22d48572fe2b7e771c777434d2ca06e8dfc869c7) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe7s02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s02.p1", 0x000000, 0x100000, CRC(caed963b) SHA1(54e7e464744e2ee2cdf66b73a4092b8c6a15a09b) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe7s01.p2", 0x100000, 0x400000, CRC(203b56ac) SHA1(22d48572fe2b7e771c777434d2ca06e8dfc869c7) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

ROM_START( mslugxrmxe7s03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "250_rmxe0s03.p1", 0x000000, 0x100000, CRC(d7f96b62) SHA1(166415cc1c2b3321fcd71cd7fa8adc3cfda2df6f) )
	ROM_LOAD16_WORD_SWAP( "250_rmxe7s01.p2", 0x100000, 0x400000, CRC(203b56ac) SHA1(22d48572fe2b7e771c777434d2ca06e8dfc869c7) )
    MSLUGXRMXE_MAINCPU

	NEO_SFIX_128K( "250.s1", CRC(fb6f441d) SHA1(2cc392ecde5d5afb28ddbaa1030552b48571dcfb) )

	NEO_BIOS_AUDIO_128K( "250.m1", CRC(fd42a842) SHA1(55769bad4860f64ef53a333e0da9e073db483d6a) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	MSLUGX_YMSND

	ROM_REGION( 0x3000000, "sprites", 0 )
	MSLUGXLB_SPRITES
ROM_END

 /**************
 Mutation Nation
*****************/

ROM_START( mslughw ) //mslug5ast
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "268_hw.p1", 0x000000, 0x400000, CRC(1e0506cf) SHA1(f1a20f1f0fc3fd74dd5d258eaf75965eb2b4cd91) )
	ROM_LOAD32_WORD_SWAP( "268_hw.p2", 0x000002, 0x400000, CRC(894cd3dd) SHA1(553f90fce8c04940892bd797dc4f1dddf7276c81) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "268_hw.m1", CRC(876df5e1) SHA1(a54bd3eb76d9768fec26fbf4153c0a6055df159f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "268_hw.v1", 0x000000, 0x800000, CRC(5d904213) SHA1(62bf1a832c14203ce913f6ac3dc5df75d141155a) )
	ROM_LOAD( "268_hw.v2", 0x800000, 0x800000, CRC(97cee550) SHA1(b4429e71847ead15b146754f35023efa276253a7) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "268_hw.c1", 0x000000, 0x800000, CRC(92a89586) SHA1(9d6dca9fd73cdd6d382d26708c632ec01654c050) )
	ROM_LOAD16_BYTE( "268_hw.c2", 0x000001, 0x800000, CRC(18d14a71) SHA1(95e7518091fe5600bd5fb5c90fcec22670bf8910) )
ROM_END

 /**************
 Mutation Nation
*****************/

ROM_START( mutnathack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "014_hack01.p1", 0x000000, 0x080000, CRC(2fe45544) SHA1(e55445f5a31c11bc11d74d040f8c4e605a7d4d36) )

	NEO_SFIX_128K( "014.s1", CRC(99419733) SHA1(b2524af8704941acc72282aa1d62fd4c93e3e822) )

	NEO_BIOS_AUDIO_128K( "014.m1", CRC(b6683092) SHA1(623ec7ec2915fb077bf65b4a16c815e071c25259) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "014.v1", 0x000000, 0x100000, CRC(25419296) SHA1(c9fc04987c4e0875d276e1a0fb671740b6f548ad) )
	ROM_LOAD( "014.v2", 0x100000, 0x100000, CRC(0de53d5e) SHA1(467f6040da3dfb1974785e95e14c3f608a93720a) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "014.c1", 0x000000, 0x100000, CRC(5e4381bf) SHA1(d429a5e09dafd2fb99495658b3652eecbf58f91b) )
	ROM_LOAD16_BYTE( "014.c2", 0x000001, 0x100000, CRC(69ba4e18) SHA1(b3369190c47771a790c7adffa958ff55d90e758b) )
	ROM_LOAD16_BYTE( "014.c3", 0x200000, 0x100000, CRC(890327d5) SHA1(47f97bf120a8480758e1f3bb8982be4c5325c036) )
	ROM_LOAD16_BYTE( "014.c4", 0x200001, 0x100000, CRC(e4002651) SHA1(17e53a5f4708866a120415bf24f3b89621ad0bcc) )
ROM_END

 /********
 NAM-1975
***********/

ROM_START( nam1975hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "001_hack01.p1", 0x000000, 0x080000, CRC(908a4fd0) SHA1(37ffc705ee772a1a25e77ff8696750a78dacc7f7) )

	NEO_SFIX_64K( "001_hack01.s1", CRC(8ded55a5) SHA1(27c8ffac16fbcc19c843d1b5b47ae8d8ef83d44a) )

	NEO_BIOS_AUDIO_64K( "001_hack01.m1", CRC(cd088502) SHA1(939f745cd576905f326e6246a2eed78e9fa88178) )

	ROM_REGION( 0x080000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "001.v1", 0x000000, 0x080000, CRC(a7c3d5e5) SHA1(e3efc86940f91c53b7724c4566cfc21ea1a7a465) )

	ROM_REGION( 0x180000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "001.v21", 0x000000, 0x080000, CRC(55e670b3) SHA1(a047049646a90b6db2d1882264df9256aa5a85e5) )
	ROM_LOAD( "001.v22", 0x080000, 0x080000, CRC(ab0d8368) SHA1(404114db9f3295929080b87a5d0106b40da6223a) )
	ROM_LOAD( "001.v23", 0x100000, 0x080000, CRC(df468e28) SHA1(4e5d4a709a4737a87bba4083aeb788f657862f1a) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "001.c1", 0x000000, 0x80000, CRC(32ea98e1) SHA1(c2fb3fb7dd14523a4b4b7fbdb81f44cb4cc48239) )
	ROM_LOAD16_BYTE( "001.c2", 0x000001, 0x80000, CRC(cbc4064c) SHA1(224c970fd060d841fd430c946ef609bb57b6d78c) )
	ROM_LOAD16_BYTE( "001.c3", 0x100000, 0x80000, CRC(0151054c) SHA1(f24fb501a7845f64833f4e5a461bcf9dc3262557) )
	ROM_LOAD16_BYTE( "001.c4", 0x100001, 0x80000, CRC(0a32570d) SHA1(f108446ec7844fde25f7a4ab454f76d384bf5e52) )
	ROM_LOAD16_BYTE( "001.c5", 0x200000, 0x80000, CRC(90b74cc2) SHA1(89898da36db259180e5261ed45eafc99ca13e504) )
	ROM_LOAD16_BYTE( "001.c6", 0x200001, 0x80000, CRC(e62bed58) SHA1(d05b2903b212a51ee131e52c761b714cb787683e) )
ROM_END

 /************
 Ninja Combat
***************/

ROM_START( ncombathack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "009_hack01.p1", 0x000000, 0x080000, CRC(4e040e04) SHA1(8a9281c745810e6977ff4d304e6fb3f8441fe4a4) )

	NEO_SFIX_128K( "009.s1", CRC(d49afee8) SHA1(77615f12edf08ae8f1353f7a056a8f3a50d3ebdc) )

	NEO_BIOS_AUDIO_128K( "009.m1", CRC(b5819863) SHA1(6f2309d51531052dbf7d712993c9e35649db0d84) )

	ROM_REGION( 0x180000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "009.v11", 0x000000, 0x080000, CRC(cf32a59c) SHA1(af5b7fcd8a4aff1307c0a1d937e5f0460c32de79) )
	ROM_LOAD( "009.v12", 0x080000, 0x080000, CRC(7b3588b7) SHA1(a4e6d9d4113ff4ce48b371f65e9187d551821d3b) )
	ROM_LOAD( "009.v13", 0x100000, 0x080000, CRC(505a01b5) SHA1(9426a4f5b31e16f74e72e61951c189a878f211c5) )

	ROM_REGION( 0x080000, "ymsnd:adpcmb", 0 )
	ROM_LOAD( "009.v21", 0x000000, 0x080000, CRC(365f9011) SHA1(aebd292214ab280b05ee9e759b7e9a681a099c4a) )

	ROM_REGION( 0x300000, "sprites", 0 )
	ROM_LOAD16_BYTE( "009.c1", 0x000000, 0x80000, CRC(33cc838e) SHA1(c445c891c0ba4190aa0b472786150620e76df5b4) )
	ROM_LOAD16_BYTE( "009.c2", 0x000001, 0x80000, CRC(26877feb) SHA1(8f48097fb8e4757f50b6d86219122fbf4b6f87ef) )
	ROM_LOAD16_BYTE( "009.c3", 0x100000, 0x80000, CRC(3b60a05d) SHA1(0a165a17af4834876fcd634599cd2208adc9248f) )
	ROM_LOAD16_BYTE( "009.c4", 0x100001, 0x80000, CRC(39c2d039) SHA1(8ca6c3f977c43c7abba2a00a0e70f02e2a49f5f2) )
	ROM_LOAD16_BYTE( "009.c5", 0x200000, 0x80000, CRC(67a4344e) SHA1(b325f152c7b2388fc92c5826e1dc99094b9ea749) )
	ROM_LOAD16_BYTE( "009.c6", 0x200001, 0x80000, CRC(2eca8b19) SHA1(16764ef10e404325ba0a1a2ad3a4c0af287be21f) )
ROM_END

 /*************
 Ninja Commando
****************/

ROM_START( ncommandhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "050_hack01.p1", 0x000000, 0x100000, CRC(76d37e41) SHA1(b5f6a8861a197be6f20ddbde28cc00f1e9176a2c) )

	NEO_SFIX_128K( "050.s1", CRC(db8f9c8e) SHA1(11cb82cf3c4d0fc2da5df0c26410a64808093610) )

	NEO_BIOS_AUDIO_128K( "050.m1", CRC(6fcf07d3) SHA1(e9ecff4bfec1f5964bf06645f75d80d611b6231c) )

	ROM_REGION( 0x180000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "050.v1", 0x000000, 0x100000, CRC(23c3ab42) SHA1(b6c59bb180f1aa34c95f3ec923f3aafb689d57b0) )
	ROM_LOAD( "050.v2", 0x100000, 0x080000, CRC(80b8a984) SHA1(950cf0e78ceffa4037663f1086fbbc88588f49f2) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "050.c1", 0x000000, 0x100000, CRC(87421a0a) SHA1(1d8faaf03778f7c5b062554d7333bbd3f0ca12ad) )
	ROM_LOAD16_BYTE( "050.c2", 0x000001, 0x100000, CRC(c4cf5548) SHA1(ef9eca5aeff9dda2209a050c2af00ed8979ae2bc) )
	ROM_LOAD16_BYTE( "050.c3", 0x200000, 0x100000, CRC(03422c1e) SHA1(920e5015aebe2ffc5ce43a52365c7f0a705f3b9e) )
	ROM_LOAD16_BYTE( "050.c4", 0x200001, 0x100000, CRC(0845eadb) SHA1(3c71a02bf0e07a5381846bb6d75bbe7dd546adea) )
ROM_END

 /*************
 Neo Bomberman
****************/

ROM_START( neobombehack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "093_hack01.p1", 0x000000, 0x100000, CRC(8426e714) SHA1(917361f9663f16b541e938c86ca7eb442d40b4ed) )

	NEO_SFIX_128K( "093.s1", CRC(4b3fa119) SHA1(41cb0909bfb017eb6f2c530cb92a423319ed7ab1) )

	NEO_BIOS_AUDIO_128K( "093.m1", CRC(e81e780b) SHA1(c56c53984e0f92e180e850c60a75f550ee84917c) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "093.v1", 0x000000, 0x400000, CRC(02abd4b0) SHA1(6bf33ebc9b01cd4a029f6a555694a9835e30ca1b) )
	ROM_LOAD( "093.v2", 0x400000, 0x200000, CRC(a92b8b3d) SHA1(b672c97b85d2f52eba3cb26025008ebc7a18312a) )

	ROM_REGION( 0x900000, "sprites", 0 )
	ROM_LOAD16_BYTE( "093.c1", 0x000000, 0x400000, CRC(d1f328f8) SHA1(ddf71280c2ce85225f15fe9e973f330609281878) )
	ROM_LOAD16_BYTE( "093.c2", 0x000001, 0x400000, CRC(82c49540) SHA1(5f37c1bc0d63c98a13967b44da3d2c85e6dbbe50) )
	ROM_LOAD16_BYTE( "093.c3", 0x800000, 0x080000, CRC(e37578c5) SHA1(20024caa0f09ee887a6418dd02d02a0df93786fd) )
	ROM_LOAD16_BYTE( "093.c4", 0x800001, 0x080000, CRC(59826783) SHA1(0110a2b6186cca95f75225d4d0269d61c2ad25b1) )
ROM_END

 /***************
 Neo-Geo Cup '98
******************/

 /**************
 Ninja Master's
*****************/

ROM_START( ninjamashack01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "217_hack01.p1",  0x000000, 0x100000, CRC(661d86c9) SHA1(8a8906e0fe2b1fc2e7bd69cd7f2dc7c7f8683331) )
	ROM_LOAD16_WORD_SWAP( "217.p2", 0x100000, 0x200000, CRC(191fca88) SHA1(e318e5931704779bbe461719a5eeeba89bd83a5d) )

	NEO_SFIX_128K( "217.s1", CRC(8ff782f0) SHA1(90099c154357042ba658d4ef6abe4d9335bb7172) )

	NEO_BIOS_AUDIO_128K( "217.m1", CRC(d00fb2af) SHA1(6bcaa52e1641cc24288e1f22f4dc98e8d8921b90) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "217.v1", 0x000000, 0x400000, CRC(1c34e013) SHA1(5368e413d2188c4fd063b6bb7d5f498ff83ea812) )
	ROM_LOAD( "217.v2", 0x400000, 0x200000, CRC(22f1c681) SHA1(09da03b2e63d180e55173ff25e8735c4162f027b) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "217.c1", 0x0000000, 0x400000, CRC(5fe97bc4) SHA1(d76c955d83baa2b9fd24222a9b2852947b7b92f0) )
	ROM_LOAD16_BYTE( "217.c2", 0x0000001, 0x400000, CRC(886e0d66) SHA1(d407e1525e4ebe996e14f6e5c0396a10f736a50d) )
	ROM_LOAD16_BYTE( "217.c3", 0x0800000, 0x400000, CRC(59e8525f) SHA1(19f602c71545d6c021dc72e112d3a8b8efe7a9b7) )
	ROM_LOAD16_BYTE( "217.c4", 0x0800001, 0x400000, CRC(8521add2) SHA1(0d1a6f2979302c4c282e31ff334d2d887aec74f7) )
	ROM_LOAD16_BYTE( "217.c5", 0x1000000, 0x400000, CRC(fb1896e5) SHA1(777a8caa9ebdbddf89e3d5ab650c94a55228ce54) )
	ROM_LOAD16_BYTE( "217.c6", 0x1000001, 0x400000, CRC(1c98c54b) SHA1(cb1cad161d9b9f2f5a7cf8ae4d6d35b51acf90f5) )
	ROM_LOAD16_BYTE( "217.c7", 0x1800000, 0x400000, CRC(8b0ede2e) SHA1(ea632ac98291ddac95441b7fe2349974b2da8a42) )
	ROM_LOAD16_BYTE( "217.c8", 0x1800001, 0x400000, CRC(a085bb61) SHA1(6a3e9e6ba96072b8849b407f2b24103dc0852259) )
ROM_END

ROM_START( ninjamashack02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "217_hack02.p1",  0x000000, 0x100000, CRC(c92bd6a4) SHA1(2176c2a0a5a0982d0077f68203d67e26136e3a7b) )
	ROM_LOAD16_WORD_SWAP( "217.p2", 0x100000, 0x200000, CRC(191fca88) SHA1(e318e5931704779bbe461719a5eeeba89bd83a5d) )

	NEO_SFIX_128K( "217.s1", CRC(8ff782f0) SHA1(90099c154357042ba658d4ef6abe4d9335bb7172) )

	NEO_BIOS_AUDIO_128K( "217.m1", CRC(d00fb2af) SHA1(6bcaa52e1641cc24288e1f22f4dc98e8d8921b90) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "217.v1", 0x000000, 0x400000, CRC(1c34e013) SHA1(5368e413d2188c4fd063b6bb7d5f498ff83ea812) )
	ROM_LOAD( "217.v2", 0x400000, 0x200000, CRC(22f1c681) SHA1(09da03b2e63d180e55173ff25e8735c4162f027b) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "217.c1", 0x0000000, 0x400000, CRC(5fe97bc4) SHA1(d76c955d83baa2b9fd24222a9b2852947b7b92f0) )
	ROM_LOAD16_BYTE( "217.c2", 0x0000001, 0x400000, CRC(886e0d66) SHA1(d407e1525e4ebe996e14f6e5c0396a10f736a50d) )
	ROM_LOAD16_BYTE( "217.c3", 0x0800000, 0x400000, CRC(59e8525f) SHA1(19f602c71545d6c021dc72e112d3a8b8efe7a9b7) )
	ROM_LOAD16_BYTE( "217.c4", 0x0800001, 0x400000, CRC(8521add2) SHA1(0d1a6f2979302c4c282e31ff334d2d887aec74f7) )
	ROM_LOAD16_BYTE( "217.c5", 0x1000000, 0x400000, CRC(fb1896e5) SHA1(777a8caa9ebdbddf89e3d5ab650c94a55228ce54) )
	ROM_LOAD16_BYTE( "217.c6", 0x1000001, 0x400000, CRC(1c98c54b) SHA1(cb1cad161d9b9f2f5a7cf8ae4d6d35b51acf90f5) )
	ROM_LOAD16_BYTE( "217.c7", 0x1800000, 0x400000, CRC(8b0ede2e) SHA1(ea632ac98291ddac95441b7fe2349974b2da8a42) )
	ROM_LOAD16_BYTE( "217.c8", 0x1800001, 0x400000, CRC(a085bb61) SHA1(6a3e9e6ba96072b8849b407f2b24103dc0852259) )
ROM_END

/**********************
 Nightmare in the Dark
************************/

ROM_START( nitdhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "260_hack01.p1", 0x000000, 0x080000, CRC(b294c6ce) SHA1(77f641c05a645e8d57014540ef1b7f52c4a8849b) )

	NEO_SFIX_128K( "260_hack01.s1", CRC(dd3bf47c) SHA1(881271caee6508b8be51bf1b59c8f1e58e08e551) )

	NEO_BIOS_AUDIO_512K( "260.m1", CRC(6407c5e5) SHA1(d273e154cc905b63205a17a1a6d419cac3485a92) )

	ROM_REGION( 0x0400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "260.v1", 0x000000, 0x400000, CRC(24b0480c) SHA1(d769e621be52a5cd2e2568891b5f95a48268e1e0) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "260_hack01.c1", 0x0000000, 0x800000, CRC(14c5a25f) SHA1(8b29f7e6920eb1b4a7dc42a063e4c8e6ec9a5700) )
	ROM_LOAD16_BYTE( "260_hack01.c2", 0x0000001, 0x800000, CRC(6630c255) SHA1(c8a77a3d6e17e1ed64eb6b1e85172567e7e3e71c) )
ROM_END

ROM_START( nitdhack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "260_hack02.p1", 0x000000, 0x080000, CRC(42b69e41) SHA1(cb814190d96ef67096ee9f4f305611d870a76e93) )

	NEO_SFIX_128K( "260_hack01.s1", CRC(dd3bf47c) SHA1(881271caee6508b8be51bf1b59c8f1e58e08e551) )

	NEO_BIOS_AUDIO_512K( "260.m1", CRC(6407c5e5) SHA1(d273e154cc905b63205a17a1a6d419cac3485a92) )

	ROM_REGION( 0x0400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "260.v1", 0x000000, 0x400000, CRC(24b0480c) SHA1(d769e621be52a5cd2e2568891b5f95a48268e1e0) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "260_hack02.c1", 0x0000000, 0x800000, CRC(3f7fbd0c) SHA1(2e6cedcb099ae8f5bbdb9d989ca3918d7fb0504f) )
	ROM_LOAD16_BYTE( "260_hack02.c2", 0x0000001, 0x800000, CRC(b5ddbb97) SHA1(b44dee65bbb5b2c6d7c6d4990762b26deaf3c696) )
ROM_END

ROM_START( nitdhack03 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "260_hack03.p1", 0x000000, 0x080000, CRC(4198ce09) SHA1(13c8c54b8533955e5b07ca0775346e14d4a10b35) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "260.m1", CRC(6407c5e5) SHA1(d273e154cc905b63205a17a1a6d419cac3485a92) )

	ROM_REGION( 0x0400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "260.v1", 0x000000, 0x400000, CRC(24b0480c) SHA1(d769e621be52a5cd2e2568891b5f95a48268e1e0) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "260.c1", 0x0000000, 0x800000, CRC(147b0c7f) SHA1(a647c3a2f6d146ff47521c1d39f58830601f5781) )
	ROM_LOAD16_BYTE( "260.c2", 0x0000001, 0x800000, CRC(d2b04b0d) SHA1(ce4322e6cfacb627fe997efe81018861e21d3c27) )
ROM_END

/********
 bootleg
**********/

ROM_START( nitdd )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "260.p1", 0x000000, 0x080000, CRC(61361082) SHA1(441f3f41c1aa752c0e0a9a0b1d92711d9e636b85) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "260.m1", CRC(6407c5e5) SHA1(d273e154cc905b63205a17a1a6d419cac3485a92) )

	ROM_REGION( 0x0400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "260.v1", 0x000000, 0x400000, CRC(24b0480c) SHA1(d769e621be52a5cd2e2568891b5f95a48268e1e0) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "260d.c1", 0x0000000, 0x800000, CRC(f3ff4953) SHA1(0e54548387f3056088e89086a30da05935d08b24) )
	ROM_LOAD16_BYTE( "260d.c2", 0x0000001, 0x800000, CRC(f1e49faa) SHA1(2a771e318423addd9daae3a9af200de2c70f4021) )
ROM_END

/**************
 Puzzle Bobble
****************/

ROM_START( pbobblenhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "462_hack01.p1", 0x000000, 0x038db2, CRC(6a3227ff) SHA1(45e9006edcb44c208ee1e8221f47158d6a749667) )

	NEO_SFIX_128K( "083.s1", CRC(9caae538) SHA1(cf2d90a7c1a42107c0bb8b9a61397634286dbe0a) )

	NEO_BIOS_AUDIO_64K( "462.m1", CRC(5e378316) SHA1(fa9a904583e00614762ad91bab83e86d52237777) )

	ROM_REGION( 0x180000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "083ch.v3", 0x000000, 0x100000, CRC(30ce579e) SHA1(635f7ab2178981ba38cc0080f4f97e954e552440) )
	ROM_LOAD( "083.v4", 0x100000, 0x080000, CRC(0a548948) SHA1(e1e4afd17811cb60401c14fbcf0465035165f4fb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "462_hack01.c1", 0x000000, 0x68000, CRC(b50a9e06) SHA1(d9f7aaddc6e48518a4ad332e718847001b5e3330) )
	ROM_LOAD16_BYTE( "462_hack01.c2", 0x000001, 0x68000, CRC(427422db) SHA1(e7fed9ae3ce9c61dd85a0adafc51af727acba37d) )
ROM_END

ROM_START( pbobblenhack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "461.p1", 0x000000, 0x080000, CRC(616e953c) SHA1(0f393e0c6397d6fec7f4e8d5b6864c062b824788) )

	NEO_SFIX_128K( "461.s1", CRC(5ef03f0f) SHA1(91c187e6f470830e1b54b0110080f387d74ebdf6) )

	NEO_BIOS_AUDIO_128K( "461.m1", CRC(085a9f98) SHA1(0bd575a0eb4599f1231f7656f309c916e8787c42) )

	ROM_REGION( 0x380000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "461.v1",   0x000000, 0x100000, CRC(3d59ad53) SHA1(b09b860cb385616c1649e68474d71bf3399cedc6) )
	ROM_LOAD( "461.v2",   0x100000, 0x100000, CRC(56d0c568) SHA1(d38d4eea7b50026dfc1e9839865243a83ce11d5b) )
	ROM_LOAD( "083ch.v3", 0x200000, 0x100000, CRC(30ce579e) SHA1(635f7ab2178981ba38cc0080f4f97e954e552440) )
	ROM_LOAD( "083.v4",   0x300000, 0x080000, CRC(0a548948) SHA1(e1e4afd17811cb60401c14fbcf0465035165f4fb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "461.c1", 0x000000, 0x080000, CRC(f8d39bdc) SHA1(3a36b4be73e04955adb41214fbbf9ea6a6a218d3) )
	ROM_LOAD16_BYTE( "461.c2", 0x000001, 0x080000, CRC(40b2ad28) SHA1(ce29406727710b23c7aacb3babf724d808c0e975) )
ROM_END

/********
 bootleg
**********/

ROM_START( pbobblendx )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "083dx.p1", 0x000000, 0x040000, CRC(5ed56e87) SHA1(b927c9fd907f90e1b5556c96f22b5245b23f07a6) )

	NEO_SFIX_128K( "083.s1", CRC(9caae538) SHA1(cf2d90a7c1a42107c0bb8b9a61397634286dbe0a) )

	NEO_BIOS_AUDIO_64K( "083dx.m1", CRC(129e6054) SHA1(4edd18c44759e7a140705f1544413c649131c551) )

	ROM_REGION( 0x380000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "083.v3", 0x200000, 0x100000, CRC(0840cbc4) SHA1(1adbd7aef44fa80832f63dfb8efdf69fd7256a57) )
	ROM_LOAD( "083.v4", 0x300000, 0x080000, CRC(0a548948) SHA1(e1e4afd17811cb60401c14fbcf0465035165f4fb) )

	ROM_REGION( 0x100000, "sprites", 0 )
	ROM_LOAD16_BYTE( "083.c5", 0x000000, 0x80000, CRC(e89ad494) SHA1(69c9ea415773af94ac44c48af05d55ada222b138) )
	ROM_LOAD16_BYTE( "083.c6", 0x000001, 0x80000, CRC(4b42d7eb) SHA1(042ae50a528cea21cf07771d3915c57aa16fd5af) )
ROM_END

/*************
 Pochi & Nyaa
***************/

ROM_START( pnyaad ) /* Decrypted C ROMs Version -  Encrypted Sound */
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "267.p1", 0x000000, 0x100000, CRC(112fe2c0) SHA1(01420e051f0bdbd4f68ce306a3738161b96f8ba8) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "267.m1", CRC(c7853ccd) SHA1(1b7a4c5093cf0fe3861ce44fd1d3b30c71ad0abe) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "267.v1", 0x000000, 0x400000, CRC(e2e8e917) SHA1(7f412d55aebff3d38a225a88c632916295ab0584) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "267d.c1", 0x0000000, 0x800000, CRC(75fa4f27) SHA1(05f39dc5bedfecff67a9e5bd0bb8d09d93168f58) )
	ROM_LOAD16_BYTE( "267d.c2", 0x0000001, 0x800000, CRC(02345689) SHA1(d40c425e73fd8dc0b0f66bf72a79f66b3bf390e1) )
ROM_END

ROM_START( pnyaadd ) /* Decrypted C ROMs Set 2 Version -  Encrypted Sound */
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "267.p1", 0x000000, 0x100000, CRC(112fe2c0) SHA1(01420e051f0bdbd4f68ce306a3738161b96f8ba8) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "267.m1", CRC(c7853ccd) SHA1(1b7a4c5093cf0fe3861ce44fd1d3b30c71ad0abe) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "267.v1", 0x000000, 0x400000, CRC(e2e8e917) SHA1(7f412d55aebff3d38a225a88c632916295ab0584) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "267dd.c1", 0x0000000, 0x800000, CRC(2bd02a33) SHA1(77771ace5c27679f09454d3b3c5aa4f761259263) )
	ROM_LOAD16_BYTE( "267dd.c2", 0x0000001, 0x800000, CRC(8fb27b0b) SHA1(84c9e7994f00e44841e6324219fd5f2e6ab5ca15) )
ROM_END

 /******************
 Prehistoric Isle 2
*********************/

ROM_START( preisle2hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "255_hack01.p1",   0x000000, 0x100000, CRC(69a69841) SHA1(01b3dbc2ae51993c4db84f62bbe2f3cf9fc90d57) )
	ROM_LOAD16_WORD_SWAP( "255.p2", 0x100000, 0x400000, CRC(42050b80) SHA1(0981a8295d43b264c2b95e5d7568bdda4e64c976) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "255.m1", CRC(8efd4014) SHA1(5b73809b6e4e49264d281ef3e5004ac8a9de296d) )

	ROM_REGION( 0x0600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "255.v1", 0x000000, 0x400000, CRC(5a14543d) SHA1(7146ac748f846c7e2d5b0bdcf953892e39b648fe) )
	ROM_LOAD( "255.v2", 0x400000, 0x200000, CRC(6610d91a) SHA1(b2c6786920dc1712e88c3cc26d2c6c3ac2615bf4) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "255d.c1", 0x0000000, 0x800000, CRC(50fd785e) SHA1(97e03925f3060df4cfa576b6d2c2000952990530) )
	ROM_LOAD16_BYTE( "255d.c2", 0x0000001, 0x800000, CRC(ab913f1e) SHA1(e3413d40dc4a2240ff0302eb9eff4cc64895b6ef) )
	ROM_LOAD16_BYTE( "255d.c3", 0x1000000, 0x800000, CRC(bc0ee75c) SHA1(50c01b97d5457c2c1b8df8c0c7eda69384dea78b) )
	ROM_LOAD16_BYTE( "255d.c4", 0x1000001, 0x800000, CRC(29908823) SHA1(2a40263f3866c06eecc209faeea7ced6641ed918) )
	ROM_LOAD16_BYTE( "255d.c5", 0x2000000, 0x800000, CRC(83c56bca) SHA1(54bec4cce628bef3b87bbd6c0339494232a9516b) )
	ROM_LOAD16_BYTE( "255d.c6", 0x2000001, 0x800000, CRC(59e0e805) SHA1(0d9c9b52bb0672fe70ffbb2780a9449f6a7f0984) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( preisle2d )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "255.p1",  0x000000, 0x100000, CRC(dfa3c0f3) SHA1(793c6a46f3a794536dc0327a3f3fad20e25ab661) )
	ROM_LOAD16_WORD_SWAP( "255.p2", 0x100000, 0x400000, CRC(42050b80) SHA1(0981a8295d43b264c2b95e5d7568bdda4e64c976) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "255.m1", CRC(8efd4014) SHA1(5b73809b6e4e49264d281ef3e5004ac8a9de296d) )

	ROM_REGION( 0x0600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "255.v1", 0x000000, 0x400000, CRC(5a14543d) SHA1(7146ac748f846c7e2d5b0bdcf953892e39b648fe) )
	ROM_LOAD( "255.v2", 0x400000, 0x200000, CRC(6610d91a) SHA1(b2c6786920dc1712e88c3cc26d2c6c3ac2615bf4) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "255d.c1", 0x0000000, 0x800000, CRC(50fd785e) SHA1(97e03925f3060df4cfa576b6d2c2000952990530) )
	ROM_LOAD16_BYTE( "255d.c2", 0x0000001, 0x800000, CRC(ab913f1e) SHA1(e3413d40dc4a2240ff0302eb9eff4cc64895b6ef) )
	ROM_LOAD16_BYTE( "255d.c3", 0x1000000, 0x800000, CRC(bc0ee75c) SHA1(50c01b97d5457c2c1b8df8c0c7eda69384dea78b) )
	ROM_LOAD16_BYTE( "255d.c4", 0x1000001, 0x800000, CRC(29908823) SHA1(2a40263f3866c06eecc209faeea7ced6641ed918) )
	ROM_LOAD16_BYTE( "255d.c5", 0x2000000, 0x800000, CRC(83c56bca) SHA1(54bec4cce628bef3b87bbd6c0339494232a9516b) )
	ROM_LOAD16_BYTE( "255d.c6", 0x2000001, 0x800000, CRC(59e0e805) SHA1(0d9c9b52bb0672fe70ffbb2780a9449f6a7f0984) )
ROM_END

 /*********
 Ragnagard
************/

ROM_START( ragnagrdhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "218_hack01.p1", 0x100000, 0x100000, CRC(befbb496) SHA1(7970d7fd151098be4625a96d8b21f66fd3d0aecf) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "218.s1", CRC(7d402f9a) SHA1(59ec29d03e62e7a8bef689a124a9164f43b2ace1) )

	NEO_BIOS_AUDIO_128K( "218.m1", CRC(17028bcf) SHA1(7a4e8f33ce9b41beac2152b8f6003f247699e2e1) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "218.v1", 0x000000, 0x400000, CRC(61eee7f4) SHA1(5b11b1a0b1b74dfbc2998cbda9f8f7a5e9059957) )
	ROM_LOAD( "218.v2", 0x400000, 0x400000, CRC(6104e20b) SHA1(18e8aae3e51e141977d523a10e737ff68fe81910) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "218.c1", 0x0000000, 0x400000, CRC(c31500a4) SHA1(cc82100038988872721028044ed2e9764bcc2fb0) )
	ROM_LOAD16_BYTE( "218.c2", 0x0000001, 0x400000, CRC(98aba1f9) SHA1(121276c569967e501d8e1b83747f1bdebff612ea) )
	ROM_LOAD16_BYTE( "218.c3", 0x0800000, 0x400000, CRC(833c163a) SHA1(b7e5356bbd9efab67fedb5bc671ba8bbd661fe0f) )
	ROM_LOAD16_BYTE( "218.c4", 0x0800001, 0x400000, CRC(c1a30f69) SHA1(f87ddda4695abcd14f5c2d4b7d41f72ad5b064cc) )
	ROM_LOAD16_BYTE( "218.c5", 0x1000000, 0x400000, CRC(6b6de0ff) SHA1(1abb24cb407258235f4a572cf101d0774823040b) )
	ROM_LOAD16_BYTE( "218.c6", 0x1000001, 0x400000, CRC(94beefcf) SHA1(d2ff0bac325c9c823dba68bd4f281b3b9f8f68e7) )
	ROM_LOAD16_BYTE( "218.c7", 0x1800000, 0x400000, CRC(de6f9b28) SHA1(455adb6bb986af8a00d7f32b7f4f3715fc3007f6) )
	ROM_LOAD16_BYTE( "218.c8", 0x1800001, 0x400000, CRC(d9b311f6) SHA1(ba61a7ab3f08bb7348ad6cd01e5d29ca5ee75074) )
ROM_END

ROM_START( ragnagrdhack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "218_hack02.p1", 0x100000, 0x100000, CRC(370a3b64) SHA1(85ddeb5ec0801c66268824236ed9ce2df51e927b) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "218.s1", CRC(7d402f9a) SHA1(59ec29d03e62e7a8bef689a124a9164f43b2ace1) )

	NEO_BIOS_AUDIO_128K( "218.m1", CRC(17028bcf) SHA1(7a4e8f33ce9b41beac2152b8f6003f247699e2e1) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "218.v1", 0x000000, 0x400000, CRC(61eee7f4) SHA1(5b11b1a0b1b74dfbc2998cbda9f8f7a5e9059957) )
	ROM_LOAD( "218.v2", 0x400000, 0x400000, CRC(6104e20b) SHA1(18e8aae3e51e141977d523a10e737ff68fe81910) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "218.c1", 0x0000000, 0x400000, CRC(c31500a4) SHA1(cc82100038988872721028044ed2e9764bcc2fb0) )
	ROM_LOAD16_BYTE( "218.c2", 0x0000001, 0x400000, CRC(98aba1f9) SHA1(121276c569967e501d8e1b83747f1bdebff612ea) )
	ROM_LOAD16_BYTE( "218.c3", 0x0800000, 0x400000, CRC(833c163a) SHA1(b7e5356bbd9efab67fedb5bc671ba8bbd661fe0f) )
	ROM_LOAD16_BYTE( "218.c4", 0x0800001, 0x400000, CRC(c1a30f69) SHA1(f87ddda4695abcd14f5c2d4b7d41f72ad5b064cc) )
	ROM_LOAD16_BYTE( "218.c5", 0x1000000, 0x400000, CRC(6b6de0ff) SHA1(1abb24cb407258235f4a572cf101d0774823040b) )
	ROM_LOAD16_BYTE( "218.c6", 0x1000001, 0x400000, CRC(94beefcf) SHA1(d2ff0bac325c9c823dba68bd4f281b3b9f8f68e7) )
	ROM_LOAD16_BYTE( "218.c7", 0x1800000, 0x400000, CRC(de6f9b28) SHA1(455adb6bb986af8a00d7f32b7f4f3715fc3007f6) )
	ROM_LOAD16_BYTE( "218.c8", 0x1800001, 0x400000, CRC(d9b311f6) SHA1(ba61a7ab3f08bb7348ad6cd01e5d29ca5ee75074) )
ROM_END

ROM_START( ragnagrdhack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "218_hack03.p1", 0x100000, 0x100000, CRC(64000f81) SHA1(483ff1c968e62318b89b6ed05a9b39160796ee48) )
	ROM_CONTINUE( 0x000000, 0x100000 )
	ROM_IGNORE( 0x254F76 )

	NEO_SFIX_128K( "218.s1", CRC(7d402f9a) SHA1(59ec29d03e62e7a8bef689a124a9164f43b2ace1) )

	NEO_BIOS_AUDIO_128K( "218.m1", CRC(17028bcf) SHA1(7a4e8f33ce9b41beac2152b8f6003f247699e2e1) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "218.v1", 0x000000, 0x400000, CRC(61eee7f4) SHA1(5b11b1a0b1b74dfbc2998cbda9f8f7a5e9059957) )
	ROM_LOAD( "218.v2", 0x400000, 0x400000, CRC(6104e20b) SHA1(18e8aae3e51e141977d523a10e737ff68fe81910) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "218.c1", 0x0000000, 0x400000, CRC(c31500a4) SHA1(cc82100038988872721028044ed2e9764bcc2fb0) )
	ROM_LOAD16_BYTE( "218.c2", 0x0000001, 0x400000, CRC(98aba1f9) SHA1(121276c569967e501d8e1b83747f1bdebff612ea) )
	ROM_LOAD16_BYTE( "218.c3", 0x0800000, 0x400000, CRC(833c163a) SHA1(b7e5356bbd9efab67fedb5bc671ba8bbd661fe0f) )
	ROM_LOAD16_BYTE( "218.c4", 0x0800001, 0x400000, CRC(c1a30f69) SHA1(f87ddda4695abcd14f5c2d4b7d41f72ad5b064cc) )
	ROM_LOAD16_BYTE( "218.c5", 0x1000000, 0x400000, CRC(6b6de0ff) SHA1(1abb24cb407258235f4a572cf101d0774823040b) )
	ROM_LOAD16_BYTE( "218.c6", 0x1000001, 0x400000, CRC(94beefcf) SHA1(d2ff0bac325c9c823dba68bd4f281b3b9f8f68e7) )
	ROM_LOAD16_BYTE( "218.c7", 0x1800000, 0x400000, CRC(de6f9b28) SHA1(455adb6bb986af8a00d7f32b7f4f3715fc3007f6) )
	ROM_LOAD16_BYTE( "218.c8", 0x1800001, 0x400000, CRC(d9b311f6) SHA1(ba61a7ab3f08bb7348ad6cd01e5d29ca5ee75074) )
ROM_END

 /*************************************
   Real Bout Fatal Fury Special
***************************************/

ROM_START( rbffspechack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "223_hack01.p1",    0x000000, 0x100000, CRC(e8eadfa5) SHA1(3314ee9adb6e9384a49b09a15d6fffbb15c39872) )
	ROM_LOAD16_WORD_SWAP( "223.p2", 0x100000, 0x400000, CRC(addd8f08) SHA1(abaf5b86c8ec915c07ef2d83fce9ad03acaa4817) )

	NEO_SFIX_128K( "223.s1", CRC(7ecd6e8c) SHA1(465455afc4d83cbb118142be4671b2539ffafd79) )

	NEO_BIOS_AUDIO_128K( "223.m1", CRC(3fee46bf) SHA1(e750f85233953853618fcdff980a4721af1710a3) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "223.v1", 0x000000, 0x400000, CRC(76673869) SHA1(78a26afa29f73de552ffabdbf3fc4bf26be8ae9e) )
	ROM_LOAD( "223.v2", 0x400000, 0x400000, CRC(7a275acd) SHA1(8afe87ce822614262b72a90b371fc79155ac0d0c) )
	ROM_LOAD( "223.v3", 0x800000, 0x400000, CRC(5a797fd2) SHA1(94958e334f86d4d71059af8138f255b8d97a3b01) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "223.c1", 0x0000000, 0x400000, CRC(ebab05e2) SHA1(0d60a8b631e3a3dcfbfdd7779dee081c9548ec39) )
	ROM_LOAD16_BYTE( "223.c2", 0x0000001, 0x400000, CRC(641868c3) SHA1(aa1aeb661842276b3326bfa4f1456f75bfecd52e) )
	ROM_LOAD16_BYTE( "223.c3", 0x0800000, 0x400000, CRC(ca00191f) SHA1(96977febfcc513e1848d7029ff169cdf51104038) )
	ROM_LOAD16_BYTE( "223.c4", 0x0800001, 0x400000, CRC(1f23d860) SHA1(e18df52f898a51074e07a0b8c6e75873e7cde35e) )
	ROM_LOAD16_BYTE( "223.c5", 0x1000000, 0x400000, CRC(321e362c) SHA1(39bd189334278f266124c97c6f70995f6f171cea) )
	ROM_LOAD16_BYTE( "223.c6", 0x1000001, 0x400000, CRC(d8fcef90) SHA1(bbccacb27f1e587bc144fe7ce68bd7b327ceaaee) )
	ROM_LOAD16_BYTE( "223.c7", 0x1800000, 0x400000, CRC(bc80dd2d) SHA1(086f372015eede88c6c578595fe915e28a589d2f) )
	ROM_LOAD16_BYTE( "223.c8", 0x1800001, 0x400000, CRC(5ad62102) SHA1(e28cc9840caed2a1a8bd65a03bef05231071040c) )
ROM_END

ROM_START( rbffspechack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "223_hack02.p1",  0x000000, 0x100000, CRC(f26ba629) SHA1(e2e436242a4dd6b09e41fad20eb4861b02c0e821) )
	ROM_LOAD16_WORD_SWAP( "223.p2", 0x100000, 0x400000, CRC(addd8f08) SHA1(abaf5b86c8ec915c07ef2d83fce9ad03acaa4817) )

	NEO_SFIX_128K( "223.s1", CRC(7ecd6e8c) SHA1(465455afc4d83cbb118142be4671b2539ffafd79) )

	NEO_BIOS_AUDIO_128K( "223.m1", CRC(3fee46bf) SHA1(e750f85233953853618fcdff980a4721af1710a3) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "223.v1", 0x000000, 0x400000, CRC(76673869) SHA1(78a26afa29f73de552ffabdbf3fc4bf26be8ae9e) )
	ROM_LOAD( "223.v2", 0x400000, 0x400000, CRC(7a275acd) SHA1(8afe87ce822614262b72a90b371fc79155ac0d0c) )
	ROM_LOAD( "223.v3", 0x800000, 0x400000, CRC(5a797fd2) SHA1(94958e334f86d4d71059af8138f255b8d97a3b01) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "223.c1", 0x0000000, 0x400000, CRC(ebab05e2) SHA1(0d60a8b631e3a3dcfbfdd7779dee081c9548ec39) )
	ROM_LOAD16_BYTE( "223.c2", 0x0000001, 0x400000, CRC(641868c3) SHA1(aa1aeb661842276b3326bfa4f1456f75bfecd52e) )
	ROM_LOAD16_BYTE( "223.c3", 0x0800000, 0x400000, CRC(ca00191f) SHA1(96977febfcc513e1848d7029ff169cdf51104038) )
	ROM_LOAD16_BYTE( "223.c4", 0x0800001, 0x400000, CRC(1f23d860) SHA1(e18df52f898a51074e07a0b8c6e75873e7cde35e) )
	ROM_LOAD16_BYTE( "223.c5", 0x1000000, 0x400000, CRC(321e362c) SHA1(39bd189334278f266124c97c6f70995f6f171cea) )
	ROM_LOAD16_BYTE( "223.c6", 0x1000001, 0x400000, CRC(d8fcef90) SHA1(bbccacb27f1e587bc144fe7ce68bd7b327ceaaee) )
	ROM_LOAD16_BYTE( "223.c7", 0x1800000, 0x400000, CRC(bc80dd2d) SHA1(086f372015eede88c6c578595fe915e28a589d2f) )
	ROM_LOAD16_BYTE( "223.c8", 0x1800001, 0x400000, CRC(5ad62102) SHA1(e28cc9840caed2a1a8bd65a03bef05231071040c) )
ROM_END

 /**********************
 Real Bout Fatal Fury 
*************************/

ROM_START( rbff1hack01 ) 
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "095_hack01.p1",   0x000000, 0x100000, CRC(fa5e3c0f) SHA1(6391ad22eabf0246aa3b9f6d9f821cd58d2e5c71) )
	ROM_LOAD16_WORD_SWAP( "095.p2", 0x100000, 0x200000, CRC(cc15826e) SHA1(44d6ac6c0ca697a6f367dcfd809b1e1771cb0635) )

	NEO_SFIX_128K( "095.s1", CRC(b6bf5e08) SHA1(b527355c35ea097f3448676f2ffa65b8e56ae30c) )

	NEO_BIOS_AUDIO_128K( "095.m1", CRC(653492a7) SHA1(39e511fb9ed5d2135dc8428a31d0baafb2ab36e0) )

	ROM_REGION( 0xc00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "069.v1", 0x000000, 0x400000, CRC(2bdbd4db) SHA1(5f4fecf69c2329d699cbd45829c19303b1e2a80e) )
	ROM_LOAD( "069.v2", 0x400000, 0x400000, CRC(a698a487) SHA1(11b8bc53bc26a51f4a408e900e3769958625c4ed) )
	ROM_LOAD( "095.v3", 0x800000, 0x400000, CRC(189d1c6c) SHA1(f0b8cd1ee40ea3feeb2800f0723b451ec8240203) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "069.c1", 0x0000000, 0x400000, CRC(e302f93c) SHA1(d8610b14900b2b8fe691b67ca9b1abb335dbff74) )
	ROM_LOAD16_BYTE( "069.c2", 0x0000001, 0x400000, CRC(1053a455) SHA1(69501bfac68739e63d798045b812badd251d57b8) )
	ROM_LOAD16_BYTE( "069.c3", 0x0800000, 0x400000, CRC(1c0fde2f) SHA1(cf6c2ef56c03a861de3b0b6dc0d7c9204d947f9d) )
	ROM_LOAD16_BYTE( "069.c4", 0x0800001, 0x400000, CRC(a25fc3d0) SHA1(83cb349e2f1032652060b233e741fb893be5af16) )
	ROM_LOAD16_BYTE( "095.c5", 0x1000000, 0x400000, CRC(8b9b65df) SHA1(e2a7e20855501f240bcd22f5cc92fcb4a9806abe) )
	ROM_LOAD16_BYTE( "095.c6", 0x1000001, 0x400000, CRC(3e164718) SHA1(53217f938c8964c1ca68a6fd5249c4169a5ac8e6) )
	ROM_LOAD16_BYTE( "095.c7", 0x1800000, 0x200000, CRC(ca605e12) SHA1(5150b835247fd705bc1dece97d423d9c20a51416) )
	ROM_LOAD16_BYTE( "095.c8", 0x1800001, 0x200000, CRC(4e6beb6c) SHA1(c0ac7cfc832ace6ad52c58f5da3a8101baead749) )
ROM_END

 /**********************
 Real Bout Fatal Fury 2
*************************/

ROM_START( rbff2hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "240_hack01.p1", 0x000000, 0x100000, CRC(D01854FA) SHA1(1a4230404df76c111c76348a04d86976cd68df93) )
	ROM_LOAD16_WORD_SWAP( "240_hack01.p2", 0x100000, 0x400000, CRC(C063193D) SHA1(3321d90f93b1f7bf423a20b780b453a756e041be) )

	NEO_SFIX_128K( "240bh.s1",  CRC(141a8492) SHA1(e3b27771c78ac1df96f9daf951511897652b1a0f) )

	NEO_BIOS_AUDIO_256K( "240.m1", CRC(ed482791) SHA1(1f54a45967cb7842c33aa24be322c9f33ff75ac3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "240.v1", 0x000000, 0x400000, CRC(f796265a) SHA1(736dff37eb91fc856b4d189249fb0de9b6c0813a) )
	ROM_LOAD( "240.v2", 0x400000, 0x400000, CRC(2cb3f3bb) SHA1(697e677890892f4b028c9a27c66809ca0a8a9b18) )
	ROM_LOAD( "240.v3", 0x800000, 0x400000, CRC(8fe1367a) SHA1(093d7a4ac2b54ad7ffb2dc316fe29415f7a99535) )
	ROM_LOAD( "240b.v4",   0xc00000, 0x400000, CRC(33a356ee) SHA1(b2a08052be670f301f8d4903b36c70088d1a11da) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "240.c1", 0x0000000, 0x800000, CRC(effac504) SHA1(e36a96e7369b02c7e839b5abf3c6799453ba1927) )
	ROM_LOAD16_BYTE( "240.c2", 0x0000001, 0x800000, CRC(ed182d44) SHA1(a9fc0a3a786bf067c129ec7220df65953dff804f) )
	ROM_LOAD16_BYTE( "240.c3", 0x1000000, 0x800000, CRC(22e0330a) SHA1(0fe7f6a8aeba7f17dbb278e85003969ff10d3cd2) )
	ROM_LOAD16_BYTE( "240.c4", 0x1000001, 0x800000, CRC(c19a07eb) SHA1(139eac8b51cadf328dd42d8109f4e2463f57230c) )
	ROM_LOAD16_BYTE( "240_hack01.c5",  0x2000000, 0x800000, CRC(21831787) SHA1(928620e9efb849c591d75a645969c24b15e5c337) )
	ROM_LOAD16_BYTE( "240_hack01.c6",  0x2000001, 0x800000, CRC(06B2D1DA) SHA1(d39da350a9017a68704ea99bff7f48163b0866fb) )
ROM_END

ROM_START( rbff2hack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "240_hack02.p1", 0x000000, 0x100000, CRC(76505c42) SHA1(83fe80d5fbfa7d03d425804a917d76fcf96631e8) )
	ROM_LOAD16_WORD_SWAP( "240.p2", 0x100000, 0x400000, CRC(960aa88d) SHA1(3d9e785891871af90313f178dca2724633406674) )

	NEO_SFIX_128K( "240.s1", CRC(da3b40de) SHA1(e6bf74e057ac6fe1f249a7547f13ba7fbc694561) )

	NEO_BIOS_AUDIO_256K( "240.m1", CRC(ed482791) SHA1(1f54a45967cb7842c33aa24be322c9f33ff75ac3) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "240.v1", 0x000000, 0x400000, CRC(f796265a) SHA1(736dff37eb91fc856b4d189249fb0de9b6c0813a) )
	ROM_LOAD( "240.v2", 0x400000, 0x400000, CRC(2cb3f3bb) SHA1(697e677890892f4b028c9a27c66809ca0a8a9b18) )
	ROM_LOAD( "240.v3", 0x800000, 0x400000, CRC(8fe1367a) SHA1(093d7a4ac2b54ad7ffb2dc316fe29415f7a99535) )
	ROM_LOAD( "240b.v4",   0xc00000, 0x400000, CRC(33a356ee) SHA1(b2a08052be670f301f8d4903b36c70088d1a11da) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "240.c1", 0x0000000, 0x800000, CRC(effac504) SHA1(e36a96e7369b02c7e839b5abf3c6799453ba1927) )
	ROM_LOAD16_BYTE( "240.c2", 0x0000001, 0x800000, CRC(ed182d44) SHA1(a9fc0a3a786bf067c129ec7220df65953dff804f) )
	ROM_LOAD16_BYTE( "240.c3", 0x1000000, 0x800000, CRC(22e0330a) SHA1(0fe7f6a8aeba7f17dbb278e85003969ff10d3cd2) )
	ROM_LOAD16_BYTE( "240.c4", 0x1000001, 0x800000, CRC(c19a07eb) SHA1(139eac8b51cadf328dd42d8109f4e2463f57230c) )
	ROM_LOAD16_BYTE( "240.c5", 0x2000000, 0x800000, CRC(244dff5a) SHA1(156548156d3ceaa808d0053d0749af2526a3943e) )
	ROM_LOAD16_BYTE( "240.c6", 0x2000001, 0x800000, CRC(4609e507) SHA1(bb17f50a377dddb77c1eeda5944a7bcbf0cca5f7) )
ROM_END

 /*******************
 Rage of the Dragons
**********************/

ROM_START( rotdhack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "264_hack01.p1", 0x000000, 0x800000, CRC(44889313) SHA1(45b7cbdb9cdc57f0218d372558aaae5fb87766f6) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_128K( "264.m1", CRC(4dbd7b43) SHA1(6b63756b0d2d30bbf13fbd219833c81fd060ef96) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "264.v1", 0x000000, 0x800000, CRC(fa005812) SHA1(73723126dab5a640ac11955ed6da1bf7a91394f5) )
	ROM_LOAD( "264.v2", 0x800000, 0x800000, CRC(c3dc8bf0) SHA1(a105e37262d9500a30fb8a5dac05aa4fab2562a3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "264_hack01.c1", 0x0000000, 0x800000, CRC(a9df0cce) SHA1(3bf0a30aa314a5ad390b3c7413c7828c8f71d555) )
	ROM_LOAD16_BYTE( "264_hack01.c2", 0x0000001, 0x800000, CRC(8b390a0b) SHA1(7eb6c839a44f22d23dffddd50324d8d87903aacc) )
	ROM_LOAD16_BYTE( "264_hack01.c3", 0x1000000, 0x800000, CRC(b7b87b63) SHA1(46851133cab4ddf1545c2a22859e7f364a80c757) )
	ROM_LOAD16_BYTE( "264_hack01.c4", 0x1000001, 0x800000, CRC(b0c8ef2e) SHA1(c5abd4207335da5acddaf718e169f17a4bc9cbc5) )
	ROM_LOAD16_BYTE( "264_hack01.c5", 0x2000000, 0x800000, CRC(738a9edd) SHA1(4a46254ac440557a626fe2668cae679a2f64994a) )
	ROM_LOAD16_BYTE( "264_hack01.c6", 0x2000001, 0x800000, CRC(fddcd101) SHA1(d6a4d5088e8ba204066e840a6ea658e77100c07f) )
	ROM_LOAD16_BYTE( "264_hack01.c7", 0x3000000, 0x800000, CRC(e32cc8d5) SHA1(9749ed4343762f8342bbc92e97fadc6704413be6) )
	ROM_LOAD16_BYTE( "264_hack01.c8", 0x3000001, 0x800000, CRC(03f124d2) SHA1(cf92a5f7aeca74946b52e7f4e96606eec62548eb) )
ROM_END

ROM_START( rotdhack02 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "264_hack02.p1", 0x000000, 0x800000, CRC(8f5d7188) SHA1(a29209a05a22e18d6baa78830d6bbc763d23dcac) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_64K( "264d.m1", CRC(7e06206d) SHA1(ba32dd6ffbfbf532bba9bed4c2d70cfbe959ac23) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "264.v1", 0x000000, 0x800000, CRC(fa005812) SHA1(73723126dab5a640ac11955ed6da1bf7a91394f5) )
	ROM_LOAD( "264.v2", 0x800000, 0x800000, CRC(c3dc8bf0) SHA1(a105e37262d9500a30fb8a5dac05aa4fab2562a3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "264_hack02.c1", 0x0000000, 0x800000, CRC(52126204) SHA1(78af3f15233b4798978a5214382a8d91fa70f69c) )
	ROM_LOAD16_BYTE( "264_hack02.c2", 0x0000001, 0x800000, CRC(58f79e13) SHA1(d1f01d46df9e79c7df2db6bf05740f2970a01ed4) )
	ROM_LOAD16_BYTE( "264_hack02.c3", 0x1000000, 0x800000, CRC(30f8e775) SHA1(49671cbd27616130e2f11b7678052fc8e090937a) )
	ROM_LOAD16_BYTE( "264_hack02.c4", 0x1000001, 0x800000, CRC(4c618f32) SHA1(b7ca14f908b71a381ce88024a9562b021c39d393) )
	ROM_LOAD16_BYTE( "264_hack02.c5", 0x2000000, 0x800000, CRC(d5c59c25) SHA1(f7b409d93fdf4d1b2a87a49722853d77dc8fc67b) )
	ROM_LOAD16_BYTE( "264_hack02.c6", 0x2000001, 0x800000, CRC(e3babe6a) SHA1(e61efa02a28f81fcfc4d76edc0ab4118126f3008) )
	ROM_LOAD16_BYTE( "264_hack02.c7", 0x3000000, 0x800000, CRC(84bb00b6) SHA1(cdf3bf1843110057556683658827da818128e82b) )
	ROM_LOAD16_BYTE( "264_hack02.c8", 0x3000001, 0x800000, CRC(eff2cd12) SHA1(cab417f8974780786b98637effad8a1a8c2e8562) )
ROM_END

ROM_START( rotdhack03 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "264_hack03.p1", 0x000000, 0x800000, CRC(fe93cb79) SHA1(2e3faf72f2ff5248c1632d50fe2000d6ae3dc01e) )

	NEO_SFIX_MT_128K
	NEO_BIOS_AUDIO_ENCRYPTED_128K( "264.m1", CRC(4dbd7b43) SHA1(6b63756b0d2d30bbf13fbd219833c81fd060ef96) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "264.v1", 0x000000, 0x800000, CRC(fa005812) SHA1(73723126dab5a640ac11955ed6da1bf7a91394f5) )
	ROM_LOAD( "264.v2", 0x800000, 0x800000, CRC(c3dc8bf0) SHA1(a105e37262d9500a30fb8a5dac05aa4fab2562a3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "264.c1", 0x0000000, 0x800000, CRC(4f148fee) SHA1(0821463765fad8fbd0dfbbabb7807337d0333719) )
	ROM_LOAD16_BYTE( "264.c2", 0x0000001, 0x800000, CRC(7cf5ff72) SHA1(ccb2f94bce943576d224cb326806942426d25584) )
	ROM_LOAD16_BYTE( "264.c3", 0x1000000, 0x800000, CRC(64d84c98) SHA1(8faf153f465ce6fb7770b27a7ce63caf11dd4086) )
	ROM_LOAD16_BYTE( "264.c4", 0x1000001, 0x800000, CRC(2f394a95) SHA1(82347e8f2b48b0522d7d91fd3f372d5768934ab2) )
	ROM_LOAD16_BYTE( "264.c5", 0x2000000, 0x800000, CRC(6b99b978) SHA1(8fd0a60029b41668f9e1e3056edd3c90f62efa83) )
	ROM_LOAD16_BYTE( "264.c6", 0x2000001, 0x800000, CRC(847d5c7d) SHA1(a2ce03f6302edf81f2645de9ec61df1a281ddd78) )
	ROM_LOAD16_BYTE( "264.c7", 0x3000000, 0x800000, CRC(231d681e) SHA1(87836e64dc816f8bf1c834641535ea96baacc024) )
	ROM_LOAD16_BYTE( "264.c8", 0x3000001, 0x800000, CRC(c5edb5c4) SHA1(253378c8739daa5da4edb15eff7050820b2b3755) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( rotdd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "264.p1", 0x000000, 0x800000, CRC(b8cc969d) SHA1(4f2205b4bdd32dd1522106ef4df10ac0eb1b852d) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_64K( "264d.m1", CRC(7e06206d) SHA1(ba32dd6ffbfbf532bba9bed4c2d70cfbe959ac23) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "264.v1", 0x000000, 0x800000, CRC(fa005812) SHA1(73723126dab5a640ac11955ed6da1bf7a91394f5) )
	ROM_LOAD( "264.v2", 0x800000, 0x800000, CRC(c3dc8bf0) SHA1(a105e37262d9500a30fb8a5dac05aa4fab2562a3) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "264d.c1", 0x0000000, 0x800000, CRC(ec9d18c0) SHA1(83EBEDAE9655B6C64421CD131930B27D252853EC) )
	ROM_LOAD16_BYTE( "264d.c2", 0x0000001, 0x800000, CRC(b1069066) SHA1(0b4E2249EFC782B572B589F72AADBD8007E8778C) )
	ROM_LOAD16_BYTE( "264d.c3", 0x1000000, 0x800000, CRC(7e636d49) SHA1(1BA15DDBCE8369D7232CC1DA3F833F6CB68FBC10) )
	ROM_LOAD16_BYTE( "264d.c4", 0x1000001, 0x800000, CRC(76892fda) SHA1(27416585973A6EC7B5721DE1116C8700B38E3A4C) )
	ROM_LOAD16_BYTE( "264d.c5", 0x2000000, 0x800000, CRC(469061bc) SHA1(34E60E5F93743BE88BFC9E8B7656FE9FAF77F8F5) )
	ROM_LOAD16_BYTE( "264d.c6", 0x2000001, 0x800000, CRC(2200220a) SHA1(8379B4C89854B3CC9B4F33F4F51AB5387ACD14CA) )
	ROM_LOAD16_BYTE( "264d.c7", 0x3000000, 0x800000, CRC(edda4baf) SHA1(3F53EB50FD9EC9E9EE05EEE050885C07023B6D53) )
	ROM_LOAD16_BYTE( "264d.c8", 0x3000001, 0x800000, CRC(82b1ba22) SHA1(B4AD715807F2C15FC06945F0BCB475C3698CF089) )
ROM_END

ROM_START( rotdnd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "264nd1.p1", 0x000000, 0x800000, CRC(23809749) SHA1(c0b63032e08337b9a9ab0e3ca1f91a8be2641e74) )

	NEO_SFIX_128K( "264nd.s1", CRC(c155d4ff) SHA1(cc20d4e30004ca82da2ba075c084d294c94651d0) )

	NEO_BIOS_AUDIO_128K( "264nd.m1", CRC(c5d36af9) SHA1(320739d0cfa6cc84455ae1f7b1f9cdab3c018933) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "264nd1.v1", 0x000000, 0x800000, CRC(2c49f3fa) SHA1(501e8b792bc96a262058326541333738418fbbdd) )
	ROM_LOAD( "264nd1.v2", 0x800000, 0x800000, CRC(967279da) SHA1(21e90e14f6a6d73dd049541120c36d4421e237e6) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "264d.c1", 0x0000000, 0x800000, CRC(ec9d18c0) SHA1(83EBEDAE9655B6C64421CD131930B27D252853EC) )
	ROM_LOAD16_BYTE( "264d.c2", 0x0000001, 0x800000, CRC(b1069066) SHA1(0b4E2249EFC782B572B589F72AADBD8007E8778C) )
	ROM_LOAD16_BYTE( "264d.c3", 0x1000000, 0x800000, CRC(7e636d49) SHA1(1BA15DDBCE8369D7232CC1DA3F833F6CB68FBC10) )
	ROM_LOAD16_BYTE( "264d.c4", 0x1000001, 0x800000, CRC(76892fda) SHA1(27416585973A6EC7B5721DE1116C8700B38E3A4C) )
	ROM_LOAD16_BYTE( "264d.c5", 0x2000000, 0x800000, CRC(469061bc) SHA1(34E60E5F93743BE88BFC9E8B7656FE9FAF77F8F5) )
	ROM_LOAD16_BYTE( "264d.c6", 0x2000001, 0x800000, CRC(2200220a) SHA1(8379B4C89854B3CC9B4F33F4F51AB5387ACD14CA) )
	ROM_LOAD16_BYTE( "264d.c7", 0x3000000, 0x800000, CRC(edda4baf) SHA1(3F53EB50FD9EC9E9EE05EEE050885C07023B6D53) )
	ROM_LOAD16_BYTE( "264d.c8", 0x3000001, 0x800000, CRC(82b1ba22) SHA1(B4AD715807F2C15FC06945F0BCB475C3698CF089) )
ROM_END

 /********************
   Strikers 1945 Plus
***********************/

ROM_START( s1945phack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "254_hack01.p1",    0x000000, 0x100000, CRC(2e106855) SHA1(53f2dd9dea00f36eaac6b56dabbb860aff4f3712) )
	ROM_LOAD16_WORD_SWAP( "254.p2", 0x100000, 0x400000, CRC(efdfd4dd) SHA1(254f3e1b546eed788f7ae919be9d1bf9702148ce) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "254.m1", CRC(994b4487) SHA1(a4e645a3ababa48a8325980ff022e04a8b51b017) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "254.v1", 0x000000, 0x400000, CRC(844f58fb) SHA1(e59544457be9f21481eac8b5a39b9cbb502f252d) )
	ROM_LOAD( "254.v2", 0x400000, 0x400000, CRC(d9a248f0) SHA1(dd3e0974b753e6f94d0943a002de45668a1b072b) )
	ROM_LOAD( "254.v3", 0x800000, 0x400000, CRC(0b0d2d33) SHA1(f8e76af42a997f36a40f66b39de00f68afe6a89c) )
	ROM_LOAD( "254.v4", 0xc00000, 0x400000, CRC(6d13dc91) SHA1(8433513c0b5aea61939068a25ab90efbe3e44116) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "254.c1", 0x0000000, 0x800000, CRC(ae6fc8ef) SHA1(544ccdaee8a4a45cdce9483e30852811d2d5f3cc) )
	ROM_LOAD16_BYTE( "254.c2", 0x0000001, 0x800000, CRC(436fa176) SHA1(d70141a91a360a1b1070753086f976608fec38af) )
	ROM_LOAD16_BYTE( "254.c3", 0x1000000, 0x800000, CRC(e53ff2dc) SHA1(31f6aaffe28146d574aa72f14f90a9d968f36bc6) )
	ROM_LOAD16_BYTE( "254.c4", 0x1000001, 0x800000, CRC(818672f0) SHA1(460c6738d0ee5ae440a23fc1434fab53bbb242b5) )
	ROM_LOAD16_BYTE( "254.c5", 0x2000000, 0x800000, CRC(4580eacd) SHA1(feb96eb5e80c9125ddd7836e0939212cd3011c34) )
	ROM_LOAD16_BYTE( "254.c6", 0x2000001, 0x800000, CRC(e34970fc) SHA1(6e43e15e27bc914357f977116ab1e2d98711bb21) )
	ROM_LOAD16_BYTE( "254.c7", 0x3000000, 0x800000, CRC(f2323239) SHA1(5b3e8dd77474203be010ec7363858d806344a320) )
	ROM_LOAD16_BYTE( "254.c8", 0x3000001, 0x800000, CRC(66848c7d) SHA1(24d4ed627940a4cf8129761c1da15556e52e199c) )
ROM_END

ROM_START( s1945phack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "254_hack02.p1",   0x000000, 0x100000, CRC(524eaf9c) SHA1(03e6e4495eca181c5d702df312565abd6684d6a4) )
	ROM_LOAD16_WORD_SWAP( "254.p2", 0x100000, 0x400000, CRC(efdfd4dd) SHA1(254f3e1b546eed788f7ae919be9d1bf9702148ce) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "254.m1", CRC(994b4487) SHA1(a4e645a3ababa48a8325980ff022e04a8b51b017) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "254.v1", 0x000000, 0x400000, CRC(844f58fb) SHA1(e59544457be9f21481eac8b5a39b9cbb502f252d) )
	ROM_LOAD( "254.v2", 0x400000, 0x400000, CRC(d9a248f0) SHA1(dd3e0974b753e6f94d0943a002de45668a1b072b) )
	ROM_LOAD( "254.v3", 0x800000, 0x400000, CRC(0b0d2d33) SHA1(f8e76af42a997f36a40f66b39de00f68afe6a89c) )
	ROM_LOAD( "254.v4", 0xc00000, 0x400000, CRC(6d13dc91) SHA1(8433513c0b5aea61939068a25ab90efbe3e44116) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "254.c1", 0x0000000, 0x800000, CRC(ae6fc8ef) SHA1(544ccdaee8a4a45cdce9483e30852811d2d5f3cc) )
	ROM_LOAD16_BYTE( "254.c2", 0x0000001, 0x800000, CRC(436fa176) SHA1(d70141a91a360a1b1070753086f976608fec38af) )
	ROM_LOAD16_BYTE( "254.c3", 0x1000000, 0x800000, CRC(e53ff2dc) SHA1(31f6aaffe28146d574aa72f14f90a9d968f36bc6) )
	ROM_LOAD16_BYTE( "254.c4", 0x1000001, 0x800000, CRC(818672f0) SHA1(460c6738d0ee5ae440a23fc1434fab53bbb242b5) )
	ROM_LOAD16_BYTE( "254.c5", 0x2000000, 0x800000, CRC(4580eacd) SHA1(feb96eb5e80c9125ddd7836e0939212cd3011c34) )
	ROM_LOAD16_BYTE( "254.c6", 0x2000001, 0x800000, CRC(e34970fc) SHA1(6e43e15e27bc914357f977116ab1e2d98711bb21) )
	ROM_LOAD16_BYTE( "254.c7", 0x3000000, 0x800000, CRC(f2323239) SHA1(5b3e8dd77474203be010ec7363858d806344a320) )
	ROM_LOAD16_BYTE( "254.c8", 0x3000001, 0x800000, CRC(66848c7d) SHA1(24d4ed627940a4cf8129761c1da15556e52e199c) )
ROM_END

 /********
  bootleg
***********/

ROM_START( s1945pd )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "254.p1",  0x000000, 0x100000, CRC(ff8efcff) SHA1(dcaeaca573385c172ecc43ee6bee355359091893) )
	ROM_LOAD16_WORD_SWAP( "254.p2", 0x100000, 0x400000, CRC(efdfd4dd) SHA1(254f3e1b546eed788f7ae919be9d1bf9702148ce) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "254.m1", CRC(994b4487) SHA1(a4e645a3ababa48a8325980ff022e04a8b51b017) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "254.v1", 0x000000, 0x400000, CRC(844f58fb) SHA1(e59544457be9f21481eac8b5a39b9cbb502f252d) )
	ROM_LOAD( "254.v2", 0x400000, 0x400000, CRC(d9a248f0) SHA1(dd3e0974b753e6f94d0943a002de45668a1b072b) )
	ROM_LOAD( "254.v3", 0x800000, 0x400000, CRC(0b0d2d33) SHA1(f8e76af42a997f36a40f66b39de00f68afe6a89c) )
	ROM_LOAD( "254.v4", 0xc00000, 0x400000, CRC(6d13dc91) SHA1(8433513c0b5aea61939068a25ab90efbe3e44116) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "254d.c1", 0x0000000, 0x800000, CRC(7b6902f9) SHA1(c974a3cd84fa15d5ae7c9053adbb004a9aa902cd) )
	ROM_LOAD16_BYTE( "254d.c2", 0x0000001, 0x800000, CRC(51bd4252) SHA1(5a99563de1d83b7cd1d7ca7a50c71d5f30439276) )
	ROM_LOAD16_BYTE( "254d.c3", 0x1000000, 0x800000, CRC(a38993e4) SHA1(0bd3bcecf17abfda7da997e348fe3b0e7a66a828) )
	ROM_LOAD16_BYTE( "254d.c4", 0x1000001, 0x800000, CRC(d5696530) SHA1(15cbdaa729b8dd4d2489157d778379cfb0f79e51) )
	ROM_LOAD16_BYTE( "254d.c5", 0x2000000, 0x800000, CRC(28764bd6) SHA1(21158ea89dd88abd7f73b4513b699089f7944ba5) )
	ROM_LOAD16_BYTE( "254d.c6", 0x2000001, 0x800000, CRC(9931bdf1) SHA1(fa95fd8621403684e09f01a89f8df9ae2fa7de76) )
	ROM_LOAD16_BYTE( "254d.c7", 0x3000000, 0x800000, CRC(6e12afcd) SHA1(ef45d9b923ec937f9c207cd50d917f0ded5b8faf) )
	ROM_LOAD16_BYTE( "254d.c8", 0x3000001, 0x800000, CRC(178d4684) SHA1(3cbf379209c189ee48260a044dc2754035f57e14) )
ROM_END

 /***************
 Samurai Shodown
******************/

ROM_START( samshohack01 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "045_hack01.p1",  0x000000, 0x100000, CRC(8e4310a8) SHA1(ff35819ce9ed1d9bdda1b1c4c15e2c402b91728b) )
	ROM_LOAD16_WORD_SWAP( "045b.p2", 0x100000, 0x080000, CRC(38ee9ba9) SHA1(48190699a6be83cb6257365ae81f93fdd23abe09) )

	NEO_SFIX_128K( "045.s1", CRC(9142a4d3) SHA1(54088e99fcfd75fd0f94852890a56350066a05a3) )

	NEO_BIOS_AUDIO_128K( "045.m1", CRC(95170640) SHA1(125c502db0693e8d11cef619b090081c14a9a300) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "045.v1", 0x000000, 0x200000, CRC(37f78a9b) SHA1(6279b497d12fa90b49ab5ac3aae20fb302ec8b81) )
	ROM_LOAD( "045.v2", 0x200000, 0x200000, CRC(568b20cf) SHA1(61af858685472a1fad608e230cccc2b108509ddb) )

	ROM_REGION( 0x900000, "sprites", 0 )
	ROM_LOAD16_BYTE( "045.c1", 0x000000, 0x200000, CRC(2e5873a4) SHA1(65c74c1e2d34390666bbb630df7d1f4c9570c3db) )
	ROM_LOAD16_BYTE( "045.c2", 0x000001, 0x200000, CRC(04febb10) SHA1(16a8cbf0fd9468e81bf9eab6dbe7a8e3623a843e) )
	ROM_LOAD16_BYTE( "045.c3", 0x400000, 0x200000, CRC(f3dabd1e) SHA1(c80e52df42be9f8b2e89b467b11ab140a480cee8) )
	ROM_LOAD16_BYTE( "045.c4", 0x400001, 0x200000, CRC(935c62f0) SHA1(0053d40085fac14096b683f4341f65e543b71dc1) )
	ROM_LOAD16_BYTE( "045b.c5", 0x800000, 0x080000, CRC(a2bb8284) SHA1(aa118e3b8c062daa219b36758b9a3814c08c69dc) )
	ROM_LOAD16_BYTE( "045b.c6", 0x800001, 0x080000, CRC(4fa71252) SHA1(afe374a9d1f2d955a59efe7b6196b89e021b164c) )
ROM_END

ROM_START( samshohack02 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "045_hack02.p1",  0x000000, 0x100000, CRC(8ec6803d) SHA1(7db5354cc58088f2427e0b8716af66a2f31765f7) )
	ROM_LOAD16_WORD_SWAP( "045b.p2", 0x100000, 0x080000, CRC(38ee9ba9) SHA1(48190699a6be83cb6257365ae81f93fdd23abe09) )

	NEO_SFIX_128K( "045.s1", CRC(9142a4d3) SHA1(54088e99fcfd75fd0f94852890a56350066a05a3) )

	NEO_BIOS_AUDIO_128K( "045.m1", CRC(95170640) SHA1(125c502db0693e8d11cef619b090081c14a9a300) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "045.v1", 0x000000, 0x200000, CRC(37f78a9b) SHA1(6279b497d12fa90b49ab5ac3aae20fb302ec8b81) )
	ROM_LOAD( "045.v2", 0x200000, 0x200000, CRC(568b20cf) SHA1(61af858685472a1fad608e230cccc2b108509ddb) )

	ROM_REGION( 0x900000, "sprites", 0 )
	ROM_LOAD16_BYTE( "045.c1", 0x000000, 0x200000, CRC(2e5873a4) SHA1(65c74c1e2d34390666bbb630df7d1f4c9570c3db) )
	ROM_LOAD16_BYTE( "045.c2", 0x000001, 0x200000, CRC(04febb10) SHA1(16a8cbf0fd9468e81bf9eab6dbe7a8e3623a843e) )
	ROM_LOAD16_BYTE( "045.c3", 0x400000, 0x200000, CRC(f3dabd1e) SHA1(c80e52df42be9f8b2e89b467b11ab140a480cee8) )
	ROM_LOAD16_BYTE( "045.c4", 0x400001, 0x200000, CRC(935c62f0) SHA1(0053d40085fac14096b683f4341f65e543b71dc1) )
	ROM_LOAD16_BYTE( "045b.c5", 0x800000, 0x080000, CRC(a2bb8284) SHA1(aa118e3b8c062daa219b36758b9a3814c08c69dc) )
	ROM_LOAD16_BYTE( "045b.c6", 0x800001, 0x080000, CRC(4fa71252) SHA1(afe374a9d1f2d955a59efe7b6196b89e021b164c) )
ROM_END

ROM_START( samshohack03 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "045_hack03.p1",  0x000000, 0x100000, CRC(84879609) SHA1(5e2b175b611f1c9f3b2720c6f6499b6a725e5c0e) )
	ROM_LOAD16_WORD_SWAP( "045b.p2", 0x100000, 0x080000, CRC(38ee9ba9) SHA1(48190699a6be83cb6257365ae81f93fdd23abe09) )

	NEO_SFIX_128K( "045.s1", CRC(9142a4d3) SHA1(54088e99fcfd75fd0f94852890a56350066a05a3) )

	NEO_BIOS_AUDIO_128K( "045.m1", CRC(95170640) SHA1(125c502db0693e8d11cef619b090081c14a9a300) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "045.v1", 0x000000, 0x200000, CRC(37f78a9b) SHA1(6279b497d12fa90b49ab5ac3aae20fb302ec8b81) )
	ROM_LOAD( "045.v2", 0x200000, 0x200000, CRC(568b20cf) SHA1(61af858685472a1fad608e230cccc2b108509ddb) )

	ROM_REGION( 0x900000, "sprites", 0 )
	ROM_LOAD16_BYTE( "045.c1", 0x000000, 0x200000, CRC(2e5873a4) SHA1(65c74c1e2d34390666bbb630df7d1f4c9570c3db) )
	ROM_LOAD16_BYTE( "045.c2", 0x000001, 0x200000, CRC(04febb10) SHA1(16a8cbf0fd9468e81bf9eab6dbe7a8e3623a843e) )
	ROM_LOAD16_BYTE( "045.c3", 0x400000, 0x200000, CRC(f3dabd1e) SHA1(c80e52df42be9f8b2e89b467b11ab140a480cee8) )
	ROM_LOAD16_BYTE( "045.c4", 0x400001, 0x200000, CRC(935c62f0) SHA1(0053d40085fac14096b683f4341f65e543b71dc1) )
	ROM_LOAD16_BYTE( "045b.c5", 0x800000, 0x080000, CRC(a2bb8284) SHA1(aa118e3b8c062daa219b36758b9a3814c08c69dc) )
	ROM_LOAD16_BYTE( "045b.c6", 0x800001, 0x080000, CRC(4fa71252) SHA1(afe374a9d1f2d955a59efe7b6196b89e021b164c) )
ROM_END

 /******************
 Samurai Shodown II
*********************/

ROM_START( samsho2hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "063_hack01.p1", 0x100000, 0x100000, CRC(7ab45a4c) SHA1(82a2dde5f2558b377bfe34b8a91e1b9dd080be6e) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "063.m1", CRC(56675098) SHA1(90429fc40d056d480d0e2bbefbc691d9fa260fc4) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "063.v1", 0x000000, 0x200000, CRC(37703f91) SHA1(a373ebef4c33ba1d8340e826981a58769aada238) )
	ROM_LOAD( "063.v2", 0x200000, 0x200000, CRC(0142bde8) SHA1(0be6c53acac44802bf70b6925452f70289a139d9) )
	ROM_LOAD( "063.v3", 0x400000, 0x200000, CRC(d07fa5ca) SHA1(1da7f081f8b8fc86a91feacf900f573218d82676) )
	ROM_LOAD( "063.v4", 0x600000, 0x100000, CRC(24aab4bb) SHA1(10ee4c5b3579865b93dcc1e4079963276aa700a6) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "063.c1", 0x000000, 0x200000, CRC(86cd307c) SHA1(0d04336f7c436d74638d8c1cd8651faf436a6bec) )
	ROM_LOAD16_BYTE( "063.c2", 0x000001, 0x200000, CRC(cdfcc4ca) SHA1(179dc81432424d68cefedd20cc1c4b2a95deb891) )
	ROM_LOAD16_BYTE( "063.c3", 0x400000, 0x200000, CRC(7a63ccc7) SHA1(49d97c543bc2860d493a353ab0d059088c6fbd21) )
	ROM_LOAD16_BYTE( "063.c4", 0x400001, 0x200000, CRC(751025ce) SHA1(e1bbaa7cd67fd04e4aab7f7ea77f63ae1cbc90d0) )
	ROM_LOAD16_BYTE( "063.c5", 0x800000, 0x200000, CRC(20d3a475) SHA1(28da44a136bd14c73c62c147c3f6e6bcfa1066de) )
	ROM_LOAD16_BYTE( "063.c6", 0x800001, 0x200000, CRC(ae4c0a88) SHA1(cc8a7d11daa3821f83a6fd0942534706f939e576) )
	ROM_LOAD16_BYTE( "063.c7", 0xc00000, 0x200000, CRC(2df3cbcf) SHA1(e54f9022359963711451c2025825b862d36c6975) )
	ROM_LOAD16_BYTE( "063.c8", 0xc00001, 0x200000, CRC(1ffc6dfa) SHA1(acea18aca76c072e0bac2a364fc96d49cfc86e77) )
ROM_END

ROM_START( samsho2hack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "063_hack02.p1", 0x100000, 0x100000, CRC(07fed89d) SHA1(c5373e729fd9f0e06d9accad8babf264000ca198) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "063.m1", CRC(56675098) SHA1(90429fc40d056d480d0e2bbefbc691d9fa260fc4) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "063.v1", 0x000000, 0x200000, CRC(37703f91) SHA1(a373ebef4c33ba1d8340e826981a58769aada238) )
	ROM_LOAD( "063.v2", 0x200000, 0x200000, CRC(0142bde8) SHA1(0be6c53acac44802bf70b6925452f70289a139d9) )
	ROM_LOAD( "063.v3", 0x400000, 0x200000, CRC(d07fa5ca) SHA1(1da7f081f8b8fc86a91feacf900f573218d82676) )
	ROM_LOAD( "063.v4", 0x600000, 0x100000, CRC(24aab4bb) SHA1(10ee4c5b3579865b93dcc1e4079963276aa700a6) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "063.c1", 0x000000, 0x200000, CRC(86cd307c) SHA1(0d04336f7c436d74638d8c1cd8651faf436a6bec) )
	ROM_LOAD16_BYTE( "063.c2", 0x000001, 0x200000, CRC(cdfcc4ca) SHA1(179dc81432424d68cefedd20cc1c4b2a95deb891) )
	ROM_LOAD16_BYTE( "063.c3", 0x400000, 0x200000, CRC(7a63ccc7) SHA1(49d97c543bc2860d493a353ab0d059088c6fbd21) )
	ROM_LOAD16_BYTE( "063.c4", 0x400001, 0x200000, CRC(751025ce) SHA1(e1bbaa7cd67fd04e4aab7f7ea77f63ae1cbc90d0) )
	ROM_LOAD16_BYTE( "063.c5", 0x800000, 0x200000, CRC(20d3a475) SHA1(28da44a136bd14c73c62c147c3f6e6bcfa1066de) )
	ROM_LOAD16_BYTE( "063.c6", 0x800001, 0x200000, CRC(ae4c0a88) SHA1(cc8a7d11daa3821f83a6fd0942534706f939e576) )
	ROM_LOAD16_BYTE( "063.c7", 0xc00000, 0x200000, CRC(2df3cbcf) SHA1(e54f9022359963711451c2025825b862d36c6975) )
	ROM_LOAD16_BYTE( "063.c8", 0xc00001, 0x200000, CRC(1ffc6dfa) SHA1(acea18aca76c072e0bac2a364fc96d49cfc86e77) )
ROM_END

ROM_START( samsho2hack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "063_hack03.p1",  0x000000, 0x100000, CRC(ed466b3d) SHA1(a3d5029d2fa29184e40038ca2a92ae09596646ea) )
	ROM_LOAD16_WORD_SWAP( "063_hack03.p2",  0x100000, 0x100000, CRC(6e1aef70) SHA1(7a035e80b6f6af1d105ecccf3f5a2c44470366f3) )

	ROM_REGION( 0x020000, "gsc", ROMREGION_BE | ROMREGION_16BIT )
	ROM_LOAD16_WORD_SWAP( "063_hack03.p3", 0x000000, 0x020000, CRC(f9079ea3) SHA1(76a4590bda85d1bdf38f6e02ce4d761f951ff79d) )

	NEO_SFIX_128K( "063_hack03.s1", CRC(1951a907) SHA1(ee0e4743d7c2ba6306f08f16de2e3fea6697a89e) )

	NEO_BIOS_AUDIO_128K( "063.m1", CRC(56675098) SHA1(90429fc40d056d480d0e2bbefbc691d9fa260fc4) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "063.v1", 0x000000, 0x200000, CRC(37703f91) SHA1(a373ebef4c33ba1d8340e826981a58769aada238) )
	ROM_LOAD( "063.v2", 0x200000, 0x200000, CRC(0142bde8) SHA1(0be6c53acac44802bf70b6925452f70289a139d9) )
	ROM_LOAD( "063.v3", 0x400000, 0x200000, CRC(d07fa5ca) SHA1(1da7f081f8b8fc86a91feacf900f573218d82676) )
	ROM_LOAD( "063.v4", 0x600000, 0x100000, CRC(24aab4bb) SHA1(10ee4c5b3579865b93dcc1e4079963276aa700a6) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "063.c1", 0x000000, 0x200000, CRC(86cd307c) SHA1(0d04336f7c436d74638d8c1cd8651faf436a6bec) )
	ROM_LOAD16_BYTE( "063.c2", 0x000001, 0x200000, CRC(cdfcc4ca) SHA1(179dc81432424d68cefedd20cc1c4b2a95deb891) )
	ROM_LOAD16_BYTE( "063.c3", 0x400000, 0x200000, CRC(7a63ccc7) SHA1(49d97c543bc2860d493a353ab0d059088c6fbd21) )
	ROM_LOAD16_BYTE( "063.c4", 0x400001, 0x200000, CRC(751025ce) SHA1(e1bbaa7cd67fd04e4aab7f7ea77f63ae1cbc90d0) )
	ROM_LOAD16_BYTE( "063.c5", 0x800000, 0x200000, CRC(20d3a475) SHA1(28da44a136bd14c73c62c147c3f6e6bcfa1066de) )
	ROM_LOAD16_BYTE( "063.c6", 0x800001, 0x200000, CRC(ae4c0a88) SHA1(cc8a7d11daa3821f83a6fd0942534706f939e576) )
	ROM_LOAD16_BYTE( "063_hack03.c7", 0xc00000, 0x200000, CRC(dbebced2) SHA1(c1db8a0a6814b8078934b68f1f579060e36f67b2) )
	ROM_LOAD16_BYTE( "063_hack03.c8", 0xc00001, 0x200000, CRC(bf70b93c) SHA1(1bffeb6c7e158b9aee33802fe86f75cee04c6ecc) )
ROM_END

ROM_START( samsho2hack04 ) //samsho2l
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "063_hack04.p1", 0x100000, 0x100000, CRC(4e3b4620) SHA1(3c978e4f182c930e941689e7bbc152d23cb1bbbf) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "063.s1", CRC(64a5cd66) SHA1(12cdfb27bf9ccd5a8df6ddd4628ef7cf2c6d4964) )

	NEO_BIOS_AUDIO_128K( "063.m1", CRC(56675098) SHA1(90429fc40d056d480d0e2bbefbc691d9fa260fc4) )

	ROM_REGION( 0x700000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "063.v1", 0x000000, 0x200000, CRC(37703f91) SHA1(a373ebef4c33ba1d8340e826981a58769aada238) )
	ROM_LOAD( "063.v2", 0x200000, 0x200000, CRC(0142bde8) SHA1(0be6c53acac44802bf70b6925452f70289a139d9) )
	ROM_LOAD( "063.v3", 0x400000, 0x200000, CRC(d07fa5ca) SHA1(1da7f081f8b8fc86a91feacf900f573218d82676) )
	ROM_LOAD( "063.v4", 0x600000, 0x100000, CRC(24aab4bb) SHA1(10ee4c5b3579865b93dcc1e4079963276aa700a6) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "063.c1", 0x000000, 0x200000, CRC(86cd307c) SHA1(0d04336f7c436d74638d8c1cd8651faf436a6bec) )
	ROM_LOAD16_BYTE( "063.c2", 0x000001, 0x200000, CRC(cdfcc4ca) SHA1(179dc81432424d68cefedd20cc1c4b2a95deb891) )
	ROM_LOAD16_BYTE( "063.c3", 0x400000, 0x200000, CRC(7a63ccc7) SHA1(49d97c543bc2860d493a353ab0d059088c6fbd21) )
	ROM_LOAD16_BYTE( "063.c4", 0x400001, 0x200000, CRC(751025ce) SHA1(e1bbaa7cd67fd04e4aab7f7ea77f63ae1cbc90d0) )
	ROM_LOAD16_BYTE( "063.c5", 0x800000, 0x200000, CRC(20d3a475) SHA1(28da44a136bd14c73c62c147c3f6e6bcfa1066de) )
	ROM_LOAD16_BYTE( "063.c6", 0x800001, 0x200000, CRC(ae4c0a88) SHA1(cc8a7d11daa3821f83a6fd0942534706f939e576) )
	ROM_LOAD16_BYTE( "063.c7", 0xc00000, 0x200000, CRC(2df3cbcf) SHA1(e54f9022359963711451c2025825b862d36c6975) )
	ROM_LOAD16_BYTE( "063.c8", 0xc00001, 0x200000, CRC(1ffc6dfa) SHA1(acea18aca76c072e0bac2a364fc96d49cfc86e77) )
ROM_END

 /*******************
 Samurai Shodown III
**********************/

ROM_START( samsho3hack01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "087_hack01.p1", 0x000000, 0x080000, CRC(6aca07b5) SHA1(4e457c8794590e63dd774ba7c7f62f6d84bdd4e9) )
	ROM_LOAD16_WORD_SWAP( "087.p2", 0x080000, 0x080000, CRC(256f5302) SHA1(e2d21b413a6059194a994b7902b2a7df98a15151) )
	ROM_LOAD16_WORD_SWAP( "087.p3", 0x100000, 0x080000, CRC(bf2db5dd) SHA1(b4fa1dc1eccc9eb1ce74f0a06992ef89b1cbc732) )
	ROM_LOAD16_WORD_SWAP( "087.p4", 0x180000, 0x080000, CRC(53e60c58) SHA1(f975e81cab6322d3260348402721c673023259fa) )
	ROM_LOAD16_WORD_SWAP( "087.p5",  0x200000, 0x100000, CRC(e86ca4af) SHA1(5246acbab77ac2f232b88b8522187764ff0872f0) )

	NEO_SFIX_128K( "087.s1", CRC(74ec7d9f) SHA1(d79c479838a7ca51735a44f91f1968ec5b3c6b91) )

	NEO_BIOS_AUDIO_128K( "087.m1", CRC(8e6440eb) SHA1(e3f72150af4e326543b29df71cda27d73ec087c1) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "087.v1", 0x000000, 0x400000, CRC(84bdd9a0) SHA1(adceceb00569eca13fcc2e0f0d9f0d9b06a06851) )
	ROM_LOAD( "087.v2", 0x400000, 0x200000, CRC(ac0f261a) SHA1(5411bdff24cba7fdbc3397d45a70fb468d7a44b3) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "087.c1", 0x0000000, 0x400000, CRC(07a233bc) SHA1(654cb56cfd6eeebe6745c0b8b730317fb8ccd3d9) )
	ROM_LOAD16_BYTE( "087.c2", 0x0000001, 0x400000, CRC(7a413592) SHA1(b8c7a2d0d7a8b14d6cab94d7a5f347e73c6ab7a4) )
	ROM_LOAD16_BYTE( "087.c3", 0x0800000, 0x400000, CRC(8b793796) SHA1(053acc129ea56691607a5d255845703e61fd3ada) )
	ROM_LOAD16_BYTE( "087.c4", 0x0800001, 0x400000, CRC(728fbf11) SHA1(daa319d455f759bfc08a37b43218bdb48dc1c9e5) )
	ROM_LOAD16_BYTE( "087.c5", 0x1000000, 0x400000, CRC(172ab180) SHA1(a6122f683bdb78d0079e1e360c1b96ba28def7b7) )
	ROM_LOAD16_BYTE( "087.c6", 0x1000001, 0x400000, CRC(002ff8f3) SHA1(3a378708697d727796c4f702dd5bbf1c9eb4daec) )
	ROM_LOAD16_BYTE( "087.c7", 0x1800000, 0x100000, CRC(ae450e3d) SHA1(ec482632cc347ec3f9e68df0ebcaa16ebe41b9f9) )
	ROM_LOAD16_BYTE( "087.c8", 0x1800001, 0x100000, CRC(a9e82717) SHA1(e39ee15d5140dbe7f06eea945cce9984a5e8b06a) )
ROM_END

ROM_START( samsho3hack02 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "087_hack02.p1", 0x000000, 0x100000, CRC(c0a9694e) SHA1(ff17a12166cb877829050343b2ae09a5717a5cce) )
	ROM_LOAD16_WORD_SWAP( "087k.p2", 0x100000, 0x200000, CRC(9bbe27e0) SHA1(b18117102159903c8e8f4e4226e1cc91a400e816) )

	NEO_SFIX_128K( "087.s1", CRC(74ec7d9f) SHA1(d79c479838a7ca51735a44f91f1968ec5b3c6b91) )

	NEO_BIOS_AUDIO_128K( "087.m1", CRC(8e6440eb) SHA1(e3f72150af4e326543b29df71cda27d73ec087c1) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "087.v1", 0x000000, 0x400000, CRC(84bdd9a0) SHA1(adceceb00569eca13fcc2e0f0d9f0d9b06a06851) )
	ROM_LOAD( "087.v2", 0x400000, 0x200000, CRC(ac0f261a) SHA1(5411bdff24cba7fdbc3397d45a70fb468d7a44b3) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "087.c1", 0x0000000, 0x400000, CRC(07a233bc) SHA1(654cb56cfd6eeebe6745c0b8b730317fb8ccd3d9) )
	ROM_LOAD16_BYTE( "087.c2", 0x0000001, 0x400000, CRC(7a413592) SHA1(b8c7a2d0d7a8b14d6cab94d7a5f347e73c6ab7a4) )
	ROM_LOAD16_BYTE( "087.c3", 0x0800000, 0x400000, CRC(8b793796) SHA1(053acc129ea56691607a5d255845703e61fd3ada) )
	ROM_LOAD16_BYTE( "087.c4", 0x0800001, 0x400000, CRC(728fbf11) SHA1(daa319d455f759bfc08a37b43218bdb48dc1c9e5) )
	ROM_LOAD16_BYTE( "087.c5", 0x1000000, 0x400000, CRC(172ab180) SHA1(a6122f683bdb78d0079e1e360c1b96ba28def7b7) )
	ROM_LOAD16_BYTE( "087.c6", 0x1000001, 0x400000, CRC(002ff8f3) SHA1(3a378708697d727796c4f702dd5bbf1c9eb4daec) )
	ROM_LOAD16_BYTE( "087.c7", 0x1800000, 0x100000, CRC(ae450e3d) SHA1(ec482632cc347ec3f9e68df0ebcaa16ebe41b9f9) )
	ROM_LOAD16_BYTE( "087.c8", 0x1800001, 0x100000, CRC(a9e82717) SHA1(e39ee15d5140dbe7f06eea945cce9984a5e8b06a) )
ROM_END

ROM_START( samsho3hack03 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "087_hack03.p1", 0x000000, 0x100000, CRC(1610e9bd) SHA1(588cc070586d61a64779ded0de66a012ddc48b59) )
	ROM_LOAD16_WORD_SWAP( "087_hack03.p2", 0x100000, 0x200000, CRC(509ab676) SHA1(34895d5b91a0e9d3f6c60f9fbdef3fa46e1e9525) )

	NEO_SFIX_128K( "087.s1", CRC(74ec7d9f) SHA1(d79c479838a7ca51735a44f91f1968ec5b3c6b91) )

	NEO_BIOS_AUDIO_128K( "087.m1", CRC(8e6440eb) SHA1(e3f72150af4e326543b29df71cda27d73ec087c1) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "087.v1", 0x000000, 0x400000, CRC(84bdd9a0) SHA1(adceceb00569eca13fcc2e0f0d9f0d9b06a06851) )
	ROM_LOAD( "087.v2", 0x400000, 0x200000, CRC(ac0f261a) SHA1(5411bdff24cba7fdbc3397d45a70fb468d7a44b3) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "087.c1", 0x0000000, 0x400000, CRC(07a233bc) SHA1(654cb56cfd6eeebe6745c0b8b730317fb8ccd3d9) )
	ROM_LOAD16_BYTE( "087.c2", 0x0000001, 0x400000, CRC(7a413592) SHA1(b8c7a2d0d7a8b14d6cab94d7a5f347e73c6ab7a4) )
	ROM_LOAD16_BYTE( "087.c3", 0x0800000, 0x400000, CRC(8b793796) SHA1(053acc129ea56691607a5d255845703e61fd3ada) )
	ROM_LOAD16_BYTE( "087.c4", 0x0800001, 0x400000, CRC(728fbf11) SHA1(daa319d455f759bfc08a37b43218bdb48dc1c9e5) )
	ROM_LOAD16_BYTE( "087.c5", 0x1000000, 0x400000, CRC(172ab180) SHA1(a6122f683bdb78d0079e1e360c1b96ba28def7b7) )
	ROM_LOAD16_BYTE( "087.c6", 0x1000001, 0x400000, CRC(002ff8f3) SHA1(3a378708697d727796c4f702dd5bbf1c9eb4daec) )
	ROM_LOAD16_BYTE( "087.c7", 0x1800000, 0x100000, CRC(ae450e3d) SHA1(ec482632cc347ec3f9e68df0ebcaa16ebe41b9f9) )
	ROM_LOAD16_BYTE( "087.c8", 0x1800001, 0x100000, CRC(a9e82717) SHA1(e39ee15d5140dbe7f06eea945cce9984a5e8b06a) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( samsho3hb )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "087hb.p1", 0x000000, 0x100000, CRC(15e021ca) SHA1(78aba32741d2e4b5bc4c641a4166f4405ab240e2) )
	ROM_LOAD16_WORD_SWAP( "087hb.p2", 0x100000, 0x200000, CRC(1cc2c370) SHA1(01fba8d315d684652fb506aa833fa0e8e3e350df) )

	NEO_SFIX_128K( "087.s1", CRC(74ec7d9f) SHA1(d79c479838a7ca51735a44f91f1968ec5b3c6b91) )

	NEO_BIOS_AUDIO_128K( "087.m1", CRC(8e6440eb) SHA1(e3f72150af4e326543b29df71cda27d73ec087c1) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "087.v1", 0x000000, 0x400000, CRC(84bdd9a0) SHA1(adceceb00569eca13fcc2e0f0d9f0d9b06a06851) )
	ROM_LOAD( "087.v2", 0x400000, 0x200000, CRC(ac0f261a) SHA1(5411bdff24cba7fdbc3397d45a70fb468d7a44b3) )

	ROM_REGION( 0x1a00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "087.c1", 0x0000000, 0x400000, CRC(07a233bc) SHA1(654cb56cfd6eeebe6745c0b8b730317fb8ccd3d9) )
	ROM_LOAD16_BYTE( "087.c2", 0x0000001, 0x400000, CRC(7a413592) SHA1(b8c7a2d0d7a8b14d6cab94d7a5f347e73c6ab7a4) )
	ROM_LOAD16_BYTE( "087.c3", 0x0800000, 0x400000, CRC(8b793796) SHA1(053acc129ea56691607a5d255845703e61fd3ada) )
	ROM_LOAD16_BYTE( "087.c4", 0x0800001, 0x400000, CRC(728fbf11) SHA1(daa319d455f759bfc08a37b43218bdb48dc1c9e5) )
	ROM_LOAD16_BYTE( "087.c5", 0x1000000, 0x400000, CRC(172ab180) SHA1(a6122f683bdb78d0079e1e360c1b96ba28def7b7) )
	ROM_LOAD16_BYTE( "087.c6", 0x1000001, 0x400000, CRC(002ff8f3) SHA1(3a378708697d727796c4f702dd5bbf1c9eb4daec) )
	ROM_LOAD16_BYTE( "087.c7", 0x1800000, 0x100000, CRC(ae450e3d) SHA1(ec482632cc347ec3f9e68df0ebcaa16ebe41b9f9) )
	ROM_LOAD16_BYTE( "087.c8", 0x1800001, 0x100000, CRC(a9e82717) SHA1(e39ee15d5140dbe7f06eea945cce9984a5e8b06a) )
ROM_END

 /******************
 Samurai Shodown IV
*********************/

ROM_START( samsho4hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "222_hack01.p1",   0x000000, 0x100000, CRC(463035fc) SHA1(f84f12929ca36fc029761560a13413dc293914a5) )
	ROM_LOAD16_WORD_SWAP( "222.p2", 0x100000, 0x400000, CRC(b023cd8b) SHA1(35b4cec9858225f90acdfa16ed8a3017d0d08327) )

	NEO_SFIX_128K( "222bh.s1", CRC(d0906101) SHA1(16873a82c408a5585d4e424e953b451981a8d6b2) )

	NEO_BIOS_AUDIO_128K( "222.m1", CRC(7615bc1b) SHA1(b936f7b341f6fe0921b4c41049734684583e3596) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "222.v1", 0x000000, 0x400000, CRC(7d6ba95f) SHA1(03cb4e0d770e0b332b07b64cacef624460b84c78) )
	ROM_LOAD( "222.v2", 0x400000, 0x400000, CRC(6c33bb5d) SHA1(fd5d4e08a962dd0d22c52c91bad5ec7f23cfb901) )
	ROM_LOAD( "222.v3", 0x800000, 0x200000, CRC(831ea8c0) SHA1(f2987b7d09bdc4311e972ce8a9ab7ca9802db4db) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "222.c1", 0x0000000, 0x400000, CRC(68f2ed95) SHA1(c0a02df012cd25bcfe341770ea861a80294148cb) )
	ROM_LOAD16_BYTE( "222.c2", 0x0000001, 0x400000, CRC(a6e9aff0) SHA1(15addca49951ed53fa3c000c8d7cd327d012a620) )
	ROM_LOAD16_BYTE( "222.c3", 0x0800000, 0x400000, CRC(c91b40f4) SHA1(dcda45e0336204e3e024de08edfd0a3217bc1fdd) )
	ROM_LOAD16_BYTE( "222.c4", 0x0800001, 0x400000, CRC(359510a4) SHA1(b6642677ebdff7788263266402080272b8a66b15) )
	ROM_LOAD16_BYTE( "222.c5", 0x1000000, 0x400000, CRC(9cfbb22d) SHA1(789c32f917d0c6e38601cd390a7bf9d803131a4a) )
	ROM_LOAD16_BYTE( "222.c6", 0x1000001, 0x400000, CRC(685efc32) SHA1(db21ba1c7e3631ce0f1cb6f503ae7e0e043ff71b) )
	ROM_LOAD16_BYTE( "222.c7", 0x1800000, 0x400000, CRC(d0f86f0d) SHA1(32502d71c2ab1469c492b6b382bf2bb3f85981d9) )
	ROM_LOAD16_BYTE( "222.c8", 0x1800001, 0x400000, CRC(adfc50e3) SHA1(7d7ee874355b5aa75ad9c9a5c9c3df98d098d85e) )
ROM_END

ROM_START( samsho4hack02 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "222_hack02.p1",   0x000000, 0x100000, CRC(5bcb6ad5) SHA1(ee22d3ab96957193e281760005b43a3f83438588) )
	ROM_LOAD16_WORD_SWAP( "222.p2", 0x100000, 0x400000, CRC(b023cd8b) SHA1(35b4cec9858225f90acdfa16ed8a3017d0d08327) )

	NEO_SFIX_128K( "222bh.s1", CRC(d0906101) SHA1(16873a82c408a5585d4e424e953b451981a8d6b2) )

	NEO_BIOS_AUDIO_128K( "222.m1", CRC(7615bc1b) SHA1(b936f7b341f6fe0921b4c41049734684583e3596) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "222.v1", 0x000000, 0x400000, CRC(7d6ba95f) SHA1(03cb4e0d770e0b332b07b64cacef624460b84c78) )
	ROM_LOAD( "222.v2", 0x400000, 0x400000, CRC(6c33bb5d) SHA1(fd5d4e08a962dd0d22c52c91bad5ec7f23cfb901) )
	ROM_LOAD( "222.v3", 0x800000, 0x200000, CRC(831ea8c0) SHA1(f2987b7d09bdc4311e972ce8a9ab7ca9802db4db) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "222.c1", 0x0000000, 0x400000, CRC(68f2ed95) SHA1(c0a02df012cd25bcfe341770ea861a80294148cb) )
	ROM_LOAD16_BYTE( "222.c2", 0x0000001, 0x400000, CRC(a6e9aff0) SHA1(15addca49951ed53fa3c000c8d7cd327d012a620) )
	ROM_LOAD16_BYTE( "222.c3", 0x0800000, 0x400000, CRC(c91b40f4) SHA1(dcda45e0336204e3e024de08edfd0a3217bc1fdd) )
	ROM_LOAD16_BYTE( "222.c4", 0x0800001, 0x400000, CRC(359510a4) SHA1(b6642677ebdff7788263266402080272b8a66b15) )
	ROM_LOAD16_BYTE( "222.c5", 0x1000000, 0x400000, CRC(9cfbb22d) SHA1(789c32f917d0c6e38601cd390a7bf9d803131a4a) )
	ROM_LOAD16_BYTE( "222.c6", 0x1000001, 0x400000, CRC(685efc32) SHA1(db21ba1c7e3631ce0f1cb6f503ae7e0e043ff71b) )
	ROM_LOAD16_BYTE( "222.c7", 0x1800000, 0x400000, CRC(d0f86f0d) SHA1(32502d71c2ab1469c492b6b382bf2bb3f85981d9) )
	ROM_LOAD16_BYTE( "222.c8", 0x1800001, 0x400000, CRC(adfc50e3) SHA1(7d7ee874355b5aa75ad9c9a5c9c3df98d098d85e) )
ROM_END

ROM_START( samsho4hack03 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "222_hack03.p1",   0x000000, 0x100000, CRC(8d653294) SHA1(4a15489febcf6e3a8fe48eac88479c7197c11865) )
	ROM_LOAD16_WORD_SWAP( "222.p2", 0x100000, 0x400000, CRC(b023cd8b) SHA1(35b4cec9858225f90acdfa16ed8a3017d0d08327) )

	NEO_SFIX_128K( "222bh.s1", CRC(d0906101) SHA1(16873a82c408a5585d4e424e953b451981a8d6b2) )

	NEO_BIOS_AUDIO_128K( "222.m1", CRC(7615bc1b) SHA1(b936f7b341f6fe0921b4c41049734684583e3596) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "222.v1", 0x000000, 0x400000, CRC(7d6ba95f) SHA1(03cb4e0d770e0b332b07b64cacef624460b84c78) )
	ROM_LOAD( "222.v2", 0x400000, 0x400000, CRC(6c33bb5d) SHA1(fd5d4e08a962dd0d22c52c91bad5ec7f23cfb901) )
	ROM_LOAD( "222.v3", 0x800000, 0x200000, CRC(831ea8c0) SHA1(f2987b7d09bdc4311e972ce8a9ab7ca9802db4db) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "222.c1", 0x0000000, 0x400000, CRC(68f2ed95) SHA1(c0a02df012cd25bcfe341770ea861a80294148cb) )
	ROM_LOAD16_BYTE( "222.c2", 0x0000001, 0x400000, CRC(a6e9aff0) SHA1(15addca49951ed53fa3c000c8d7cd327d012a620) )
	ROM_LOAD16_BYTE( "222.c3", 0x0800000, 0x400000, CRC(c91b40f4) SHA1(dcda45e0336204e3e024de08edfd0a3217bc1fdd) )
	ROM_LOAD16_BYTE( "222.c4", 0x0800001, 0x400000, CRC(359510a4) SHA1(b6642677ebdff7788263266402080272b8a66b15) )
	ROM_LOAD16_BYTE( "222.c5", 0x1000000, 0x400000, CRC(9cfbb22d) SHA1(789c32f917d0c6e38601cd390a7bf9d803131a4a) )
	ROM_LOAD16_BYTE( "222.c6", 0x1000001, 0x400000, CRC(685efc32) SHA1(db21ba1c7e3631ce0f1cb6f503ae7e0e043ff71b) )
	ROM_LOAD16_BYTE( "222.c7", 0x1800000, 0x400000, CRC(d0f86f0d) SHA1(32502d71c2ab1469c492b6b382bf2bb3f85981d9) )
	ROM_LOAD16_BYTE( "222.c8", 0x1800001, 0x400000, CRC(adfc50e3) SHA1(7d7ee874355b5aa75ad9c9a5c9c3df98d098d85e) )
ROM_END

ROM_START( samsho4hack04 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "222_hack04.p1", 0x000000, 0x100000, CRC(6e98579a) SHA1(89cefbbfe161ac52c71db12088d4fa99161b5aa9) )
	ROM_LOAD16_WORD_SWAP( "222_hack04.p2", 0x100000, 0x400000, CRC(2c9c64db) SHA1(2a9825bc7577cb1f3128b6a9f41b72bb1228e037) )

	NEO_SFIX_128K( "222.s1", CRC(8d3d3bf9) SHA1(9975ed9b458bdd14e23451d2534153f68a5e4e6c) )

	NEO_BIOS_AUDIO_128K( "222.m1", CRC(7615bc1b) SHA1(b936f7b341f6fe0921b4c41049734684583e3596) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "222.v1", 0x000000, 0x400000, CRC(7d6ba95f) SHA1(03cb4e0d770e0b332b07b64cacef624460b84c78) )
	ROM_LOAD( "222.v2", 0x400000, 0x400000, CRC(6c33bb5d) SHA1(fd5d4e08a962dd0d22c52c91bad5ec7f23cfb901) )
	ROM_LOAD( "222.v3", 0x800000, 0x200000, CRC(831ea8c0) SHA1(f2987b7d09bdc4311e972ce8a9ab7ca9802db4db) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "222.c1", 0x0000000, 0x400000, CRC(68f2ed95) SHA1(c0a02df012cd25bcfe341770ea861a80294148cb) )
	ROM_LOAD16_BYTE( "222.c2", 0x0000001, 0x400000, CRC(a6e9aff0) SHA1(15addca49951ed53fa3c000c8d7cd327d012a620) )
	ROM_LOAD16_BYTE( "222.c3", 0x0800000, 0x400000, CRC(c91b40f4) SHA1(dcda45e0336204e3e024de08edfd0a3217bc1fdd) )
	ROM_LOAD16_BYTE( "222.c4", 0x0800001, 0x400000, CRC(359510a4) SHA1(b6642677ebdff7788263266402080272b8a66b15) )
	ROM_LOAD16_BYTE( "222.c5", 0x1000000, 0x400000, CRC(9cfbb22d) SHA1(789c32f917d0c6e38601cd390a7bf9d803131a4a) )
	ROM_LOAD16_BYTE( "222.c6", 0x1000001, 0x400000, CRC(685efc32) SHA1(db21ba1c7e3631ce0f1cb6f503ae7e0e043ff71b) )
	ROM_LOAD16_BYTE( "222_hack03.c7", 0x1800000, 0x400000, CRC(ecb13c24) SHA1(5f7a62e04a7c2bed52cc4d6377f41d1dbe55f6ee) )
	ROM_LOAD16_BYTE( "222_hack03.c8", 0x1800001, 0x400000, CRC(0f9a0bda) SHA1(6093f607dbcc9e1475bb1bb8e713b23fcd99c486) )
ROM_END

ROM_START( samsho4hack05 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "222_hack05.p1",   0x000000, 0x100000, CRC(c6d9523d) SHA1(16b5705659537357fd636ef5ba01dcda855dc670) )
	ROM_LOAD16_WORD_SWAP( "222.p2", 0x100000, 0x400000, CRC(b023cd8b) SHA1(35b4cec9858225f90acdfa16ed8a3017d0d08327) )

	NEO_SFIX_128K( "222bh.s1", CRC(d0906101) SHA1(16873a82c408a5585d4e424e953b451981a8d6b2) )

	NEO_BIOS_AUDIO_128K( "222.m1", CRC(7615bc1b) SHA1(b936f7b341f6fe0921b4c41049734684583e3596) )

	ROM_REGION( 0xa00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "222.v1", 0x000000, 0x400000, CRC(7d6ba95f) SHA1(03cb4e0d770e0b332b07b64cacef624460b84c78) )
	ROM_LOAD( "222.v2", 0x400000, 0x400000, CRC(6c33bb5d) SHA1(fd5d4e08a962dd0d22c52c91bad5ec7f23cfb901) )
	ROM_LOAD( "222.v3", 0x800000, 0x200000, CRC(831ea8c0) SHA1(f2987b7d09bdc4311e972ce8a9ab7ca9802db4db) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "222.c1", 0x0000000, 0x400000, CRC(68f2ed95) SHA1(c0a02df012cd25bcfe341770ea861a80294148cb) )
	ROM_LOAD16_BYTE( "222.c2", 0x0000001, 0x400000, CRC(a6e9aff0) SHA1(15addca49951ed53fa3c000c8d7cd327d012a620) )
	ROM_LOAD16_BYTE( "222.c3", 0x0800000, 0x400000, CRC(c91b40f4) SHA1(dcda45e0336204e3e024de08edfd0a3217bc1fdd) )
	ROM_LOAD16_BYTE( "222.c4", 0x0800001, 0x400000, CRC(359510a4) SHA1(b6642677ebdff7788263266402080272b8a66b15) )
	ROM_LOAD16_BYTE( "222.c5", 0x1000000, 0x400000, CRC(9cfbb22d) SHA1(789c32f917d0c6e38601cd390a7bf9d803131a4a) )
	ROM_LOAD16_BYTE( "222.c6", 0x1000001, 0x400000, CRC(685efc32) SHA1(db21ba1c7e3631ce0f1cb6f503ae7e0e043ff71b) )
	ROM_LOAD16_BYTE( "222.c7", 0x1800000, 0x400000, CRC(d0f86f0d) SHA1(32502d71c2ab1469c492b6b382bf2bb3f85981d9) )
	ROM_LOAD16_BYTE( "222.c8", 0x1800001, 0x400000, CRC(adfc50e3) SHA1(7d7ee874355b5aa75ad9c9a5c9c3df98d098d85e) )
ROM_END

 /*****************
 Samurai Shodown V
********************/

ROM_START( samsho5hack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270_hack01.p1",  0x000000, 0x400000, CRC(21ef1871) SHA1(5ae2e475fa2f88e5a23d373e7c7d15ebdcc60149) )
	ROM_LOAD16_WORD_SWAP( "270.p2", 0x400000, 0x400000, CRC(e0c74c85) SHA1(DF24A4EE76438E40C2F04A714175A7F85CACDFE0) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270_hack01.c1", 0x0000000, 0x800000, CRC(588d2a7c) SHA1(47c5b184d647d0a8de90fd9823e9073bb7733ea9) )
	ROM_LOAD16_BYTE( "270_hack01.c2", 0x0000001, 0x800000, CRC(0512aa87) SHA1(8e43c08e7c6c8bc7c83b5133c1d0b12b2ded0e79) )
	ROM_LOAD16_BYTE( "270_hack01.c3", 0x1000000, 0x800000, CRC(28cacb61) SHA1(8b247465ab10008b80f73113140eb0d2c6b6397c) )
	ROM_LOAD16_BYTE( "270_hack01.c4", 0x1000001, 0x800000, CRC(A384679f) SHA1(66e7c4193df16d7d334bee6bd61b38db88a98ad9) )
	ROM_LOAD16_BYTE( "270_hack01.c5", 0x2000000, 0x800000, CRC(D6757760) SHA1(D884d0f219750693ae92fe50c91afab147bdf74e) )
	ROM_LOAD16_BYTE( "270_hack01.c6", 0x2000001, 0x800000, CRC(9c489553) SHA1(2a1784c86502485e6a75f356f13a571b7fa2d334) )
	ROM_LOAD16_BYTE( "270_hack01.c7", 0x3000000, 0x800000, CRC(B4f1386d) SHA1(25ec93ef082a16f9844e472013c0ca2980435038) )
	ROM_LOAD16_BYTE( "270_hack01.c8", 0x3000001, 0x800000, CRC(Fa24e3f2) SHA1(A20e78359062ece2fa27032992928f42b08cccf8) )
ROM_END

ROM_START( samsho5hack02 ) 
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270_hack02.p1",  0x000000, 0x400000, CRC(f5fbff09) SHA1(c77e4c7017650439389fd2760aa099dcd657d148) )
	ROM_LOAD16_WORD_SWAP( "270.p2", 0x400000, 0x400000, CRC(e0c74c85) SHA1(df24a4ee76438e40c2f04a714175a7f85cacdfe0) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270.c1", 0x0000000, 0x800000, CRC(14ffffac) SHA1(2ccebfdd0c7907679ae95bf6eca85b8d322441e2) )
	ROM_LOAD16_BYTE( "270.c2", 0x0000001, 0x800000, CRC(401f7299) SHA1(94e48cdf1682b1250f53c59f3f71d995e928d17b) )
	ROM_LOAD16_BYTE( "270.c3", 0x1000000, 0x800000, CRC(838f0260) SHA1(d5c8d3c6e7221d04e0b20882a847752e5ba95635) )
	ROM_LOAD16_BYTE( "270.c4", 0x1000001, 0x800000, CRC(041560a5) SHA1(d165e533699f15b1e079c82f97db3542b3a7dd66) )
	ROM_LOAD16_BYTE( "270.c5", 0x2000000, 0x800000, CRC(bd30b52d) SHA1(9f8282e684415b4045218cf764ef7d75a70e3240) )
	ROM_LOAD16_BYTE( "270.c6", 0x2000001, 0x800000, CRC(86a69c70) SHA1(526732cdb408cf680af9da39057bce6a4dfb5e13) )
	ROM_LOAD16_BYTE( "270.c7", 0x3000000, 0x800000, CRC(d28fbc3c) SHA1(a82a6ba6760fad14d9309f9147cb7d80bd6f70fc) )
	ROM_LOAD16_BYTE( "270.c8", 0x3000001, 0x800000, CRC(02c530a6) SHA1(7a3fafa6075506c6ef78cc4ec2cb72118ec83cb9) )
ROM_END

ROM_START( samsho5hack03 ) 
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270_hack03.p1",  0x000000, 0x400000, CRC(8c00c038) SHA1(a30976eae9b587348ffdb860e6d52779cc4ca9c6) )
	ROM_LOAD16_WORD_SWAP( "270.p2", 0x400000, 0x400000, CRC(e0c74c85) SHA1(df24a4ee76438e40c2f04a714175a7f85cacdfe0) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270.c1", 0x0000000, 0x800000, CRC(14ffffac) SHA1(2ccebfdd0c7907679ae95bf6eca85b8d322441e2) )
	ROM_LOAD16_BYTE( "270.c2", 0x0000001, 0x800000, CRC(401f7299) SHA1(94e48cdf1682b1250f53c59f3f71d995e928d17b) )
	ROM_LOAD16_BYTE( "270.c3", 0x1000000, 0x800000, CRC(838f0260) SHA1(d5c8d3c6e7221d04e0b20882a847752e5ba95635) )
	ROM_LOAD16_BYTE( "270.c4", 0x1000001, 0x800000, CRC(041560a5) SHA1(d165e533699f15b1e079c82f97db3542b3a7dd66) )
	ROM_LOAD16_BYTE( "270.c5", 0x2000000, 0x800000, CRC(bd30b52d) SHA1(9f8282e684415b4045218cf764ef7d75a70e3240) )
	ROM_LOAD16_BYTE( "270.c6", 0x2000001, 0x800000, CRC(86a69c70) SHA1(526732cdb408cf680af9da39057bce6a4dfb5e13) )
	ROM_LOAD16_BYTE( "270.c7", 0x3000000, 0x800000, CRC(d28fbc3c) SHA1(a82a6ba6760fad14d9309f9147cb7d80bd6f70fc) )
	ROM_LOAD16_BYTE( "270.c8", 0x3000001, 0x800000, CRC(02c530a6) SHA1(7a3fafa6075506c6ef78cc4ec2cb72118ec83cb9) )
ROM_END

ROM_START( samsho5hack04 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270_hack04.p1",  0x000000, 0x400000, CRC(3fadf263) SHA1(2f40957c110565f399b82a38fa348871ab4e31ec) )
	ROM_LOAD16_WORD_SWAP( "270_hack04.p2",  0x400000, 0x400000, CRC(F34f3e85) SHA1(C6180516dd85c2a5de22f34108d320c0d5431b1b) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270_hack04.c1", 0x0000000, 0x800000, CRC(1216d51d) SHA1(59c896e8d71921cf8ff3586dc87abc3453ba0f52) )
	ROM_LOAD16_BYTE( "270_hack04.c2", 0x0000001, 0x800000, CRC(9f60d7f6) SHA1(Ae8abc9e418246a2ee55bb974eee7b3420e8d532) )
	ROM_LOAD16_BYTE( "270_hack04.c3", 0x1000000, 0x800000, CRC(3d6490cb) SHA1(95920c195406a25af7fea4844a43294818a6a983) )
	ROM_LOAD16_BYTE( "270_hack04.c4", 0x1000001, 0x800000, CRC(9f716b99) SHA1(1010660b20223e4376be05ec03bca95efbd538cc) )
	ROM_LOAD16_BYTE( "270_hack04.c5", 0x2000000, 0x800000, CRC(8a41f282) SHA1(8d7dae18085c18d120f59b2d8c1a7cf6271b8c90) )
	ROM_LOAD16_BYTE( "270_hack04.c6", 0x2000001, 0x800000, CRC(58e5e466) SHA1(841638c5df9a39ccdec4e04e51fb4ffbc9f269bd) )
	ROM_LOAD16_BYTE( "270_hack04.c7", 0x3000000, 0x800000, CRC(Ef0f7f6d) SHA1(F1eaa3cb7100a19d5b04a81c1343affb9431e589) )
	ROM_LOAD16_BYTE( "270_hack04.c8", 0x3000001, 0x800000, CRC(2b6b690d) SHA1(75bd30b3a1e105d799225838188f34c16388c1d7) )
ROM_END

ROM_START( samsho5hack05 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270_hack05.p1", 0x000000, 0x800000, CRC(16983af9) SHA1(8b1ab272200bede14ad66d7e98474ac3fb7564d3) )

	NEO_SFIX_128K( "270cd.s1", CRC(2ad6048b) SHA1(173c8561f0633e0d6e4a9750e632f2e46766e674) )

	NEO_BIOS_AUDIO_512K( "270_hack05.m1", CRC(5218a10a) SHA1(da4968e55d4a56249d85d6742a6acffcff4ad65d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270cd.v1", 0x000000, 0x800000, CRC(809c7617) SHA1(b6b542eb4c1078016fcd78d050433dc8e3029329) )
	ROM_LOAD( "270cd.v2", 0x800000, 0x800000, CRC(42671607) SHA1(4fa64b5a9349e9c3d5d60806c9da4a2b6beff18a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270_hack05.c1", 0x0000000, 0x800000, CRC(25272e50) SHA1(cc2aa5e5de2483539398f27f317ab2dda7b5c49e) )
	ROM_LOAD16_BYTE( "270_hack05.c2", 0x0000001, 0x800000, CRC(ba68f2e7) SHA1(77b9b5ddc3b313df0dd328c75eac0e8bdfcebad8) )
	ROM_LOAD16_BYTE( "270_hack05.c3", 0x1000000, 0x800000, CRC(75883cde) SHA1(2f9d72a07f8f14b52150cfb70bbc33d171a3d515) )
	ROM_LOAD16_BYTE( "270_hack05.c4", 0x1000001, 0x800000, CRC(348540e6) SHA1(4c6eec8948fbb67ae57eaa4a98525deed6cd2859) )
	ROM_LOAD16_BYTE( "270_hack05.c5", 0x2000000, 0x800000, CRC(1fee8dc8) SHA1(c7f1016ad711ec248218627c92dd5af68cba0206) )
	ROM_LOAD16_BYTE( "270_hack05.c6", 0x2000001, 0x800000, CRC(c300b50d) SHA1(8c8fbb81dbc6c283b55380d577944c6b9cdd6cf4) )
	ROM_LOAD16_BYTE( "270_hack05.c7", 0x3000000, 0x800000, CRC(5e722b0b) SHA1(7015dac7cc234473949912216a1b82ca640b52b6) )
	ROM_LOAD16_BYTE( "270_hack05.c8", 0x3000001, 0x800000, CRC(e2a2c546) SHA1(c870acb090606ae185fa9d7b4932949abacb0705) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( samsho5bd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270bd.p1", 0x000000, 0x800000, CRC(611a6687) SHA1(55ff9122d64efd276e22a66fa443e0bebd079928) )

	NEO_SFIX_128K( "270bd.s1", CRC(33227d62) SHA1(5f4b7ab642175657af212304783a1df98415ed44) )

	NEO_BIOS_AUDIO_128K( "ssv.m1", CRC(18114fb1) SHA1(016dc2f328340f3637a9bff373a20973df29f6b8) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270bd.v1", 0x000000, 0x400000, CRC(6849136c) SHA1(5f8e7cc33b8d994165dd577a0d7e0eb2df16f07e) )
	ROM_LOAD( "270bd.v2", 0x400000, 0x400000, CRC(222e1774) SHA1(3c1b2f37d4f2fa2cd7d6a6f04b3aedf6b2f4b089) )
	ROM_LOAD( "270bd.v3", 0x800000, 0x400000, CRC(cd9e7adc) SHA1(941ae992c004594885f635fdea3ac1c51bb5d469) )
	ROM_LOAD( "270bd.v4", 0xc00000, 0x400000, CRC(8b305cac) SHA1(938e2600c0c2b3d4a0a362ef6e91850c32217f18) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270bd.c1", 0x0000000, 0x800000, CRC(4e7bdea1) SHA1(2376386e0e82820c0853cf39e27167f8c181980c) )
	ROM_LOAD16_BYTE( "270bd.c2", 0x0000001, 0x800000, CRC(7b444985) SHA1(1f2941f6a1bee6391002b88277edcfa7554f10f3) )
	ROM_LOAD16_BYTE( "270bd.c3", 0x1000000, 0x800000, CRC(8c709a9b) SHA1(92a03ccf08d301955b06f3323417b4f20d773428) )
	ROM_LOAD16_BYTE( "270bd.c4", 0x1000001, 0x800000, CRC(cfd53f5c) SHA1(773b03fb6faa958eddee6b001ebb619f8a8d5b30) )
	ROM_LOAD16_BYTE( "270bd.c5", 0x2000000, 0x800000, CRC(c026d318) SHA1(fec9a9309afe94f21e0f35ec992a39ce51deefc2) )
	ROM_LOAD16_BYTE( "270bd.c6", 0x2000001, 0x800000, CRC(b3d9d204) SHA1(9dbcf6c179ea3a04c8ed26416e4e91324fd1cf11) )
	ROM_LOAD16_BYTE( "270bd.c7", 0x3000000, 0x800000, CRC(fe03a025) SHA1(3a052869f6b46cffc5d1ea497b0d5f1f4d1874c2) )
	ROM_LOAD16_BYTE( "270bd.c8", 0x3000001, 0x800000, CRC(89db2d34) SHA1(cd3f1e4ef631704c1078c915135a4510891bd429) )
ROM_END

ROM_START( samsho5cd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270cd.p1",  0x000000, 0x400000, CRC(88e477cc) SHA1(053e7f88e74f6fca0e9cfa6fbc5a505775e44125) )
	ROM_LOAD16_WORD_SWAP( "270cd.p2", 0x400000, 0x400000, CRC(f01e62aa) SHA1(5e4997f519d4be3fa17868731991a8ebc6e2df71) )

	NEO_SFIX_128K( "270cd.s1", CRC(2ad6048b) SHA1(173c8561f0633e0d6e4a9750e632f2e46766e674) )

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270cd.v1", 0x000000, 0x800000, CRC(809c7617) SHA1(b6b542eb4c1078016fcd78d050433dc8e3029329) )
	ROM_LOAD( "270cd.v2", 0x800000, 0x800000, CRC(42671607) SHA1(4fa64b5a9349e9c3d5d60806c9da4a2b6beff18a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270hd.c1", 0x0000000, 0x800000, CRC(9adec562) SHA1(069d73384dc320c38db420c8b04db86ac0b76101) )
	ROM_LOAD16_BYTE( "270hd.c2", 0x0000001, 0x800000, CRC(ac0309e5) SHA1(9795fb2fa9eea617ec25656cfa9cd507899ec4fe) )
	ROM_LOAD16_BYTE( "270hd.c3", 0x1000000, 0x800000, CRC(82db9dae) SHA1(0206e1cc2c5f0fbbc074a93add5a44f645951d0c) )
	ROM_LOAD16_BYTE( "270hd.c4", 0x1000001, 0x800000, CRC(f8041153) SHA1(55705c725aff949e668c24b23e2e4cdb4ecf78ae) )
	ROM_LOAD16_BYTE( "270hd.c5", 0x2000000, 0x800000, CRC(e689d62d) SHA1(001a59bb1bb14f263d16f8b7e977cd49e44e265a) )
	ROM_LOAD16_BYTE( "270hd.c6", 0x2000001, 0x800000, CRC(a993bdcf) SHA1(d15980afe6d4b33506918186a2efc3f51b2347ca) )
	ROM_LOAD16_BYTE( "270hd.c7", 0x3000000, 0x800000, CRC(707d56a0) SHA1(303ecc621305c8fbeece8d599f9382fe82d191ee) )
	ROM_LOAD16_BYTE( "270hd.c8", 0x3000001, 0x800000, CRC(f5903adc) SHA1(64f867a4e9d23302aa1e884f8fad7872ee347a86) )
ROM_END

ROM_START( samsho5d )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270.p1",  0x000000, 0x400000, CRC(4a2a09e6) SHA1(2644DE02CDAB8CCC605488A7C76B8C9CD1D5BCB9) )
	ROM_LOAD16_WORD_SWAP( "270.p2", 0x400000, 0x400000, CRC(e0c74c85) SHA1(DF24A4EE76438E40C2F04A714175A7F85CACDFE0) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_256K( "270d.m1", CRC(e94a5e2b) SHA1(53ef2ad6583060af69fdde73576e09ba88affa55) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270d.c1", 0x0000000, 0x800000, CRC(726abb40) SHA1(86A08C1266FB057D9A30A3AC259D526EC10D2FFA) )
	ROM_LOAD16_BYTE( "270d.c2", 0x0000001, 0x800000, CRC(76806188) SHA1(B448C06D7311791BAD09F0828FD3E56326DD7737) )
	ROM_LOAD16_BYTE( "270hd.c3", 0x1000000, 0x800000, CRC(82db9dae) SHA1(0206E1CC2C5F0FBBC074A93ADD5A44F645951D0C) )
	ROM_LOAD16_BYTE( "270hd.c4", 0x1000001, 0x800000, CRC(f8041153) SHA1(55705C725AFF949E668C24B23E2E4CDB4ECF78AE) )
	ROM_LOAD16_BYTE( "270d.c5", 0x2000000, 0x800000, CRC(2219186a) SHA1(ACE6D04DA578F2A5E7B7A405E0CBB31073070185) )
	ROM_LOAD16_BYTE( "270d.c6", 0x2000001, 0x800000, CRC(d225120d) SHA1(AE28664486135949DD16BB91E8F35936BDA725A0) )
	ROM_LOAD16_BYTE( "270d.c7", 0x3000000, 0x800000, CRC(d245d493) SHA1(C0E89F863B9B592FF5F0F813D9B127CEE31132D1) )
	ROM_LOAD16_BYTE( "270d.c8", 0x3000001, 0x800000, CRC(eb06c9cc) SHA1(1DFD45407586DC7F574189BBC0D8AE1967D0A089) )
ROM_END

ROM_START( samsho5d1 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270.p1",  0x000000, 0x400000, CRC(4a2a09e6) SHA1(2644de02cdab8ccc605488a7c76b8c9cd1d5bcb9) )
	ROM_LOAD16_WORD_SWAP( "270.p2", 0x400000, 0x400000, CRC(e0c74c85) SHA1(df24a4ee76438e40c2f04a714175a7f85cacdfe0) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270hd.c1", 0x0000000, 0x800000, CRC(9adec562) SHA1(069d73384dc320c38db420c8b04db86ac0b76101) )
	ROM_LOAD16_BYTE( "270hd.c2", 0x0000001, 0x800000, CRC(ac0309e5) SHA1(9795fb2fa9eea617ec25656cfa9cd507899ec4fe) )
	ROM_LOAD16_BYTE( "270hd.c3", 0x1000000, 0x800000, CRC(82db9dae) SHA1(0206e1cc2c5f0fbbc074a93add5a44f645951d0c) )
	ROM_LOAD16_BYTE( "270hd.c4", 0x1000001, 0x800000, CRC(f8041153) SHA1(55705c725aff949e668c24b23e2e4cdb4ecf78ae) )
	ROM_LOAD16_BYTE( "270hd.c5", 0x2000000, 0x800000, CRC(e689d62d) SHA1(001a59bb1bb14f263d16f8b7e977cd49e44e265a) )
	ROM_LOAD16_BYTE( "270hd.c6", 0x2000001, 0x800000, CRC(a993bdcf) SHA1(d15980afe6d4b33506918186a2efc3f51b2347ca) )
	ROM_LOAD16_BYTE( "270hd.c7", 0x3000000, 0x800000, CRC(707d56a0) SHA1(303ecc621305c8fbeece8d599f9382fe82d191ee) )
	ROM_LOAD16_BYTE( "270hd.c8", 0x3000001, 0x800000, CRC(f5903adc) SHA1(64f867a4e9d23302aa1e884f8fad7872ee347a86) )
ROM_END

ROM_START( samsho5hd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "270hd.p1", 0x000000, 0x400000, CRC(bf956089) SHA1(c538289069bf338b9fa7ecc5c9143763dbb776a8) )
	ROM_LOAD16_WORD_SWAP( "270hd.p2", 0x400000, 0x400000, CRC(943a6b1d) SHA1(12bd02fc197456da6ee86f066086094cef0f4bf9) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "270.m1", CRC(49c9901a) SHA1(2623e9765a0eba58fee2de72851e9dc502344a3d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "270.v1", 0x000000, 0x800000, CRC(62e434eb) SHA1(1985f5e88f8e866f9683b6cea901aa28c04b80bf) )
	ROM_LOAD( "270.v2", 0x800000, 0x800000, CRC(180f3c9a) SHA1(6d7dc2605ead6e78704efa127e7e0dfe621e2c54) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "270hd.c1", 0x0000000, 0x800000, CRC(9adec562) SHA1(069d73384dc320c38db420c8b04db86ac0b76101) )
	ROM_LOAD16_BYTE( "270hd.c2", 0x0000001, 0x800000, CRC(ac0309e5) SHA1(9795fb2fa9eea617ec25656cfa9cd507899ec4fe) )
	ROM_LOAD16_BYTE( "270hd.c3", 0x1000000, 0x800000, CRC(82db9dae) SHA1(0206e1cc2c5f0fbbc074a93add5a44f645951d0c) )
	ROM_LOAD16_BYTE( "270hd.c4", 0x1000001, 0x800000, CRC(f8041153) SHA1(55705c725aff949e668c24b23e2e4cdb4ecf78ae) )
	ROM_LOAD16_BYTE( "270hd.c5", 0x2000000, 0x800000, CRC(e689d62d) SHA1(001a59bb1bb14f263d16f8b7e977cd49e44e265a) )
	ROM_LOAD16_BYTE( "270hd.c6", 0x2000001, 0x800000, CRC(a993bdcf) SHA1(d15980afe6d4b33506918186a2efc3f51b2347ca) )
	ROM_LOAD16_BYTE( "270hd.c7", 0x3000000, 0x800000, CRC(707d56a0) SHA1(303ecc621305c8fbeece8d599f9382fe82d191ee) )
	ROM_LOAD16_BYTE( "270hd.c8", 0x3000001, 0x800000, CRC(f5903adc) SHA1(64f867a4e9d23302aa1e884f8fad7872ee347a86) )
ROM_END

 /*************************
 Samurai Shodown V Special
****************************/

ROM_START( samsh5sphack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272_hack01.p1",  0x000000, 0x400000, CRC(03099674) SHA1(06a9f3437414b47a7dff68885160afd0a11746a7) )
	ROM_LOAD16_WORD_SWAP( "272.p2", 0x400000, 0x400000, CRC(63492ea6) SHA1(6ba946acb62c63ed61a42fe72b7fff3828883bcc) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "272d.m1", CRC(203d744e) SHA1(24fc73943009effa14eed0f7a29955f349ca8e8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5sphack02 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272sp_hack02.p1",  0x000000, 0x400000, CRC(caff2e5c) SHA1(76106dae850a791afad7d23f0b6f86d7aa463254) )
	ROM_LOAD16_WORD_SWAP( "272.p2", 0x400000, 0x400000, CRC(63492ea6) SHA1(6ba946acb62c63ed61a42fe72b7fff3828883bcc) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "272d.m1", CRC(203d744e) SHA1(24fc73943009effa14eed0f7a29955f349ca8e8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5sphack03 ) //samsh5sphol
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272sp_hack03.p1", 0x000000, 0x400000, CRC(1401fa27) SHA1(90e7cce90361f53e3108f7e3ca4b55d8f37b8dcb) )
	ROM_LOAD16_WORD_SWAP( "272hd.p2", 0x400000, 0x400000, CRC(fa1a7dd8) SHA1(62443dad76d6c1e18f515d7d4ef8e1295a4b7f1d) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "272.m1", CRC(adeebf40) SHA1(8cbd63dda3fff4de38060405bf70cd9308c9e66e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5sphack04 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272sp_hack04.p1", 0x000000, 0x400000, CRC(6e20b19a) SHA1(906481e91da79564be9aa01c481acf28cbb69607) )
	ROM_LOAD16_WORD_SWAP( "272sp_hack04.p2", 0x400000, 0x400000, CRC(1f5469bc) SHA1(31d031c2b1268c4895c6679ce38a46b5c5b829d1) )
	ROM_FILL(0x10a80,1,0x60)   // Patch to enable story when fighters selected [by Ack]
	ROM_FILL(0x10a81,1,0x08)
	ROM_FILL(0x068c8,1,0x4e)
	ROM_FILL(0x068c9,1,0xf9)
	ROM_FILL(0x068ca,1,0x00)
	ROM_FILL(0x068cb,1,0x09)
	ROM_FILL(0x068cc,1,0xdd)
	ROM_FILL(0x068cd,1,0xd0)
	ROM_FILL(0x068ce,1,0x4e)
	ROM_FILL(0x068cf,1,0x71)
	ROM_FILL(0x9ddd0,1,0x48)
	ROM_FILL(0x9ddd1,1,0xe7)
	ROM_FILL(0x9ddd2,1,0xe0)
	ROM_FILL(0x9ddd3,1,0xc0)
	ROM_FILL(0x9ddd4,1,0x4a)
	ROM_FILL(0x9ddd5,1,0x2d)
	ROM_FILL(0x9ddd6,1,0x7d)
	ROM_FILL(0x9ddd7,1,0x83)
	ROM_FILL(0x9ddd8,1,0x67)
	ROM_FILL(0x9ddd9,1,0x1e)
	ROM_FILL(0x9ddda,1,0x43)
	ROM_FILL(0x9dddb,1,0xfa)
	ROM_FILL(0x9dddc,1,0x02)
	ROM_FILL(0x9dddd,1,0x24)
	ROM_FILL(0x9ddde,1,0x20)
	ROM_FILL(0x9dddf,1,0x19)
	ROM_FILL(0x9dde0,1,0x58)
	ROM_FILL(0x9dde1,1,0x89)
	ROM_FILL(0x9dde2,1,0x53)
	ROM_FILL(0x9dde3,1,0x40)
	ROM_FILL(0x9dde4,1,0x20)
	ROM_FILL(0x9dde5,1,0x19)
	ROM_FILL(0x9dde6,1,0xb1)
	ROM_FILL(0x9dde7,1,0xd9)
	ROM_FILL(0x9dde8,1,0x67)
	ROM_FILL(0x9dde9,1,0x06)
	ROM_FILL(0x9ddea,1,0x51)
	ROM_FILL(0x9ddeb,1,0xc8)
	ROM_FILL(0x9ddec,1,0xff)
	ROM_FILL(0x9dded,1,0xf8)
	ROM_FILL(0x9ddee,1,0x60)
	ROM_FILL(0x9ddef,1,0x08)
	ROM_FILL(0x9ddf0,1,0xd0)
	ROM_FILL(0x9ddf1,1,0xbc)
	ROM_FILL(0x9ddf2,1,0x00)
	ROM_FILL(0x9ddf3,1,0x09)
	ROM_FILL(0x9ddf4,1,0xe0)
	ROM_FILL(0x9ddf5,1,0x00)
	ROM_FILL(0x9ddf6,1,0x20)
	ROM_FILL(0x9ddf7,1,0x40)
	ROM_FILL(0x9ddf8,1,0x43)
	ROM_FILL(0x9ddf9,1,0xed)
	ROM_FILL(0x9ddfa,1,0x18)
	ROM_FILL(0x9ddfb,1,0x8e)
	ROM_FILL(0x9ddfc,1,0x4e)
	ROM_FILL(0x9ddfd,1,0xf8)
	ROM_FILL(0x9ddfe,1,0x68)
	ROM_FILL(0x9ddff,1,0xd0)

	NEO_SFIX_128K( "272n.s1", CRC(C297F973) SHA1(B31AF6B51E7536B538CD7EB3542C631C6327E826) )

	NEO_BIOS_AUDIO_512K( "272d.m1", CRC(203d744e) SHA1(24fc73943009effa14eed0f7a29955f349ca8e8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272sp_hack04.v1", 0x000000, 0x800000, CRC(3bf61586) SHA1(270fd58781b2b897a7365ccdf878c7e57048da35) )
	ROM_LOAD( "272sp_hack04.v2", 0x800000, 0x800000, CRC(95fe7646) SHA1(eec1a3a4dc5b5a960735147c29b976581a660628) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272sp_hack04.c1", 0x0000000, 0x800000, CRC(ec9fda8d) SHA1(acc307e864d0ba15beb78a42edfa62941394ab9f) )
	ROM_LOAD16_BYTE( "272sp_hack04.c2", 0x0000001, 0x800000, CRC(d2fc888d) SHA1(cffa2aed9581ac42db0672a25ae737885f4c2044) )
	ROM_LOAD16_BYTE( "272sp_hack04.c3", 0x1000000, 0x800000, CRC(b0ea781b) SHA1(6e336952df4ec2d203a335e024ca85b912f7fa38) )
	ROM_LOAD16_BYTE( "272sp_hack04.c4", 0x1000001, 0x800000, CRC(d34ac591) SHA1(dc429100fc26baaca70f6dbf9d1c4d837b85935e) )
	ROM_LOAD16_BYTE( "272sp_hack04.c5", 0x2000000, 0x800000, CRC(1b5c1ea2) SHA1(7741344a640945770b9c97ccb3e30155874de9e8) )
	ROM_LOAD16_BYTE( "272sp_hack04.c6", 0x2000001, 0x800000, CRC(deeaad58) SHA1(e718fb195af55c2df8a101cbb6b5a050c4cb14e3) )
	ROM_LOAD16_BYTE( "272sp_hack04.c7", 0x3000000, 0x800000, CRC(dce0119f) SHA1(a89adc624fc549d69222df9202bdf277ccdbf735) )
	ROM_LOAD16_BYTE( "272sp_hack04.c8", 0x3000001, 0x800000, CRC(3bf256c5) SHA1(7b24d2b78a8937ad1aed1b68c134bc48b60bfbba) )
ROM_END

ROM_START( samsh5sphack05 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272sp_hack05.p1", 0x000000, 0x800000, CRC(e3d18d3a) SHA1(25beacc2354bb8eff5c08900b954de877d3c4776) )

	NEO_SFIX_128K( "272sp_hack05.s1", CRC(abb3baf9) SHA1(dc8072e8757dc525d9285b77a56fdedaf4d9330d) )

	NEO_BIOS_AUDIO_128K( "272sp_hack05.m1", CRC(654e9236) SHA1(e13a1b4b73d43008565ee419c9714a220927d0bc) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272sp_hack04.v1", 0x000000, 0x800000, CRC(3bf61586) SHA1(270fd58781b2b897a7365ccdf878c7e57048da35) )
	ROM_LOAD( "272sp_hack04.v2", 0x800000, 0x800000, CRC(95fe7646) SHA1(eec1a3a4dc5b5a960735147c29b976581a660628) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272sp_hack04.c1", 0x0000000, 0x800000, CRC(ec9fda8d) SHA1(acc307e864d0ba15beb78a42edfa62941394ab9f) )
	ROM_LOAD16_BYTE( "272sp_hack04.c2", 0x0000001, 0x800000, CRC(d2fc888d) SHA1(cffa2aed9581ac42db0672a25ae737885f4c2044) )
	ROM_LOAD16_BYTE( "272sp_hack04.c3", 0x1000000, 0x800000, CRC(b0ea781b) SHA1(6e336952df4ec2d203a335e024ca85b912f7fa38) )
	ROM_LOAD16_BYTE( "272sp_hack04.c4", 0x1000001, 0x800000, CRC(d34ac591) SHA1(dc429100fc26baaca70f6dbf9d1c4d837b85935e) )
	ROM_LOAD16_BYTE( "272sp_hack04.c5", 0x2000000, 0x800000, CRC(1b5c1ea2) SHA1(7741344a640945770b9c97ccb3e30155874de9e8) )
	ROM_LOAD16_BYTE( "272sp_hack04.c6", 0x2000001, 0x800000, CRC(deeaad58) SHA1(e718fb195af55c2df8a101cbb6b5a050c4cb14e3) )
	ROM_LOAD16_BYTE( "272sp_hack05.c7", 0x3000000, 0x800000, CRC(af90afc8) SHA1(96957010046ed4493e18fc45582ce7a494b8f2b2) )
	ROM_LOAD16_BYTE( "272sp_hack05.c8", 0x3000001, 0x800000, CRC(b14872da) SHA1(b043094f29305cdc8669feaf096ef4451778a408) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( samsh5spn ) // fully decrypted
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272n.p1", 0x000000, 0x800000, CRC(339f5081) SHA1(3e120481a5aee6a978d87a0f600a7323657d1c51) )

	NEO_SFIX_128K( "272n.s1", CRC(c297f973) SHA1(b31af6b51e7536b538cd7eb3542c631c6327e826) )

	NEO_BIOS_AUDIO_128K( "272n.m1", CRC(0b5d4d6a) SHA1(2467e06be986cba8c879412cba0388019892dbb2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272n.v1", 0x000000, 0x400000, CRC(4d1b252b) SHA1(e5efe408c337a2ca556e790f0fd917ba44f975e3) )
	ROM_LOAD( "272n.v2", 0x400000, 0x400000, CRC(0e46d2f8) SHA1(b8b0838124ea29f73f31e08d5cced8797cea75b1) )
	ROM_LOAD( "272n.v3", 0x800000, 0x400000, CRC(3f0f7554) SHA1(2581e297725f3b950b59e9086ac5569171da6140) )
	ROM_LOAD( "272n.v4", 0xc00000, 0x400000, CRC(ad8fabb4) SHA1(cebe3b7306e1431ebb99df13bfc2270b1d3ba1ff) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5spd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272.p1",  0x000000, 0x400000, CRC(fb7a6bba) SHA1(f68c527208d8a55ca44b0caaa8ab66b3a0ffdfe5) )
	ROM_LOAD16_WORD_SWAP( "272.p2", 0x400000, 0x400000, CRC(63492ea6) SHA1(6ba946acb62c63ed61a42fe72b7fff3828883bcc) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "272d.m1", CRC(203d744e) SHA1(24fc73943009effa14eed0f7a29955f349ca8e8f) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5spd1 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272.p1",  0x000000, 0x400000, CRC(fb7a6bba) SHA1(f68c527208d8a55ca44b0caaa8ab66b3a0ffdfe5) )
	ROM_LOAD16_WORD_SWAP( "272.p2", 0x400000, 0x400000, CRC(63492ea6) SHA1(6ba946acb62c63ed61a42fe72b7fff3828883bcc) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "272.m1", CRC(adeebf40) SHA1(8cbd63dda3fff4de38060405bf70cd9308c9e66e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5spd2 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272d2.p1", 0x000000, 0x400000, CRC(D190102F) SHA1(A853395575E135C2113FCCA942651A2A6193B504) )
	ROM_LOAD16_WORD_SWAP( "272d2.p2", 0x400000, 0x400000, CRC(DA770F06) SHA1(F23DE7385AE5C6A27D70B1140654192F0ED2DF84) )

	NEO_SFIX_128K( "272n.s1", CRC(C297F973) SHA1(B31AF6B51E7536B538CD7EB3542C631C6327E826) )

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "272.m1", CRC(adeebf40) SHA1(8cbd63dda3fff4de38060405bf70cd9308c9e66e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5sphd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272hd.p1", 0x000000, 0x400000, CRC(9291794d) SHA1(66588ff9b00ffad6508b03423548984e28a3209d) )
	ROM_LOAD16_WORD_SWAP( "272hd.p2", 0x400000, 0x400000, CRC(fa1a7dd8) SHA1(62443dad76d6c1e18f515d7d4ef8e1295a4b7f1d) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "272.m1", CRC(adeebf40) SHA1(8cbd63dda3fff4de38060405bf70cd9308c9e66e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

ROM_START( samsh5spnd ) /* 2nd bugfix release */
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "272nd.p1",  0x000000, 0x400000, CRC(c30a08dd) SHA1(66864954017c841d7ca8490112c3aa7a71a4da70) )
	ROM_LOAD16_WORD_SWAP( "272nd.sp2", 0x400000, 0x400000, CRC(bd64a518) SHA1(aa259a168930f106377d680db444535411b3bce0) )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "272.m1", CRC(adeebf40) SHA1(8cbd63dda3fff4de38060405bf70cd9308c9e66e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "272.v1", 0x000000, 0x800000, CRC(76a94127) SHA1(c3affd7ff1eb02345cfb755962ec173a8ec34acd) )
	ROM_LOAD( "272.v2", 0x800000, 0x800000, CRC(4ba507f1) SHA1(728d139da3fe8a391fd8be4d24bb7fdd4bf9548a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "272d.c1", 0x0000000, 0x800000, CRC(8548097e) SHA1(99177F64039D2914FA4EBD4AFC7EB0FBF4FFE0AF) )
	ROM_LOAD16_BYTE( "272d.c2", 0x0000001, 0x800000, CRC(8c1b48d0) SHA1(3F20288ED369E6276CFAEDFF7E9703D4EA1CF6F6) )
	ROM_LOAD16_BYTE( "272d.c3", 0x1000000, 0x800000, CRC(96ddb28c) SHA1(7EBE5166D279C0D3EFEF65618D015B4EA13F2C49) )
	ROM_LOAD16_BYTE( "272d.c4", 0x1000001, 0x800000, CRC(99ef7a0a) SHA1(9AC3EF339BC1D6E3482E0B32D40B259EC2B44FC1) )
	ROM_LOAD16_BYTE( "272d.c5", 0x2000000, 0x800000, CRC(772e8b1e) SHA1(8DE4898BBD8EDE593763BFA93BBF26492492D23A) )
	ROM_LOAD16_BYTE( "272d.c6", 0x2000001, 0x800000, CRC(5fff21fc) SHA1(5D0A23A41F5277AB09FD11F227BE35F1713A4B4F) )
	ROM_LOAD16_BYTE( "272d.c7", 0x3000000, 0x800000, CRC(9ac56a0e) SHA1(EA4FFEC3FE2E58DF16B40FDF33E1708680010414) )
	ROM_LOAD16_BYTE( "272d.c8", 0x3000001, 0x800000, CRC(cfde7aff) SHA1(25185F471DD35E5D388B75B83D8652085CD5E17D) )
ROM_END

 /************
 Savage Reign
***************/

ROM_START( savagerehack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "059_hack01.p1", 0x100000, 0x100000, CRC(e892979a) SHA1(f8bc776a18b25275541f0128ad621aa881475303) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "059.s1", CRC(e08978ca) SHA1(55152cb9bd0403ae8656b93a6b1522dba5db6d1a) )

	NEO_BIOS_AUDIO_128K( "059.m1", CRC(29992eba) SHA1(187be624abe8670503edb235ff21ae8fdc3866e0) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "059.v1", 0x000000, 0x200000, CRC(530c50fd) SHA1(29401cee7f7d2c199c7cb58092e86b28205e81ad) )
	ROM_LOAD( "059.v2", 0x200000, 0x200000, CRC(eb6f1cdb) SHA1(7a311388315ea543babf872f62219fdc4d39d013) )
	ROM_LOAD( "059.v3", 0x400000, 0x200000, CRC(7038c2f9) SHA1(c1d6f86b24feba03fe009b58199d2eeabe572f4e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "059.c1", 0x000000, 0x200000, CRC(763ba611) SHA1(d3262e0332c894ee149c5963f882cc5e5562ee57) )
	ROM_LOAD16_BYTE( "059.c2", 0x000001, 0x200000, CRC(e05e8ca6) SHA1(986a9b16ff92bc101ab567d2d01348e093abea9a) )
	ROM_LOAD16_BYTE( "059.c3", 0x400000, 0x200000, CRC(3e4eba4b) SHA1(770adec719e63a30ebe9522cc7576caaca44f3b2) )
	ROM_LOAD16_BYTE( "059.c4", 0x400001, 0x200000, CRC(3c2a3808) SHA1(698adcec0715c9e78b6286be38debf0ce28fd644) )
	ROM_LOAD16_BYTE( "059.c5", 0x800000, 0x200000, CRC(59013f9e) SHA1(5bf48fcc450da72a8c4685f6e3887e67eae49988) )
	ROM_LOAD16_BYTE( "059.c6", 0x800001, 0x200000, CRC(1c8d5def) SHA1(475d89a5c4922a9f6bd756d23c2624d57b6e9d62) )
	ROM_LOAD16_BYTE( "059.c7", 0xc00000, 0x200000, CRC(c88f7035) SHA1(c29a428b741f4fe7b71a3bc23c87925b6bc1ca8f) )
	ROM_LOAD16_BYTE( "059.c8", 0xc00001, 0x200000, CRC(484ce3ba) SHA1(4f21ed20ce6e2b67e2b079404599310c94f591ff) )
ROM_END

ROM_START( savagerehack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "059_hack02.p1", 0x100000, 0x100000, CRC(73fe4838) SHA1(18638f7110069aa993c000dd2698dac0ac6061d4) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "059.s1", CRC(e08978ca) SHA1(55152cb9bd0403ae8656b93a6b1522dba5db6d1a) )

	NEO_BIOS_AUDIO_128K( "059.m1", CRC(29992eba) SHA1(187be624abe8670503edb235ff21ae8fdc3866e0) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "059.v1", 0x000000, 0x200000, CRC(530c50fd) SHA1(29401cee7f7d2c199c7cb58092e86b28205e81ad) )
	ROM_LOAD( "059.v2", 0x200000, 0x200000, CRC(eb6f1cdb) SHA1(7a311388315ea543babf872f62219fdc4d39d013) )
	ROM_LOAD( "059.v3", 0x400000, 0x200000, CRC(7038c2f9) SHA1(c1d6f86b24feba03fe009b58199d2eeabe572f4e) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "059.c1", 0x000000, 0x200000, CRC(763ba611) SHA1(d3262e0332c894ee149c5963f882cc5e5562ee57) )
	ROM_LOAD16_BYTE( "059.c2", 0x000001, 0x200000, CRC(e05e8ca6) SHA1(986a9b16ff92bc101ab567d2d01348e093abea9a) )
	ROM_LOAD16_BYTE( "059.c3", 0x400000, 0x200000, CRC(3e4eba4b) SHA1(770adec719e63a30ebe9522cc7576caaca44f3b2) )
	ROM_LOAD16_BYTE( "059.c4", 0x400001, 0x200000, CRC(3c2a3808) SHA1(698adcec0715c9e78b6286be38debf0ce28fd644) )
	ROM_LOAD16_BYTE( "059.c5", 0x800000, 0x200000, CRC(59013f9e) SHA1(5bf48fcc450da72a8c4685f6e3887e67eae49988) )
	ROM_LOAD16_BYTE( "059.c6", 0x800001, 0x200000, CRC(1c8d5def) SHA1(475d89a5c4922a9f6bd756d23c2624d57b6e9d62) )
	ROM_LOAD16_BYTE( "059.c7", 0xc00000, 0x200000, CRC(c88f7035) SHA1(c29a428b741f4fe7b71a3bc23c87925b6bc1ca8f) )
	ROM_LOAD16_BYTE( "059.c8", 0xc00001, 0x200000, CRC(484ce3ba) SHA1(4f21ed20ce6e2b67e2b079404599310c94f591ff) )
ROM_END

/*****************
 Super Dodge Ball
*******************/

ROM_START( sdodgebhac01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "208_hac01.p1", 0x100000, 0x100000, CRC(e6e58566) SHA1(81197d0e02fc37cae0fa071e0343f8bdc53878ac) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "208.s1", CRC(64abd6b3) SHA1(0315d724e4d83a44ce84c531ff9b8c398363c039) )

	NEO_BIOS_AUDIO_128K( "208.m1", CRC(0a5f3325) SHA1(04e0236df478a5452654c823dcb42fea65b6a718) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "208.v1", 0x000000, 0x400000, CRC(e7899a24) SHA1(3e75b449898fee73fbacf58d70e3a460b9e0c573) )

	ROM_REGION( 0x0c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "208.c1", 0x0000000, 0x400000, CRC(93d8619b) SHA1(6588cb67e38722d5843fb29943d92e3905101aff) )
	ROM_LOAD16_BYTE( "208.c2", 0x0000001, 0x400000, CRC(1c737bb6) SHA1(8e341989981a713e61dfed8bde9a6459583ef46d) )
	ROM_LOAD16_BYTE( "208.c3", 0x0800000, 0x200000, CRC(14cb1703) SHA1(a46acec03c1b2351fe36810628f02b7c848d13db) )
	ROM_LOAD16_BYTE( "208.c4", 0x0800001, 0x200000, CRC(c7165f19) SHA1(221f03de893dca0e5305fa17aa94f96c67713818) )
ROM_END

ROM_START( sengoku3hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack01.p1", 0x100000, 0x100000, CRC(1baf5deb) SHA1(3acacc6dd68a47f40b99d1bcd4f30fcb284fb3ed) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack02 ) //sgk3eb
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack02.p1", 0x100000, 0x100000, CRC(2df82062) SHA1(55048f41b1f4c5987f701a884ffd59bca7ecb3a1) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack03 ) //sengoku3h
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack03.p1", 0x100000, 0x100000, CRC(18a8c9ae) SHA1(40848ae6397caffdea82705a7ccfc79e534865a3) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack04 ) //sengoku3sf
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack04.p1", 0x000000, 0x200000, CRC(76b2a57a) SHA1(07feea211341a5b3effe02f0d4f44345bd007a2a) )

	NEO_SFIX_128K( "261s.s1", CRC(c1e27cc7) SHA1(7d38319f517059f60287a8ce393a4901719db8a9) )

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack05 ) //sengoku3a
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack05.p1", 0x100000, 0x100000, CRC(9a76c011) SHA1(d65d108adec207ce614f34843952b62596504f13) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack06 ) // sgk3eb
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261-ph1_hack06.p1", 0x100000, 0x100000, CRC(a77a0dc7) SHA1(a78885d1d121583c7733a868f6140babc4ba5544) )
	ROM_IGNORE(0x100000)
	ROM_CONTINUE(0x000000, 0x100000)
	ROM_IGNORE(0x154f48)

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack07 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ph1_hack07.p1", 0x100000, 0x100000, CRC(3207586a) SHA1(a5515faf9e9941133c82fe421c7235a8c69b8278) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack08 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "ph1_hack08.p1", 0x100000, 0x100000, CRC(abfda8b0) SHA1(5ab5850325418f9ba68fe701154347dc1343a294) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_512K( "261.m1", CRC(7d501c39) SHA1(8e6bcc428f5ac7532d9c9be7e07ad0821461a080) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261.c1", 0x0000000, 0x800000, CRC(ded84d9c) SHA1(d960523b813d4fae06d716298d4e431a5c77a0c5) )
	ROM_LOAD16_BYTE( "261.c2", 0x0000001, 0x800000, CRC(b8eb4348) SHA1(619d24312549932959481fa58f43f11c048e1ca5) )
	ROM_LOAD16_BYTE( "261.c3", 0x1000000, 0x800000, CRC(84e2034a) SHA1(38ec4ae4b86933a25c9a03799b8cade4b1346401) )
	ROM_LOAD16_BYTE( "261.c4", 0x1000001, 0x800000, CRC(0b45ae53) SHA1(a19fb21408ab633aee8bbf38bf43b5e26766b355) )
ROM_END

ROM_START( sengoku3hack09 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261_hack09.p1", 0x000000, 0x200000, CRC(5a1b5e85) SHA1(239617385b40ae6a86f737195ec6017171157bef) )

	NEO_SFIX_128K( "261s.s1", CRC(c1e27cc7) SHA1(7d38319f517059f60287a8ce393a4901719db8a9) )

	NEO_BIOS_AUDIO_128K( "261d.m1", CRC(36ed9cdd) SHA1(78a7d755e9e9f52255ac6228d9d402fd6a02c126) )

	ROM_REGION( 0x0e00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261d.c1", 0x0000000, 0x800000, CRC(9af7cbca) SHA1(cf2fe3e5265861369f2878140ef33a6715b031aa) )
	ROM_LOAD16_BYTE( "261d.c2", 0x0000001, 0x800000, CRC(2a1f874d) SHA1(26923ec0bedba56f25e6222aa107b45ccb3def7b) )
	ROM_LOAD16_BYTE( "261d.c3", 0x1000000, 0x800000, CRC(5403adb5) SHA1(5614ee892e810dc49758f5985b3dc4f227e01b56) )
	ROM_LOAD16_BYTE( "261d.c4", 0x1000001, 0x800000, CRC(18926df6) SHA1(a4859c9166ef7abec1f3e6af3292051a63a81f93) )
ROM_END

 /*******
 bootleg
*********/

ROM_START( sengoku3d )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "261.p1", 0x100000, 0x100000, CRC(e0d4bc0a) SHA1(8df366097f224771ca6d1aa5c1691cd46776cd12) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_MT_128K

	NEO_BIOS_AUDIO_128K( "261d.m1", CRC(36ed9cdd) SHA1(78a7d755e9e9f52255ac6228d9d402fd6a02c126) )

	ROM_REGION( 0xE00000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "261.v1", 0x000000, 0x400000, CRC(64c30081) SHA1(f9ebd20cf59b72e864b7274c1bdb6d99ecaf4595) )
	ROM_LOAD( "261.v2", 0x400000, 0x400000, CRC(392a9c47) SHA1(7ab90a54089236ca6c3ef1af8e566a8025d38159) )
	ROM_LOAD( "261.v3", 0x800000, 0x400000, CRC(c1a7ebe3) SHA1(1d7bb481451f5ee0457e954bb5210300182c3c9c) )
	ROM_LOAD( "261.v4", 0xc00000, 0x200000, CRC(9000d085) SHA1(11157b355ab4eb6627e9f322ed875332d3d77349) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "261d.c1", 0x0000000, 0x800000, CRC(9af7cbca) SHA1(cf2fe3e5265861369f2878140ef33a6715b031aa) )
	ROM_LOAD16_BYTE( "261d.c2", 0x0000001, 0x800000, CRC(2a1f874d) SHA1(26923ec0bedba56f25e6222aa107b45ccb3def7b) )
	ROM_LOAD16_BYTE( "261d.c3", 0x1000000, 0x800000, CRC(5403adb5) SHA1(5614ee892e810dc49758f5985b3dc4f227e01b56) )
	ROM_LOAD16_BYTE( "261d.c4", 0x1000001, 0x800000, CRC(18926df6) SHA1(a4859c9166ef7abec1f3e6af3292051a63a81f93) )
ROM_END

/***************************
 Shock Troopers - 2nd Squad
******************************/

ROM_START( shocktr2hack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "246_hack01.p1",   0x000000, 0x100000, CRC(6e55a59c) SHA1(092c39ee9c28e4b45940ae2d43dc92922e6a9eba) )
	ROM_LOAD16_WORD_SWAP( "246.p2", 0x100000, 0x400000, CRC(72ea04c3) SHA1(4fb1d22c30f5f3db4637dd92a4d2705c88de399d) )

	NEO_SFIX_128K( "246.s1", CRC(2a360637) SHA1(431b43da5377dd189e51bd93d88d8a24d1b5090a) )

	NEO_BIOS_AUDIO_128K( "246.m1", CRC(d0604ad1) SHA1(fae3cd52a177eadd5f5775ace957cc0f8301e65d) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "246.v1", 0x000000, 0x400000, CRC(16986fc6) SHA1(cff3103dadf2f4390460456a5bd3fb5f28e21f6a) )
	ROM_LOAD( "246.v2", 0x400000, 0x400000, CRC(ada41e83) SHA1(78e37ffaaa5679c8775a3a71f6df7a0d15082bdc) )
	ROM_LOAD( "246.v3", 0x800000, 0x200000, CRC(a05ba5db) SHA1(09d739cad323d918f4196f91b654627fcafd8f4d) )

	ROM_REGION( 0x3000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "246.c1", 0x0000000, 0x800000, CRC(47ac9ec5) SHA1(2d9eea11ba87baa23b18a1a3f607dc137846e807) )
	ROM_LOAD16_BYTE( "246.c2", 0x0000001, 0x800000, CRC(7bcab64f) SHA1(08d0edddd14b53d606e9a7a46aa4fb4e7398e0d0) )
	ROM_LOAD16_BYTE( "246.c3", 0x1000000, 0x800000, CRC(db2f73e8) SHA1(8d0c3473a8b2a4e28fed1b74beb2e025b7e61867) )
	ROM_LOAD16_BYTE( "246.c4", 0x1000001, 0x800000, CRC(5503854e) SHA1(a0f2e7c609cbb2aa43493a39d7dcaeca3d511d26) )
	ROM_LOAD16_BYTE( "246.c5", 0x2000000, 0x800000, CRC(055b3701) SHA1(97f5e92538d1f2e437dcb3f80e56e1230287e8d1) )
	ROM_LOAD16_BYTE( "246.c6", 0x2000001, 0x800000, CRC(7e2caae1) SHA1(d9de14e3e323664a8c5b7f1df1ba9ec7dd0e6a46) )
ROM_END

/****************
 Shock Troopers
******************/

ROM_START( shocktrohack01 )
	ROM_REGION( 0x500000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "238_hack01.p1",   0x000000, 0x100000, CRC(6ee78d00) SHA1(8847879749e55f7b24e149a7e24ec301f7b56929) )
	ROM_LOAD16_WORD_SWAP( "238.p2", 0x100000, 0x400000, CRC(5b4a09c5) SHA1(de04036cba2da4bb2da73d902d1822b82b4f67a9) )

	NEO_SFIX_128K( "238.s1", CRC(1f95cedb) SHA1(adfa74868147fd260481e4c387d254d3b6de83f4) )

	NEO_BIOS_AUDIO_128K( "238.m1", CRC(075b9518) SHA1(ac21b88a860b9572bf24432b4cadcc96d108055d) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "238.v1", 0x000000, 0x400000, CRC(260c0bef) SHA1(9c4f80ce4bb205afed11bb8b8926d20748eb5512) )
	ROM_LOAD( "238.v2", 0x400000, 0x200000, CRC(4ad7d59e) SHA1(bfdf2684f7f38af4e75ad0068ff9463dc2601598) )

	ROM_REGION( 0x2000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "238.c1", 0x0000000, 0x400000, CRC(90c6a181) SHA1(a381bc8449718814ff12b3a4f7fc4d1bb7ea1631) )
	ROM_LOAD16_BYTE( "238.c2", 0x0000001, 0x400000, CRC(888720f0) SHA1(cd4d65df8d3ef0dbcca2b7f3f803f45c457f5beb) )
	ROM_LOAD16_BYTE( "238.c3", 0x0800000, 0x400000, CRC(2c393aa3) SHA1(1cd7cebe5861a2d65f1d6615dd7752162e573a02) )
	ROM_LOAD16_BYTE( "238.c4", 0x0800001, 0x400000, CRC(b9e909eb) SHA1(33cc9b2d13e4ed2ab6040ff582a53dc9bca402e0) )
	ROM_LOAD16_BYTE( "238.c5", 0x1000000, 0x400000, CRC(c22c68eb) SHA1(a4b04118b1b1909d3b76be8d9ee5d97db6120600) )
	ROM_LOAD16_BYTE( "238.c6", 0x1000001, 0x400000, CRC(119323cd) SHA1(05a9d4b1fb4cc963b25452ff6f81e296e0c0b2a1) )
	ROM_LOAD16_BYTE( "238.c7", 0x1800000, 0x400000, CRC(a72ce7ed) SHA1(05b151554bd7af09ccf554a17bc3c75a0512faaf) )
	ROM_LOAD16_BYTE( "238.c8", 0x1800001, 0x400000, CRC(1c7c2efb) SHA1(b055ee43cbdaf9a3cb19e4e1f9dd2c40bde69d70) )
ROM_END

/****************
 Aero Fighters 2
******************/

ROM_START( sonicwi2hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "075_hack01.p1", 0x100000, 0x100000, CRC(22e4dd5f) SHA1(4f9511c7c2b98db306fece35ada47beb7f3ae4c5) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "075.s1", CRC(c9eec367) SHA1(574e1afe7e0d54610c145131106e59ba2894eeb7) )

	NEO_BIOS_AUDIO_128K( "075.m1", CRC(bb828df1) SHA1(eab8e2868173bdaac7c7ed97305a9aa1033fd303) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "075.v1", 0x000000, 0x200000, CRC(7577e949) SHA1(3ba9f11094dd0cf519f33a16016cfae0d2c6629c) )
	ROM_LOAD( "075.v2", 0x200000, 0x100000, CRC(021760cd) SHA1(8a24e38f1d4982c4dcd82718995571ac94cbb390) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "075.c1", 0x000000, 0x200000, CRC(3278e73e) SHA1(d9e6c8a3a5213690a1b8747d27806d8ac5aac405) )
	ROM_LOAD16_BYTE( "075.c2", 0x000001, 0x200000, CRC(fe6355d6) SHA1(ca72fff7a908b6d9325761079ff2a0e28f34cf89) )
	ROM_LOAD16_BYTE( "075.c3", 0x400000, 0x200000, CRC(c1b438f1) SHA1(b3751c5b426bca0fcc3a58bdb86712c22ef908ab) )
	ROM_LOAD16_BYTE( "075.c4", 0x400001, 0x200000, CRC(1f777206) SHA1(e29c5ae65ebdcc1167a894306d2446ce909639da) )
ROM_END

/****************
 Aero Fighters 3
******************/

ROM_START( sonicwi3hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "097_hack01.p1", 0x100000, 0x100000, CRC(a09735bd) SHA1(ab7b8f14d4e07933bed52219b0c161cfd85601a0) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "097.s1", CRC(8dd66743) SHA1(39214bb25a1d5b44a8524010be05bf5a0211981f) )

	NEO_BIOS_AUDIO_128K( "097.m1", CRC(b20e4291) SHA1(0e891ab53f9fded510295dfc7818bc59b4a9dd97) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "097.v1", 0x000000, 0x400000, CRC(6f885152) SHA1(8175804d5c1420c5d37b733d4a8fa2aa81e59f1b) )
	ROM_LOAD( "097.v2", 0x400000, 0x200000, CRC(3359e868) SHA1(b7efd9f1a6dab33271fe8356bcc863aeae1d3ed8) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "097.c1", 0x000000, 0x400000, CRC(33d0d589) SHA1(fe4aa95555e478ceb2d28fd27d83ee06cd09520c) )
	ROM_LOAD16_BYTE( "097.c2", 0x000001, 0x400000, CRC(186f8b43) SHA1(f5cced93e21dc841b00ebeaa30786cb0e047bd9a) )
	ROM_LOAD16_BYTE( "097.c3", 0x800000, 0x200000, CRC(c339fff5) SHA1(58dfd1e30dc0ad3f816a5dbd1cc7e7ccbb792c53) )
	ROM_LOAD16_BYTE( "097.c4", 0x800001, 0x200000, CRC(84a40c6e) SHA1(061a13fba5fed883e5ee9566cedc208df2511bcf) )
ROM_END

ROM_START( sonicwi3hack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "097_hack01.p1", 0x100000, 0x100000, CRC(da80771c) SHA1(654d7d04d03cf2a30fb9cc612cf099bbea852403) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "097.s1", CRC(8dd66743) SHA1(39214bb25a1d5b44a8524010be05bf5a0211981f) )

	NEO_BIOS_AUDIO_128K( "097.m1", CRC(b20e4291) SHA1(0e891ab53f9fded510295dfc7818bc59b4a9dd97) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "097.v1", 0x000000, 0x400000, CRC(6f885152) SHA1(8175804d5c1420c5d37b733d4a8fa2aa81e59f1b) )
	ROM_LOAD( "097.v2", 0x400000, 0x200000, CRC(3359e868) SHA1(b7efd9f1a6dab33271fe8356bcc863aeae1d3ed8) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "097.c1", 0x000000, 0x400000, CRC(33d0d589) SHA1(fe4aa95555e478ceb2d28fd27d83ee06cd09520c) )
	ROM_LOAD16_BYTE( "097.c2", 0x000001, 0x400000, CRC(186f8b43) SHA1(f5cced93e21dc841b00ebeaa30786cb0e047bd9a) )
	ROM_LOAD16_BYTE( "097.c3", 0x800000, 0x200000, CRC(c339fff5) SHA1(58dfd1e30dc0ad3f816a5dbd1cc7e7ccbb792c53) )
	ROM_LOAD16_BYTE( "097.c4", 0x800001, 0x200000, CRC(84a40c6e) SHA1(061a13fba5fed883e5ee9566cedc208df2511bcf) )
ROM_END

 /**************
 SNK vs. Capcom
*****************/

ROM_START( svcsplushack01 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "svc-p1sp.bin",  0x000000, 0x400000, CRC(2601902f) SHA1(202348a13c6480f7de37a3ee983823838822fc98) )
	ROM_LOAD16_WORD_SWAP( "269_hack01.p2", 0x400000, 0x400000, CRC(c30b4d6c) SHA1(5bd2cc39e988e2c8c92e0df3f75b8343cec5b09c) )
	ROM_IGNORE( 0x054f4c )

	NEO_SFIX_128K( "svc-s1sp.bin", CRC(233d6439) SHA1(369024c7a2405c3144c14ac016c07c3dc0f44187) )

	NEOGEO_BIOS

	ROM_REGION( 0x20000, "audiobios", 0 )
	ROM_LOAD( "sm1.sm1", 0x00000, 0x20000, CRC(94416d67) SHA1(42f9d7ddd6c0931fd64226a60dc73602b2819dcf) )

	ROM_REGION( 0x50000, "audiocpu", 0 )
	ROM_LOAD( "svc-m1.bin", 0x20000, 0x10000, CRC(804328c3) SHA1(f931636c563b0789d4812033a77b47bf663db43f) )
	ROM_CONTINUE( 0x00000, 0x10000 )
	ROM_COPY( "audiocpu", 0x000000, 0x10000, 0x10000 )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD16_WORD_SWAP( "svc-v2.bin", 0x000000, 0x400000, CRC(b5097287) SHA1(3ba3a9b5624879616382ed40337a3d9c50a0f314) )
	ROM_LOAD16_WORD_SWAP( "svc-v1.bin", 0x400000, 0x400000, CRC(bd3a391f) SHA1(972bf09b75e99a683ee965bec93b0da8f15d72d9) )
	ROM_LOAD16_WORD_SWAP( "svc-v4.bin", 0x800000, 0x400000, CRC(33fc0b37) SHA1(d61017d829f44c7df8795ba10c55c727d9972662) )
	ROM_LOAD16_WORD_SWAP( "svc-v3.bin", 0xc00000, 0x400000, CRC(aa9849a0) SHA1(9539b3356a070a066a89f27c287f316e7367ce2a) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "svc-c1.bin", 0x0000000, 0x800000, CRC(a7826b89) SHA1(3bbe348ce54b80b56ef032ea532a18ef3cafeb11) )
	ROM_LOAD16_BYTE( "svc-c2.bin", 0x0000001, 0x800000, CRC(ed3c2089) SHA1(b5d17692f15f5a678c273589fab2e3918711135e) )
	ROM_LOAD16_BYTE( "svc-c3.bin", 0x1000000, 0x800000, CRC(71ed8063) SHA1(ea1df9e2e382a8560a06d447421844cc588f43dd) )
	ROM_LOAD16_BYTE( "svc-c4.bin", 0x1000001, 0x800000, CRC(250bde2d) SHA1(8c72dcfceef6d022ab4b73ab37cf3ac0c3940c17) )
	ROM_LOAD16_BYTE( "svc-c5.bin", 0x2000000, 0x800000, CRC(9817c082) SHA1(1bea9c7220c2b1524896c86841d6d8fd55f5d366) )
	ROM_LOAD16_BYTE( "svc-c6.bin", 0x2000001, 0x800000, CRC(2bc0307f) SHA1(8090fa82c46eb503832359093c8cc3cee3141c90) )
	ROM_LOAD16_BYTE( "svc-c7.bin", 0x3000000, 0x800000, CRC(4358d7b9) SHA1(9270b58c2abc072a046bedda72f1395df26d0714) )
	ROM_LOAD16_BYTE( "svc-c8.bin", 0x3000001, 0x800000, CRC(366deee5) SHA1(d477ad7a5987fd6c7ef2c1680fbb7c884654590e) )
ROM_END

 /*******
 bootleg
**********/

ROM_START( svcfd )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "269fd.p1", 0x000000, 0x600000, CRC(f429e90a) SHA1(bc696bfadb8f3d14eb261eb529fcc1e0712b5bf2) )

	NEO_SFIX_128K( "269fd.s1", CRC(bc670454) SHA1(468641985fd3136235510bc48781039ef7b05c64) )

	NEO_BIOS_AUDIO_128K( "269plusn.m1", CRC(dcc67f10) SHA1(acdd75144f90d2b135181a4fd2c6c72c240c66b0) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269h.v1", 0x000000, 0x400000, CRC(e7203930) SHA1(c6753df1db87d223ee39ffac920da449127dbcdf) )
	ROM_LOAD( "269h.v2", 0x400000, 0x400000, CRC(675159c3) SHA1(c3a4f6db61dfab5bc31bfe94e1c3915676347031) )
	ROM_LOAD( "269h.v3", 0x800000, 0x400000, CRC(f8810665) SHA1(fc1e94629a5124076959938257a80d04b2e871eb) )
	ROM_LOAD( "269h.v4", 0xc00000, 0x400000, CRC(b57b4ea3) SHA1(873dc7f5eae687bc70af01ea310e935287de0ebf) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269d.c1", 0x0000000, 0x800000, CRC(465d473b) SHA1(0b2BE2B2657C2AE021EC91515D1D6306179751D5) )
	ROM_LOAD16_BYTE( "269d.c2", 0x0000001, 0x800000, CRC(3eb28f78) SHA1(DF1B096FE95796DB1515B69F74266ED13DD9EE56) )
	ROM_LOAD16_BYTE( "269d.c3", 0x1000000, 0x800000, CRC(f4d4ab2b) SHA1(A19788435173540469FE5D07429A83DDE6D701A0) )
	ROM_LOAD16_BYTE( "269d.c4", 0x1000001, 0x800000, CRC(a69d523a) SHA1(98D8DAC2E40F71B5B8426D3C77476A0A20783181) )
	ROM_LOAD16_BYTE( "269d.c5", 0x2000000, 0x800000, CRC(ba2a7892) SHA1(EED45E1330DCC9F610598DC53977BF3A81DD161A) )
	ROM_LOAD16_BYTE( "269d.c6", 0x2000001, 0x800000, CRC(37371ca1) SHA1(8AAA1B9EA34470085D9944C9AD64CFB0FFE09859) )
	ROM_LOAD16_BYTE( "269d.c7", 0x3000000, 0x800000, CRC(56ac5fb9) SHA1(74CB0D57060D25E35F76A9820F607633BD1B9C2C) )
	ROM_LOAD16_BYTE( "269d.c8", 0x3000001, 0x800000, CRC(de99e613) SHA1(F28C6AF4A31A937E5B441D4B860E3CCCA725F27A) )
ROM_END

ROM_START( svcd )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "269.p1", 0x000000, 0x400000, CRC(38e2005e) SHA1(1b902905916a30969282f1399a756e32ff069097) )
	ROM_LOAD32_WORD_SWAP( "269.p2", 0x000002, 0x400000, CRC(6d13797c) SHA1(3cb71a95cea6b006b44cac0f547df88aec0007b7) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_128K( "269d.m1", CRC(447b3123) SHA1(a09adc2c0ee2ee0f01287ceb97474a1a58093bd2) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269.v1", 0x000000, 0x800000, CRC(c659b34c) SHA1(1931e8111ef43946f68699f8707334c96f753a1e) )
	ROM_LOAD( "269.v2", 0x800000, 0x800000, CRC(dd903835) SHA1(e58d38950a7a8697bb22a1cc7a371ae6664ae8f9) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269d.c1", 0x0000000, 0x800000, CRC(465d473b) SHA1(0b2BE2B2657C2AE021EC91515D1D6306179751D5) )
	ROM_LOAD16_BYTE( "269d.c2", 0x0000001, 0x800000, CRC(3eb28f78) SHA1(DF1B096FE95796DB1515B69F74266ED13DD9EE56) )
	ROM_LOAD16_BYTE( "269d.c3", 0x1000000, 0x800000, CRC(f4d4ab2b) SHA1(A19788435173540469FE5D07429A83DDE6D701A0) )
	ROM_LOAD16_BYTE( "269d.c4", 0x1000001, 0x800000, CRC(a69d523a) SHA1(98D8DAC2E40F71B5B8426D3C77476A0A20783181) )
	ROM_LOAD16_BYTE( "269d.c5", 0x2000000, 0x800000, CRC(ba2a7892) SHA1(EED45E1330DCC9F610598DC53977BF3A81DD161A) )
	ROM_LOAD16_BYTE( "269d.c6", 0x2000001, 0x800000, CRC(37371ca1) SHA1(8AAA1B9EA34470085D9944C9AD64CFB0FFE09859) )
	ROM_LOAD16_BYTE( "269d.c7", 0x3000000, 0x800000, CRC(56ac5fb9) SHA1(74CB0D57060D25E35F76A9820F607633BD1B9C2C) )
	ROM_LOAD16_BYTE( "269d.c8", 0x3000001, 0x800000, CRC(de99e613) SHA1(F28C6AF4A31A937E5B441D4B860E3CCCA725F27A) )
ROM_END

ROM_START( svcd1 )
	ROM_REGION( 0x800000, "maincpu", 0 )
	ROM_LOAD32_WORD_SWAP( "269.p1", 0x000000, 0x400000, CRC(38e2005e) SHA1(1b902905916a30969282f1399a756e32ff069097) )
	ROM_LOAD32_WORD_SWAP( "269.p2", 0x000002, 0x400000, CRC(6d13797c) SHA1(3cb71a95cea6b006b44cac0f547df88aec0007b7) )

	NEO_SFIX_MT_512K

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "269.m1", CRC(f6819d00) SHA1(d3bbe09df502464f104e53501708ac6e2c1832c6) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269.v1", 0x000000, 0x800000, CRC(c659b34c) SHA1(1931e8111ef43946f68699f8707334c96f753a1e) )
	ROM_LOAD( "269.v2", 0x800000, 0x800000, CRC(dd903835) SHA1(e58d38950a7a8697bb22a1cc7a371ae6664ae8f9) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269d.c1", 0x0000000, 0x800000, CRC(465d473b) SHA1(0b2BE2B2657C2AE021EC91515D1D6306179751D5) )
	ROM_LOAD16_BYTE( "269d.c2", 0x0000001, 0x800000, CRC(3eb28f78) SHA1(DF1B096FE95796DB1515B69F74266ED13DD9EE56) )
	ROM_LOAD16_BYTE( "269d.c3", 0x1000000, 0x800000, CRC(f4d4ab2b) SHA1(A19788435173540469FE5D07429A83DDE6D701A0) )
	ROM_LOAD16_BYTE( "269d.c4", 0x1000001, 0x800000, CRC(a69d523a) SHA1(98D8DAC2E40F71B5B8426D3C77476A0A20783181) )
	ROM_LOAD16_BYTE( "269d.c5", 0x2000000, 0x800000, CRC(ba2a7892) SHA1(EED45E1330DCC9F610598DC53977BF3A81DD161A) )
	ROM_LOAD16_BYTE( "269d.c6", 0x2000001, 0x800000, CRC(37371ca1) SHA1(8AAA1B9EA34470085D9944C9AD64CFB0FFE09859) )
	ROM_LOAD16_BYTE( "269d1.c7", 0x3000000, 0x800000, CRC(5595b6cc) SHA1(9d6cc70f435401355b885475ccc3864825887656) )
	ROM_LOAD16_BYTE( "269d1.c8", 0x3000001, 0x800000, CRC(b17dfcf9) SHA1(46af296b12831ee40355f52d2c316f609557d6df) )
ROM_END

ROM_START( svcnd ) 
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "269nd.p1", 0x000000, 0x600000, CRC(93855c0b) SHA1(03dd06ff5a11a37990bd0a9531beb44c63de2d91) )

	NEO_SFIX_MT_512K
	ROM_LOAD( "269nd.s1", 0x60000, 0x20000, CRC(ad184232) SHA1(6645d323d641004fa7a17e3b1e65613f398c95dd) )
	ROM_CONTINUE( 0x40000, 0x20000 )
	ROM_CONTINUE( 0x20000, 0x20000 )
	ROM_CONTINUE( 0x00000, 0x20000 )

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "269.m1", CRC(f6819d00) SHA1(d3bbe09df502464f104e53501708ac6e2c1832c6) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269pcbd.v1", 0x000000, 0x800000, CRC(ff64cd56) SHA1(e2754c554ed5ca14c2020c5d931021d5ac82660c) )
	ROM_LOAD( "269pcbd.v2", 0x800000, 0x800000, CRC(a8dd6446) SHA1(8972aab271c33f8af344bffe6359d9ddc4b8af2e) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269pcbd.c1", 0x0000000, 0x2000000, CRC(382ce01f) SHA1(8eec32f70169de83fc15df470aba9c51c312b577) )
	ROM_LOAD16_BYTE( "269pcbd.c2", 0x0000001, 0x2000000, CRC(88ad01ec) SHA1(da223bc09aa465ea6c15954c45fefbf3ee79a4d7) )
ROM_END

ROM_START( svcplusn )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "269plusn.p1", 0x000000, 0x600000, CRC(1cc8fb7a) SHA1(96fc02803d597c8083a8a25ca1fedb9f50553c8d) )

	NEO_SFIX_128K( "269plusn.s1", CRC(aaef740c) SHA1(d7ae898ac93f78b19b125e98659dcfd9ee3179b2) )

	NEO_BIOS_AUDIO_128K( "269plusn.m1", CRC(dcc67f10) SHA1(acdd75144f90d2b135181a4fd2c6c72c240c66b0) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269h.v1", 0x000000, 0x400000, CRC(e7203930) SHA1(c6753df1db87d223ee39ffac920da449127dbcdf) )
	ROM_LOAD( "269h.v2", 0x400000, 0x400000, CRC(675159c3) SHA1(c3a4f6db61dfab5bc31bfe94e1c3915676347031) )
	ROM_LOAD( "269h.v3", 0x800000, 0x400000, CRC(f8810665) SHA1(fc1e94629a5124076959938257a80d04b2e871eb) )
	ROM_LOAD( "269h.v4", 0xc00000, 0x400000, CRC(b57b4ea3) SHA1(873dc7f5eae687bc70af01ea310e935287de0ebf) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269d.c1", 0x0000000, 0x800000, CRC(465d473b) SHA1(0b2BE2B2657C2AE021EC91515D1D6306179751D5) )
	ROM_LOAD16_BYTE( "269d.c2", 0x0000001, 0x800000, CRC(3eb28f78) SHA1(DF1B096FE95796DB1515B69F74266ED13DD9EE56) )
	ROM_LOAD16_BYTE( "269d.c3", 0x1000000, 0x800000, CRC(f4d4ab2b) SHA1(A19788435173540469FE5D07429A83DDE6D701A0) )
	ROM_LOAD16_BYTE( "269d.c4", 0x1000001, 0x800000, CRC(a69d523a) SHA1(98D8DAC2E40F71B5B8426D3C77476A0A20783181) )
	ROM_LOAD16_BYTE( "269d.c5", 0x2000000, 0x800000, CRC(ba2a7892) SHA1(EED45E1330DCC9F610598DC53977BF3A81DD161A) )
	ROM_LOAD16_BYTE( "269d.c6", 0x2000001, 0x800000, CRC(37371ca1) SHA1(8AAA1B9EA34470085D9944C9AD64CFB0FFE09859) )
	ROM_LOAD16_BYTE( "269d.c7", 0x3000000, 0x800000, CRC(56ac5fb9) SHA1(74CB0D57060D25E35F76A9820F607633BD1B9C2C) )
	ROM_LOAD16_BYTE( "269d.c8", 0x3000001, 0x800000, CRC(de99e613) SHA1(F28C6AF4A31A937E5B441D4B860E3CCCA725F27A) )
ROM_END

ROM_START( svcplusb )
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "269plusb.p1", 0x000000, 0x600000, CRC(5D738F9A) SHA1(10AD161D6DE285659DCAEBBEA4AA2B384341E14D) )

	NEO_SFIX_MT_512K
	ROM_LOAD( "269plusb.s1", 0x60000, 0x20000, CRC(06125f98) SHA1(69ba2625c90bb09abf64f6d49d88d2ac2f1ee9ca) )
	ROM_CONTINUE( 0x40000, 0x20000 )
	ROM_CONTINUE( 0x20000, 0x20000 )
	ROM_CONTINUE( 0x00000, 0x20000 )

	NEO_BIOS_AUDIO_128K( "269rm.m1", CRC(fd602218) SHA1(d7af0af3e116a7c2f594d1ca77a1289b4e31cd7e) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269h.v1", 0x000000, 0x400000, CRC(e7203930) SHA1(c6753df1db87d223ee39ffac920da449127dbcdf) )
	ROM_LOAD( "269h.v2", 0x400000, 0x400000, CRC(675159c3) SHA1(c3a4f6db61dfab5bc31bfe94e1c3915676347031) )
	ROM_LOAD( "269h.v3", 0x800000, 0x400000, CRC(f8810665) SHA1(fc1e94629a5124076959938257a80d04b2e871eb) )
	ROM_LOAD( "269h.v4", 0xc00000, 0x400000, CRC(b57b4ea3) SHA1(873dc7f5eae687bc70af01ea310e935287de0ebf) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269d.c1", 0x0000000, 0x800000, CRC(465d473b) SHA1(0b2BE2B2657C2AE021EC91515D1D6306179751D5) )
	ROM_LOAD16_BYTE( "269d.c2", 0x0000001, 0x800000, CRC(3eb28f78) SHA1(DF1B096FE95796DB1515B69F74266ED13DD9EE56) )
	ROM_LOAD16_BYTE( "269d.c3", 0x1000000, 0x800000, CRC(f4d4ab2b) SHA1(A19788435173540469FE5D07429A83DDE6D701A0) )
	ROM_LOAD16_BYTE( "269d.c4", 0x1000001, 0x800000, CRC(a69d523a) SHA1(98D8DAC2E40F71B5B8426D3C77476A0A20783181) )
	ROM_LOAD16_BYTE( "269d.c5", 0x2000000, 0x800000, CRC(ba2a7892) SHA1(EED45E1330DCC9F610598DC53977BF3A81DD161A) )
	ROM_LOAD16_BYTE( "269d.c6", 0x2000001, 0x800000, CRC(37371ca1) SHA1(8AAA1B9EA34470085D9944C9AD64CFB0FFE09859) )
	ROM_LOAD16_BYTE( "269d.c7", 0x3000000, 0x800000, CRC(56ac5fb9) SHA1(74CB0D57060D25E35F76A9820F607633BD1B9C2C) )
	ROM_LOAD16_BYTE( "269d.c8", 0x3000001, 0x800000, CRC(de99e613) SHA1(F28C6AF4A31A937E5B441D4B860E3CCCA725F27A) )
ROM_END

ROM_START( svcplusd ) // only p1,vx,cx confirmed
	ROM_REGION( 0x600000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "269plusd.p1", 0x000000, 0x600000, CRC(2a57e4d4) SHA1(09fec6b376adfb2b051cd281f66b483e2deffa60) )

	NEO_SFIX_MT_512K // This one contains Plus logo
	ROM_LOAD( "269plusb.s1", 0x60000, 0x20000, CRC(06125f98) SHA1(69ba2625c90bb09abf64f6d49d88d2ac2f1ee9ca) )
	ROM_CONTINUE( 0x40000, 0x20000 )
	ROM_CONTINUE( 0x20000, 0x20000 )
	ROM_CONTINUE( 0x00000, 0x20000 )

	NEO_BIOS_AUDIO_ENCRYPTED_512K( "269.m1", CRC(f6819d00) SHA1(d3bbe09df502464f104e53501708ac6e2c1832c6) )

	ROM_REGION( 0x1000000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "269pcbd.v1", 0x000000, 0x800000, CRC(ff64cd56) SHA1(e2754c554ed5ca14c2020c5d931021d5ac82660c) )
	ROM_LOAD( "269pcbd.v2", 0x800000, 0x800000, CRC(a8dd6446) SHA1(8972aab271c33f8af344bffe6359d9ddc4b8af2e) )

	ROM_REGION( 0x4000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "269pcbd.c1", 0x0000000, 0x2000000, CRC(382ce01f) SHA1(8eec32f70169de83fc15df470aba9c51c312b577) )
	ROM_LOAD16_BYTE( "269pcbd.c2", 0x0000001, 0x2000000, CRC(88ad01ec) SHA1(da223bc09aa465ea6c15954c45fefbf3ee79a4d7) )
ROM_END

/************
 Spin Master
**************/

ROM_START( spinmasthack01 )
	ROM_REGION( 0x180000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "062_hack01.p1", 0x000000, 0x100000, CRC(51d531ee) SHA1(bc8b16ddc76d14de9d239efddd3a1cdfbf207521) )
	ROM_LOAD16_WORD_SWAP( "062_hack01.p2", 0x100000, 0x080000, CRC(43763ad2) SHA1(9b08cf1a79294c3206f6364466cae2c8b15acad5) )

	NEO_SFIX_128K( "062.s1", CRC(289e2bbe) SHA1(f52c7f2bffc89df3130b3cabd200408509a28cdc) )

	NEO_BIOS_AUDIO_128K( "062.m1", CRC(76108b2f) SHA1(08c89a8b746dbb10ff885b41cde344173c2e3699) )

	ROM_REGION( 0x100000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "062.v1", 0x000000, 0x100000, CRC(cc281aef) SHA1(68be154b3e25f837afb4a477600dbe0ee69bec44) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "062.c1", 0x000000, 0x100000, CRC(a9375aa2) SHA1(69218d8f1361e9ea709da11e3f15fe46b1db7181) )
	ROM_LOAD16_BYTE( "062.c2", 0x000001, 0x100000, CRC(0e73b758) SHA1(a247f736fbca0b609818dca4844ebb8442753bc1) )
	ROM_LOAD16_BYTE( "062.c3", 0x200000, 0x100000, CRC(df51e465) SHA1(171953c7a870f3ab96e0f875117ee7343931fd38) )
	ROM_LOAD16_BYTE( "062.c4", 0x200001, 0x100000, CRC(38517e90) SHA1(f7c64b94ac20f5146f9bb48b53cb2b30fe5b8f8c) )
	ROM_LOAD16_BYTE( "062.c5", 0x400000, 0x100000, CRC(7babd692) SHA1(0d4cd5006baa8d951cd2b6194ace566fa2845b8a) )
	ROM_LOAD16_BYTE( "062.c6", 0x400001, 0x100000, CRC(cde5ade5) SHA1(5899ef5dfcdbb8cf8c6aba748dbb52f3c5fed5fe) )
	ROM_LOAD16_BYTE( "062.c7", 0x600000, 0x100000, CRC(bb2fd7c0) SHA1(cce11c4cf39ac60143235ff89261806df339dae5) )
	ROM_LOAD16_BYTE( "062.c8", 0x600001, 0x100000, CRC(8d7be933) SHA1(e7097cfa26a959f90721e2e8368ceb47ea9db661) )
ROM_END

/***********
 Top Hunter
*************/

ROM_START( tophuntrhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "046_hack01.p1",   0x000000, 0x100000, CRC(949dbb78) SHA1(fb005e60ff693c6656e33cd0641c7300826a6b54) )
	ROM_LOAD16_WORD_SWAP( "046.p2", 0x100000, 0x100000, CRC(f182cb3e) SHA1(6b4e0af5d4e623f0682f37ff5c69e5b705e20028) )

	NEO_SFIX_128K( "046.s1", CRC(14b01d7b) SHA1(618ce75c25d6cc86a3b46bd64a0aa34ab82f75ae) )

	NEO_BIOS_AUDIO_128K( "046.m1", CRC(3f84bb9f) SHA1(07446040871d11da3c2217ee9d1faf8c3cae7420) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "046.v1", 0x000000, 0x100000, CRC(c1f9c2db) SHA1(bed95a76afefa46503a12e0f0a9787c4c967ac50) )
	ROM_LOAD( "046.v2", 0x100000, 0x100000, CRC(56254a64) SHA1(1cf049cb4c414419859d2c8ee714317a35a85251) )
	ROM_LOAD( "046.v3", 0x200000, 0x100000, CRC(58113fb1) SHA1(40972982a63c7adecef840f9882f4165da723ab6) )
	ROM_LOAD( "046.v4", 0x300000, 0x100000, CRC(4f54c187) SHA1(63a76949301b83bdd44aa1a4462f642ab9ca3c0b) )

	ROM_REGION( 0x800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "046.c1", 0x000000, 0x100000, CRC(fa720a4a) SHA1(364913b9fa40d46e4e39ae3cdae914cfd0de137d) )
	ROM_LOAD16_BYTE( "046.c2", 0x000001, 0x100000, CRC(c900c205) SHA1(50274e79aa26f334eb806288688b30720bade883) )
	ROM_LOAD16_BYTE( "046.c3", 0x200000, 0x100000, CRC(880e3c25) SHA1(b6974af0c833b766866919b6f15b6f8cef82530d) )
	ROM_LOAD16_BYTE( "046.c4", 0x200001, 0x100000, CRC(7a2248aa) SHA1(8af0b26025a54e3b91604dd24a3c1c518fbd8536) )
	ROM_LOAD16_BYTE( "046.c5", 0x400000, 0x100000, CRC(4b735e45) SHA1(2f8b46388c4696aee6a97e1e21cdadf6b142b01a) )
	ROM_LOAD16_BYTE( "046.c6", 0x400001, 0x100000, CRC(273171df) SHA1(9c35832221e016c12ef1ed71da167f565daaf86c) )
	ROM_LOAD16_BYTE( "046.c7", 0x600000, 0x100000, CRC(12829c4c) SHA1(ac5f3d848d7116fc35c97f53a72c85e049dd3a2f) )
	ROM_LOAD16_BYTE( "046.c8", 0x600001, 0x100000, CRC(c944e03d) SHA1(be23999b8ce09ee15ba500ce4d5e2a82a4f58d9b) )
ROM_END

/**********************
 Twinkle Star Sprites
************************/

ROM_START( twinsprihack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "224_hack01.p1", 0x100000, 0x100000, CRC(df71c8cd) SHA1(1e49205b46c59c7f4ca6ee1dc06f555320d3b9af) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "224.s1", CRC(eeed5758) SHA1(24e48f396716e145b692468762cf595fb7267873) )

	NEO_BIOS_AUDIO_128K( "224.m1", CRC(364d6f96) SHA1(779b95a6476089b71f48c8368d9043ee1dba9032) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "224.v1", 0x000000, 0x400000, CRC(ff57f088) SHA1(1641989b8aac899dbd68aa2332bcdf9b90b33564) )
	ROM_LOAD( "224.v2", 0x400000, 0x200000, CRC(7ad26599) SHA1(822030037b7664795bf3d64e1452d0aecc22497e) )

	ROM_REGION( 0xa00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "224.c1", 0x000000, 0x400000, CRC(f7da64ab) SHA1(587a10ed9235c9046a3523fe80feba07764fac9b) )
	ROM_LOAD16_BYTE( "224.c2", 0x000001, 0x400000, CRC(4c09bbfb) SHA1(e781aafba3bdefb7ed152826f4c3eb441735331c) )
	ROM_LOAD16_BYTE( "224.c3", 0x800000, 0x100000, CRC(c59e4129) SHA1(93f02d1b4fbb152a9d336494fbff0d7642921de5) )
	ROM_LOAD16_BYTE( "224.c4", 0x800001, 0x100000, CRC(b5532e53) SHA1(7d896c25ba97f6e5d43c13d4df4ba72964a976ed) )
ROM_END

/**********
 Viewpoint
************/

ROM_START( viewpoinhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "051_hack01.p1", 0x000000, 0x100000, CRC(d28c99ef) SHA1(c603ae8ad4a80fa3921016cd294a61a60cc37923) )

	NEO_SFIX_64K( "051_hack01.s1", CRC(6d0f146a) SHA1(537fc5ef099f46fef64d147c0d1264b319758b4f) )

	NEO_BIOS_AUDIO_64K( "051_hack01.m1", CRC(d57bd7af) SHA1(9ed766dbc3c07dbba7869ff335eeb7e297c8e2d9) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "051.v1", 0x000000, 0x200000, CRC(019978b6) SHA1(7896a551115fc6ed38b5944e0c8dcb2b2c1c077d) )
	ROM_LOAD( "051.v2", 0x200000, 0x200000, CRC(5758f38c) SHA1(da10f4b7d22d9139bbf068bd940be82168a74ca1) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "051.c1", 0x000000, 0x100000, CRC(d624c132) SHA1(49c7e9f020cba45d7083b45252bcc03397f8c286) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "051.c2", 0x000001, 0x100000, CRC(40d69f1e) SHA1(ec4a13582772594957f927622d50f54b0dfcd8d8) )
	ROM_CONTINUE( 0x400001, 0x100000 )
ROM_END

ROM_START( viewpoinhack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "051_hack02.p1", 0x000000, 0x100000, CRC(063b9fa2) SHA1(e2d43646a976d82c809c6fad7dd40cc3dda07b15) )

	NEO_SFIX_64K( "051tr.s1", CRC(6d0f146a) SHA1(537fc5ef099f46fef64d147c0d1264b319758b4f) )

	NEO_BIOS_AUDIO_64K( "051tr.m1", CRC(d57bd7af) SHA1(9ed766dbc3c07dbba7869ff335eeb7e297c8e2d9) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "051.v1", 0x000000, 0x200000, CRC(019978b6) SHA1(7896a551115fc6ed38b5944e0c8dcb2b2c1c077d) )
	ROM_LOAD( "051.v2", 0x200000, 0x200000, CRC(5758f38c) SHA1(da10f4b7d22d9139bbf068bd940be82168a74ca1) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "051.c1", 0x000000, 0x100000, CRC(d624c132) SHA1(49c7e9f020cba45d7083b45252bcc03397f8c286) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "051.c2", 0x000001, 0x100000, CRC(40d69f1e) SHA1(ec4a13582772594957f927622d50f54b0dfcd8d8) )
	ROM_CONTINUE( 0x400001, 0x100000 )
ROM_END

/************
 Waku Waku 7
*************/

ROM_START( wakuwak7hack01 )
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "225_hack01.p1", 0x000000, 0x100000, CRC(0b7a3776) SHA1(3d8d9e7bdd481dbec3765901999b5148c0f3d776) )
	ROM_LOAD16_WORD_SWAP( "225.p2", 0x100000, 0x200000, CRC(fe190665) SHA1(739d9a8fc2da34381654d9e291141eacc210ae5c) )

	NEO_SFIX_128K( "225.s1", CRC(71c4b4b5) SHA1(9410f13807f01082dc86f2d84051be4bed8e9f7c) )

	NEO_BIOS_AUDIO_128K( "225.m1", CRC(0634bba6) SHA1(153aaf016440500df7a4454f3f2f2911219cb7d8) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "225.v1", 0x000000, 0x400000, CRC(6195c6b4) SHA1(66c06b5904aedb256e3997bbec60f8ab50c6ff0c) )
	ROM_LOAD( "225.v2", 0x400000, 0x400000, CRC(6159c5fe) SHA1(9015e93416497f1ef877c717afed40f7ecfa42e4) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "225.c1", 0x0000000, 0x400000, CRC(ee4fea54) SHA1(314b513a52b2cc88cbf2409d1934c357269a8bb2) )
	ROM_LOAD16_BYTE( "225.c2", 0x0000001, 0x400000, CRC(0c549e2d) SHA1(d8c4626231c92e43d9bf183202553ee2b5c532e6) )
	ROM_LOAD16_BYTE( "225.c3", 0x0800000, 0x400000, CRC(af0897c0) SHA1(2b8ec19b9dd0bd1f1171fb01b915e9d25ec8c421) )
	ROM_LOAD16_BYTE( "225.c4", 0x0800001, 0x400000, CRC(4c66527a) SHA1(6c8c9342fad70b456e282b0d52e7ad890e4673d3) )
	ROM_LOAD16_BYTE( "225.c5", 0x1000000, 0x400000, CRC(8ecea2b5) SHA1(cad51e6e76d8258a78becb6f4096dd061f537494) )
	ROM_LOAD16_BYTE( "225.c6", 0x1000001, 0x400000, CRC(0eb11a6d) SHA1(c6d4f978ff3ca190a3060ac52bd7347189194f76) )
ROM_END

ROM_START( wakuwak7hack02 ) //wakuwak7l
	ROM_REGION( 0x300000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "225_hack02.p1",   0x000000, 0x100000, CRC(a1d46442) SHA1(7d9b3f78b54add21b6619ffd34786958464505bb) )
	ROM_LOAD16_WORD_SWAP( "225.p2", 0x100000, 0x200000, CRC(fe190665) SHA1(739d9a8fc2da34381654d9e291141eacc210ae5c) )

	NEO_SFIX_128K( "225.s1", CRC(71c4b4b5) SHA1(9410f13807f01082dc86f2d84051be4bed8e9f7c) )

	NEO_BIOS_AUDIO_128K( "225.m1", CRC(0634bba6) SHA1(153aaf016440500df7a4454f3f2f2911219cb7d8) )

	ROM_REGION( 0x800000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "225.v1", 0x000000, 0x400000, CRC(6195c6b4) SHA1(66c06b5904aedb256e3997bbec60f8ab50c6ff0c) )
	ROM_LOAD( "225.v2", 0x400000, 0x400000, CRC(6159c5fe) SHA1(9015e93416497f1ef877c717afed40f7ecfa42e4) )

	ROM_REGION( 0x1800000, "sprites", 0 )
	ROM_LOAD16_BYTE( "225.c1", 0x0000000, 0x400000, CRC(ee4fea54) SHA1(314b513a52b2cc88cbf2409d1934c357269a8bb2) )
	ROM_LOAD16_BYTE( "225.c2", 0x0000001, 0x400000, CRC(0c549e2d) SHA1(d8c4626231c92e43d9bf183202553ee2b5c532e6) )
	ROM_LOAD16_BYTE( "225.c3", 0x0800000, 0x400000, CRC(af0897c0) SHA1(2b8ec19b9dd0bd1f1171fb01b915e9d25ec8c421) )
	ROM_LOAD16_BYTE( "225.c4", 0x0800001, 0x400000, CRC(4c66527a) SHA1(6c8c9342fad70b456e282b0d52e7ad890e4673d3) )
	ROM_LOAD16_BYTE( "225.c5", 0x1000000, 0x400000, CRC(8ecea2b5) SHA1(cad51e6e76d8258a78becb6f4096dd061f537494) )
	ROM_LOAD16_BYTE( "225.c6", 0x1000001, 0x400000, CRC(0eb11a6d) SHA1(c6d4f978ff3ca190a3060ac52bd7347189194f76) )
ROM_END

/************
 Windjammers
*************/

ROM_START( wjammershack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "065_hack01.p1", 0x000000, 0x100000, CRC(a458cc18) SHA1(0d476edef593c4fa6f50ec4457f69629b2be37e7) )

	NEO_SFIX_128K( "065.s1", CRC(074b5723) SHA1(86d3b3bb5414f43e4d3b7a2af0db23cc71ce8412) )

	NEO_BIOS_AUDIO_128K( "065.m1", CRC(52c23cfc) SHA1(809a7e072ad9acbffc25e9bd27cdb97638d09d07) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "065.v1", 0x000000, 0x100000, CRC(ce8b3698) SHA1(7d75e2a5cf8c90be422f8b425447e81597fe145a) )
	ROM_LOAD( "065.v2", 0x100000, 0x100000, CRC(659f9b96) SHA1(62f40365212153bc3b92a1187fa44f6cdc7f7b83) )
	ROM_LOAD( "065.v3", 0x200000, 0x100000, CRC(39f73061) SHA1(ec57cd58e7f8569cff925d11e2320d588ce4fe49) )
	ROM_LOAD( "065.v4", 0x300000, 0x100000, CRC(5dee7963) SHA1(f8e6de73d65dd80b29c711f00835a574a770cb4e) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "065.c1", 0x000000, 0x100000, CRC(c7650204) SHA1(42918d700d59864f8ab15caf968a062a563c9b09) )
	ROM_LOAD16_BYTE( "065.c2", 0x000001, 0x100000, CRC(d9f3e71d) SHA1(fad1f64061eac1bf85bf6d75d2eae974a8c94069) )
	ROM_LOAD16_BYTE( "065.c3", 0x200000, 0x100000, CRC(40986386) SHA1(65795a50197049681265946713d416c9cdb68f08) )
	ROM_LOAD16_BYTE( "065.c4", 0x200001, 0x100000, CRC(715e15ff) SHA1(ac8b8b01f5c7384b883afbe0cf977430378e3fef) )
ROM_END

ROM_START( wjammershack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "065.p1", 0x000000, 0x100000, CRC(6692c140) SHA1(5da574e906974fac92bb2f49bdeea257c014a897) )

	NEO_SFIX_128K( "065.s1", CRC(074b5723) SHA1(86d3b3bb5414f43e4d3b7a2af0db23cc71ce8412) )

	NEO_BIOS_AUDIO_128K( "065.m1", CRC(52c23cfc) SHA1(809a7e072ad9acbffc25e9bd27cdb97638d09d07) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "065.v1", 0x000000, 0x100000, CRC(ce8b3698) SHA1(7d75e2a5cf8c90be422f8b425447e81597fe145a) )
	ROM_LOAD( "065.v2", 0x100000, 0x100000, CRC(659f9b96) SHA1(62f40365212153bc3b92a1187fa44f6cdc7f7b83) )
	ROM_LOAD( "065.v3", 0x200000, 0x100000, CRC(39f73061) SHA1(ec57cd58e7f8569cff925d11e2320d588ce4fe49) )
	ROM_LOAD( "065.v4", 0x300000, 0x100000, CRC(5dee7963) SHA1(f8e6de73d65dd80b29c711f00835a574a770cb4e) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "065_hack02.c1",   0x000000, 0x100000, CRC(ebded27e) SHA1(574a0086bf13a5ac5d27de90c4d4611a19668cf8) )
	ROM_LOAD16_BYTE( "065_hack02.c2",   0x000001, 0x100000, CRC(f5483767) SHA1(756dd8e161bf428659c642b5056aa0bcf5e0d9a8) )
	ROM_LOAD16_BYTE( "065.c3", 0x200000, 0x100000, CRC(40986386) SHA1(65795a50197049681265946713d416c9cdb68f08) )
	ROM_LOAD16_BYTE( "065.c4", 0x200001, 0x100000, CRC(715e15ff) SHA1(ac8b8b01f5c7384b883afbe0cf977430378e3fef) )
ROM_END

 /********************
 World Heroes Perfect
***********************/

ROM_START( whphack01 ) 
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "090_hack01.p1", 0x100000, 0x100000, CRC(9bd000f2) SHA1(3f3b2cc47a0ef33f2b8e7ba6e6e81ebdff39a54b) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "090bh.s1", CRC(f2de6500) SHA1(5acc7746394724903b0fd3d4b5acf5bab355c9f4) )

	NEO_BIOS_AUDIO_128K( "090.m1", CRC(28065668) SHA1(0c60d4afa1dccad0135e733104f056be73b54e4e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "090.v1", 0x000000, 0x200000, CRC(30cf2709) SHA1(d1845033f16de2470afd3858ee0efb45176d9ed7) )
	ROM_LOAD( "064.v2", 0x200000, 0x200000, CRC(b6527edd) SHA1(2bcf5bfa6e117cf4a3728a5e5f5771313c93f22a) )
	ROM_LOAD( "090.v3", 0x400000, 0x200000, CRC(1908a7ce) SHA1(78f31bcfea33eb94752bbf5226c481baec1af5ac) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "090.c1", 0x0000000, 0x400000, CRC(cd30ed9b) SHA1(839c20f7ff31251acc875ae402b5d267e55510c7) )
	ROM_LOAD16_BYTE( "090.c2", 0x0000001, 0x400000, CRC(10eed5ee) SHA1(12131b1c8c017ea77a98c044b392a5db6aad0143) )
	ROM_LOAD16_BYTE( "064.c3", 0x0800000, 0x200000, CRC(436d1b31) SHA1(059776d77b91377ed0bcfc278802d659c917fc0f) )
	ROM_LOAD16_BYTE( "064.c4", 0x0800001, 0x200000, CRC(f9c8dd26) SHA1(25a9eea1d49b21b4a988beb32c25bf2f7796f227) )
	ROM_LOAD16_BYTE( "064.c5", 0x1000000, 0x200000, CRC(8e34a9f4) SHA1(67b839b426ef3fad0a85d951fdd44c0a45c55226) )
	ROM_LOAD16_BYTE( "064.c6", 0x1000001, 0x200000, CRC(a43e4766) SHA1(54f282f2b1ff2934cca7acbb4386a2b99a29df3a) )
	ROM_LOAD16_BYTE( "064.c7", 0x1800000, 0x200000, CRC(59d97215) SHA1(85a960dc7f364df13ee0c2f99a4c53aefb081486) )
	ROM_LOAD16_BYTE( "064.c8", 0x1800001, 0x200000, CRC(fc092367) SHA1(69ff4ae909dd857de3ca8645d63f8b4bde117448) )
ROM_END

ROM_START( whphack02 ) //whpl
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "090_hack02.p1", 0x100000, 0x100000, CRC(bbe6344b) SHA1(36b8853246fa0090e7624389e90f08238088a2d1) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "090.s1", CRC(174a880f) SHA1(c35d315d728d119a6e9aa42e0593937c90897449) )

	NEO_BIOS_AUDIO_128K( "090.m1", CRC(28065668) SHA1(0c60d4afa1dccad0135e733104f056be73b54e4e) )

	ROM_REGION( 0x600000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "090.v1", 0x000000, 0x200000, CRC(30cf2709) SHA1(d1845033f16de2470afd3858ee0efb45176d9ed7) )
	ROM_LOAD( "064.v2", 0x200000, 0x200000, CRC(b6527edd) SHA1(2bcf5bfa6e117cf4a3728a5e5f5771313c93f22a) )
	ROM_LOAD( "090.v3", 0x400000, 0x200000, CRC(1908a7ce) SHA1(78f31bcfea33eb94752bbf5226c481baec1af5ac) )

	ROM_REGION( 0x1c00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "090.c1", 0x0000000, 0x400000, CRC(cd30ed9b) SHA1(839c20f7ff31251acc875ae402b5d267e55510c7) )
	ROM_LOAD16_BYTE( "090.c2", 0x0000001, 0x400000, CRC(10eed5ee) SHA1(12131b1c8c017ea77a98c044b392a5db6aad0143) )
	ROM_LOAD16_BYTE( "064.c3", 0x0800000, 0x200000, CRC(436d1b31) SHA1(059776d77b91377ed0bcfc278802d659c917fc0f) )
	ROM_LOAD16_BYTE( "064.c4", 0x0800001, 0x200000, CRC(f9c8dd26) SHA1(25a9eea1d49b21b4a988beb32c25bf2f7796f227) )
	ROM_LOAD16_BYTE( "064.c5", 0x1000000, 0x200000, CRC(8e34a9f4) SHA1(67b839b426ef3fad0a85d951fdd44c0a45c55226) )
	ROM_LOAD16_BYTE( "064.c6", 0x1000001, 0x200000, CRC(a43e4766) SHA1(54f282f2b1ff2934cca7acbb4386a2b99a29df3a) )
	ROM_LOAD16_BYTE( "064.c7", 0x1800000, 0x200000, CRC(59d97215) SHA1(85a960dc7f364df13ee0c2f99a4c53aefb081486) )
	ROM_LOAD16_BYTE( "064.c8", 0x1800001, 0x200000, CRC(fc092367) SHA1(69ff4ae909dd857de3ca8645d63f8b4bde117448) )
ROM_END

  /***********
 World Heroes
***************/

ROM_START( wh1hack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "053_hack01.p1", 0x000000, 0x080000, CRC(a7b49883) SHA1(c6dfe2326e66afe4b0eb670ce15a36b4f36b7bc8) )
	ROM_LOAD16_WORD_SWAP( "053.p2", 0x080000, 0x080000, CRC(0e33e8a3) SHA1(4b7086edb504f3c30529d51ba8f453d48eba5164) )

	NEO_SFIX_128K( "053.s1", CRC(8c2c2d6b) SHA1(87fa79611c6f8886dcc8766814829c669c65b40f) )

	NEO_BIOS_AUDIO_128K( "053.m1", CRC(1bd9d04b) SHA1(65cd7b002123ed1a3111e3d942608d0082799ff3) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "053.v2", 0x000000, 0x200000, CRC(a68df485) SHA1(007fa715423fba72c899cd3db3f4bec13281cf7a) )
	ROM_LOAD( "053.v4", 0x200000, 0x100000, CRC(7bea8f66) SHA1(428e8721bd87f7faa756adb1e12672219be46c1d) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "053.c1", 0x000000, 0x100000, CRC(85eb5bce) SHA1(3d03d29296ca6e6b5106aac4aaeec9d4b4ed1313) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "053.c2", 0x000001, 0x100000, CRC(ec93b048) SHA1(d4159210df94e259f874a4671d271ec27be13451) )
	ROM_CONTINUE( 0x400001, 0x100000 )
	ROM_LOAD16_BYTE( "053.c3", 0x200000, 0x100000, CRC(0dd64965) SHA1(e97b3b8a461da5e8861b3dfdacb25e007ced37a1) )
	ROM_LOAD16_BYTE( "053.c4", 0x200001, 0x100000, CRC(9270d954) SHA1(a2ef909868f6b06cdcc22a63ddf6c96be12b999c) )
ROM_END

ROM_START( wh1hack02 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "053_hack02.p1", 0x000000, 0x080000, CRC(d3e22c9a) SHA1(4d7dd9686dc2637406c95f6b2ad9ee8c296a656e) )
	ROM_LOAD16_WORD_SWAP( "053.p2", 0x080000, 0x080000, CRC(0e33e8a3) SHA1(4b7086edb504f3c30529d51ba8f453d48eba5164) )

	NEO_SFIX_128K( "053.s1", CRC(8c2c2d6b) SHA1(87fa79611c6f8886dcc8766814829c669c65b40f) )

	NEO_BIOS_AUDIO_128K( "053.m1", CRC(1bd9d04b) SHA1(65cd7b002123ed1a3111e3d942608d0082799ff3) )

	ROM_REGION( 0x300000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "053.v2", 0x000000, 0x200000, CRC(a68df485) SHA1(007fa715423fba72c899cd3db3f4bec13281cf7a) )
	ROM_LOAD( "053.v4", 0x200000, 0x100000, CRC(7bea8f66) SHA1(428e8721bd87f7faa756adb1e12672219be46c1d) )

	ROM_REGION( 0x600000, "sprites", 0 )
	ROM_LOAD16_BYTE( "053.c1", 0x000000, 0x100000, CRC(85eb5bce) SHA1(3d03d29296ca6e6b5106aac4aaeec9d4b4ed1313) )
	ROM_CONTINUE( 0x400000, 0x100000 )
	ROM_LOAD16_BYTE( "053.c2", 0x000001, 0x100000, CRC(ec93b048) SHA1(d4159210df94e259f874a4671d271ec27be13451) )
	ROM_CONTINUE( 0x400001, 0x100000 )
	ROM_LOAD16_BYTE( "053.c3", 0x200000, 0x100000, CRC(0dd64965) SHA1(e97b3b8a461da5e8861b3dfdacb25e007ced37a1) )
	ROM_LOAD16_BYTE( "053.c4", 0x200001, 0x100000, CRC(9270d954) SHA1(a2ef909868f6b06cdcc22a63ddf6c96be12b999c) )
ROM_END

/****************
  World Heroes 2
 *****************/

ROM_START( wh2hack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "057_hack01.p1", 0x100000, 0x100000, CRC(6e6a0077) SHA1(a0a0eca1aff5dec55f863b1cb1f67bbe4ac3530a) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "057.s1", CRC(fcaeb3a4) SHA1(1f3f85e38b8552333261c04ae5af0d6e3b310622) )

	NEO_BIOS_AUDIO_128K( "057.m1", CRC(8fa3bc77) SHA1(982f92978671e4ee66630948e6bb7565b37b5dc0) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "057.v1", 0x000000, 0x200000, CRC(8877e301) SHA1(1bab988d74ea8fd12db201c257ec844622cf5f4e) )
	ROM_LOAD( "057.v2", 0x200000, 0x200000, CRC(c1317ff4) SHA1(4c28b2b5998abaeaa5143f2f3a9ba52c6041f4f3) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "057.c1", 0x000000, 0x200000, CRC(21c6bb91) SHA1(a2c17d0c91dd59528d8fa7fe110af8b20b25ff99) )
	ROM_LOAD16_BYTE( "057.c2", 0x000001, 0x200000, CRC(a3999925) SHA1(0ee861a77850d378d03c1bf00b9692abd860c759) )
	ROM_LOAD16_BYTE( "057.c3", 0x400000, 0x200000, CRC(b725a219) SHA1(4857687d156a9150a69b97d2729245a51c144a0c) )
	ROM_LOAD16_BYTE( "057.c4", 0x400001, 0x200000, CRC(8d96425e) SHA1(0f79c868a6a33ad25e38d842f30ec4440d809033) )
	ROM_LOAD16_BYTE( "057.c5", 0x800000, 0x200000, CRC(b20354af) SHA1(da7609fd467f2f4d71d92970f438a04d11ab1cc1) )
	ROM_LOAD16_BYTE( "057.c6", 0x800001, 0x200000, CRC(b13d1de3) SHA1(7d749c23a33d90fe50279e884540d71cf1aaaa6b) )
ROM_END

ROM_START( wh2hack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "057_hack02.p1", 0x100000, 0x100000, CRC(0cfa0f77) SHA1(c4837c18ac5a524266e2ffa33f9ce1f411a6cb35) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "057.s1", CRC(fcaeb3a4) SHA1(1f3f85e38b8552333261c04ae5af0d6e3b310622) )

	NEO_BIOS_AUDIO_128K( "057.m1", CRC(8fa3bc77) SHA1(982f92978671e4ee66630948e6bb7565b37b5dc0) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "057.v1", 0x000000, 0x200000, CRC(8877e301) SHA1(1bab988d74ea8fd12db201c257ec844622cf5f4e) )
	ROM_LOAD( "057.v2", 0x200000, 0x200000, CRC(c1317ff4) SHA1(4c28b2b5998abaeaa5143f2f3a9ba52c6041f4f3) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "057.c1", 0x000000, 0x200000, CRC(21c6bb91) SHA1(a2c17d0c91dd59528d8fa7fe110af8b20b25ff99) )
	ROM_LOAD16_BYTE( "057.c2", 0x000001, 0x200000, CRC(a3999925) SHA1(0ee861a77850d378d03c1bf00b9692abd860c759) )
	ROM_LOAD16_BYTE( "057.c3", 0x400000, 0x200000, CRC(b725a219) SHA1(4857687d156a9150a69b97d2729245a51c144a0c) )
	ROM_LOAD16_BYTE( "057.c4", 0x400001, 0x200000, CRC(8d96425e) SHA1(0f79c868a6a33ad25e38d842f30ec4440d809033) )
	ROM_LOAD16_BYTE( "057.c5", 0x800000, 0x200000, CRC(b20354af) SHA1(da7609fd467f2f4d71d92970f438a04d11ab1cc1) )
	ROM_LOAD16_BYTE( "057.c6", 0x800001, 0x200000, CRC(b13d1de3) SHA1(7d749c23a33d90fe50279e884540d71cf1aaaa6b) )
ROM_END

ROM_START( wh2hack03 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "057_hack03.p1", 0x100000, 0x100000, CRC(fbff72e2) SHA1(dda35f1440142c5acf488dd374c30263c5d8e6fc) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "057.s1", CRC(fcaeb3a4) SHA1(1f3f85e38b8552333261c04ae5af0d6e3b310622) )

	NEO_BIOS_AUDIO_128K( "057.m1", CRC(8fa3bc77) SHA1(982f92978671e4ee66630948e6bb7565b37b5dc0) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "057.v1", 0x000000, 0x200000, CRC(8877e301) SHA1(1bab988d74ea8fd12db201c257ec844622cf5f4e) )
	ROM_LOAD( "057.v2", 0x200000, 0x200000, CRC(c1317ff4) SHA1(4c28b2b5998abaeaa5143f2f3a9ba52c6041f4f3) )

	ROM_REGION( 0xc00000, "sprites", 0 )
	ROM_LOAD16_BYTE( "057.c1", 0x000000, 0x200000, CRC(21c6bb91) SHA1(a2c17d0c91dd59528d8fa7fe110af8b20b25ff99) )
	ROM_LOAD16_BYTE( "057.c2", 0x000001, 0x200000, CRC(a3999925) SHA1(0ee861a77850d378d03c1bf00b9692abd860c759) )
	ROM_LOAD16_BYTE( "057.c3", 0x400000, 0x200000, CRC(b725a219) SHA1(4857687d156a9150a69b97d2729245a51c144a0c) )
	ROM_LOAD16_BYTE( "057.c4", 0x400001, 0x200000, CRC(8d96425e) SHA1(0f79c868a6a33ad25e38d842f30ec4440d809033) )
	ROM_LOAD16_BYTE( "057.c5", 0x800000, 0x200000, CRC(b20354af) SHA1(da7609fd467f2f4d71d92970f438a04d11ab1cc1) )
	ROM_LOAD16_BYTE( "057.c6", 0x800001, 0x200000, CRC(b13d1de3) SHA1(7d749c23a33d90fe50279e884540d71cf1aaaa6b) )
ROM_END

/*******************
  World Heroes 2 Jet
 ********************/

ROM_START( wh2jhack01 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "064j_hack01.p1", 0x100000, 0x100000, CRC(14d5f4af) SHA1(62de8c90758baef92bd8174ced32c75c0e7cd622) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "064.s1", CRC(2a03998a) SHA1(5e33f469982f12d4622a06d323a345f192bf88e6) )

	NEO_BIOS_AUDIO_128K( "064.m1", CRC(d2eec9d3) SHA1(09478787045f1448d19d064dd3d540d1741fd619) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "064.v1", 0x000000, 0x200000, CRC(aa277109) SHA1(35c22b15bb0a4d0ab118cb22a2d450d03995a17c) )
	ROM_LOAD( "064.v2", 0x200000, 0x200000, CRC(b6527edd) SHA1(2bcf5bfa6e117cf4a3728a5e5f5771313c93f22a) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "064ja.c1", 0x000000, 0x400000, CRC(771a6365) SHA1(646bdf3a79819fbca98f513c172b9e9500c073be) )
	ROM_LOAD16_BYTE( "064ja.c2", 0x000001, 0x400000, CRC(e7863a05) SHA1(39d34119137216f914b8b8b7b3e622a6c210f130) )
	ROM_LOAD16_BYTE( "064ja.c3", 0x800000, 0x400000, CRC(64594ed4) SHA1(982dbb932c0cb2eb9292bf19c463fd618c8180f3) )
	ROM_LOAD16_BYTE( "064ja.c4", 0x800001, 0x400000, CRC(6e385398) SHA1(e74a0e72876bff8766152b76e5d94f2958b03cf2) )
ROM_END

ROM_START( wh2jhack02 )
	ROM_REGION( 0x200000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "064j_hack02.p1", 0x100000, 0x100000, CRC(069449e9) SHA1(9044611492509ead78059f78d9e10e18d999389a) )
	ROM_CONTINUE( 0x000000, 0x100000 )

	NEO_SFIX_128K( "064.s1", CRC(2a03998a) SHA1(5e33f469982f12d4622a06d323a345f192bf88e6) )

	NEO_BIOS_AUDIO_128K( "064.m1", CRC(d2eec9d3) SHA1(09478787045f1448d19d064dd3d540d1741fd619) )

	ROM_REGION( 0x400000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "064.v1", 0x000000, 0x200000, CRC(aa277109) SHA1(35c22b15bb0a4d0ab118cb22a2d450d03995a17c) )
	ROM_LOAD( "064.v2", 0x200000, 0x200000, CRC(b6527edd) SHA1(2bcf5bfa6e117cf4a3728a5e5f5771313c93f22a) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "064.c1", 0x000000, 0x200000, CRC(2ec87cea) SHA1(e713ec7839a7665edee6ee3f82a6e530b3b4bd7c) )
	ROM_LOAD16_BYTE( "064.c2", 0x000001, 0x200000, CRC(526b81ab) SHA1(b5f0a2f04489539ed6b9d0810b12787356c64b23) )
	ROM_LOAD16_BYTE( "064.c3", 0x400000, 0x200000, CRC(436d1b31) SHA1(059776d77b91377ed0bcfc278802d659c917fc0f) )
	ROM_LOAD16_BYTE( "064.c4", 0x400001, 0x200000, CRC(f9c8dd26) SHA1(25a9eea1d49b21b4a988beb32c25bf2f7796f227) )
	ROM_LOAD16_BYTE( "064.c5", 0x800000, 0x200000, CRC(8e34a9f4) SHA1(67b839b426ef3fad0a85d951fdd44c0a45c55226) )
	ROM_LOAD16_BYTE( "064.c6", 0x800001, 0x200000, CRC(a43e4766) SHA1(54f282f2b1ff2934cca7acbb4386a2b99a29df3a) )
	ROM_LOAD16_BYTE( "064.c7", 0xc00000, 0x200000, CRC(59d97215) SHA1(85a960dc7f364df13ee0c2f99a4c53aefb081486) )
	ROM_LOAD16_BYTE( "064.c8", 0xc00001, 0x200000, CRC(fc092367) SHA1(69ff4ae909dd857de3ca8645d63f8b4bde117448) )
ROM_END

/*********
 ZinTricK
***********/

ROM_START( zintrckbhack01 )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "211h.p1", 0x000000, 0x100000, CRC(328af665) SHA1(ece74116955ecb9ec40ac90913d496836ecc82cf) )

	NEO_SFIX_128K( "zin-s1.bin", CRC(a7ab0e81) SHA1(f0649819b96cea79b05411e0b15c8edc677d79ba) )

	NEO_BIOS_AUDIO_128K( "zin-m1.bin", CRC(fd9627ca) SHA1(b640c1f1ff466f734bb1cb5d7b589cb7e8a55346) )

	ROM_REGION( 0x200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "zin-v1.bin", 0x000000, 0x200000, CRC(c09f74f1) SHA1(d0b56a780a6eba85ff092240b1f1cc6718f17c21) )

	ROM_REGION( 0x400000, "sprites", 0 )
	ROM_LOAD16_BYTE( "zin-c1.bin", 0x000000, 0x200000, CRC(76aee189) SHA1(ad6929804c5b9a59aa609e6baebc6aa37e858a47) )
	ROM_LOAD16_BYTE( "zin-c2.bin", 0x000001, 0x200000, CRC(844ed4b3) SHA1(fb7cd057bdc6cbe8b78097dd124118bae7402256) )
ROM_END


/********
 Zupapa!
**********/

ROM_START( zupapad )
	ROM_REGION( 0x100000, "maincpu", 0 )
	ROM_LOAD16_WORD_SWAP( "070.p1", 0x000000, 0x100000, CRC(5a96203e) SHA1(49cddec9ca6cc51e5ecf8a34e447a23e1f8a15a1) )

	NEO_SFIX_128K( "070d.s1", CRC(307967ba) SHA1(b91430d0fb08ca6e0d63ded42016fd2a5cd9728b) )

	NEO_BIOS_AUDIO_128K( "070.m1", CRC(5a3b3191) SHA1(fa9a9930e18c64e598841fb344c4471d3d2c1964) )

	ROM_REGION( 0x0200000, "ymsnd:adpcma", 0 )
	ROM_LOAD( "070.v1", 0x000000, 0x200000, CRC(d3a7e1ff) SHA1(4a4a227e10f4af58168f6c26011ea1d414253f92) )

	ROM_REGION( 0x1000000, "sprites", 0 )
	ROM_LOAD16_BYTE( "070d.c1", 0x0000000, 0x200000, CRC(2bf91ba1) SHA1(5eecbc5ec2c493d2ffd0e57012e0808d4349871f) )
	ROM_LOAD16_BYTE( "070d.c2", 0x0000001, 0x200000, CRC(8700a5b8) SHA1(bc1ff7e55309120c0ca2d8800b7b003dc3c5ed65) )
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
// Andro Dunos
GAME( 1992, androdunhack01,    androdun, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Andro Dunos (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Art of Fighting 1
GAME( 1992, aofhack01,         aof,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Art of Fighting (Enable hidden characters)", MACHINE_SUPPORTS_SAVE )
// Art of Fighting 2
GAME( 1994, aof2hack01,        aof2,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Art of Fighting 2 (Enable hidden characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, aof2hack02,        aof2,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Art of Fighting 2 (Boss With Simple Attack Edition 2018-12-16)", MACHINE_SUPPORTS_SAVE )
// Art of Fighting 3
GAME( 1996, aof3hack01,        aof3,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Art of Fighting 3 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Bang Bead
GAME( 2000, bangbeadd,         bangbead, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Bang Bead (decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, bangbeadp,         bangbead, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Visco",          "Bang Bead (prototype)", MACHINE_SUPPORTS_SAVE )
// Crouching Tiger Hidden Dragon
GAME( 2016, ct2k3hack01,       kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_cthd2003,  ROT0, "hack",           "Crouching Tiger Hidden Dragon 2003 (Simplified 2016-03-15)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2004, ct2k3sps,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_cthd2003,  ROT0, "bootleg",        "Crouching Tiger Hidden Dragon 2003 Plus (The King of Fighters 2001 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, ct2k3spd,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Crouching Tiger Hidden Dragon 2003 Super Plus (The King of Fighters 2001 bootleg / decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, cthd2k3d,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Crouching Tiger Hidden Dragon 2003 (The King of Fighters 2001 bootleg / set 1 fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, ct2k3ad,           kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Crouching Tiger Hidden Dragon 2003 (The King of Fighters 2001 bootleg / set 2 fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, cthd2k3a,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_cthd2k3a,  ROT0, "bootleg",        "Crouching Tiger Hidden Dragon 2003 (The King of Fighters 2001 bootleg, set 2)", MACHINE_SUPPORTS_SAVE )
// Blue's Journey / Raguy
GAME( 1990, bjourneyhack01,    bjourney, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Blue's Journey / Raguy (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Blazing Star
GAME( 2021, blazstarhack01,    blazstar, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "NeoCvera",       "Blazing Star (Stage Select hack)", MACHINE_SUPPORTS_SAVE )
// Breakers
GAME( 1996, breakershack01,    breakers, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Breakers (Enable Hidden Character)", MACHINE_SUPPORTS_SAVE )
// Breakers Revenge
GAME( 1998, breakrevhack01,    breakrev, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Breakers Revenge (Enable Hidden Character)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, breakrevhack02,    breakrev, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Breakers Revenge (Doomsday Heroes Strengthens Lite 2015-01-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, breakrevhack03,    breakrev, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Breakers Revenge (Boss With Simple Attack Edition 2018-07-17)", MACHINE_SUPPORTS_SAVE )
//Burning Fight
GAME( 1991, burningfhack01,    burningf, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "EEZEZY",         "Burning Fight (Time is not reduced)", MACHINE_SUPPORTS_SAVE )
//Captain Tomaday
GAME( 1999, ctomadayhack01,    ctomaday, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Captain Tomaday (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Double Dragon
GAME( 1995, doubledrhack01,    doubledr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Double Dragon (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, doubledrhack02,    doubledr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Double Dragon (Edition Language China 2015-04-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2009, doubledrhack03,    doubledr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Blackheart",     "Double Dragon (Perfectly simplified Edition 2009-09-19)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, doubledrhack04,    doubledr, gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Double Dragon (Special 2017 v1.0.0311)", MACHINE_SUPPORTS_SAVE )
// Dragon's Heaven
GAME( 200?, dragons1,          neogeo,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Face",           "Dragon's Heaven", MACHINE_SUPPORTS_SAVE )
// Eight Man
GAME( 1991, eightmanhack01,    eightman, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Blackheart",     "Eight Man (Enhanced Version 2009-08-22)", MACHINE_SUPPORTS_SAVE )
// Fatal Fury Special
GAME( 1993, fatfursphack01,    fatfursp, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Fatal Fury Special (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, fatfursphack02,    fatfursp, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "kawada7278",     "Fatal Fury Special (Easy Special Attacks 2015-03-15)", MACHINE_SUPPORTS_SAVE )
// Fatal Fury 1
GAME( 1991, fatfury1hack01,    fatfury1, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji1768",     "Fatal Fury (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Fatal Fury 2
GAME( 1992, fatfury2hack01,    fatfury2, neogeo_noslot, neogeo, neogeo_state,     init_fatfury2,  ROT0, "yumeji[2ch]",    "Fatal Fury 2 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, fatfury2hack02,    fatfury2, neogeo_noslot, neogeo, neogeo_state,     init_fatfury2,  ROT0, "lichenzhao",     "Fatal Fury 2 (The New Legendary Simplified Version 2015-03-11)", MACHINE_SUPPORTS_SAVE )
// Fatal Fury 3
GAME( 1995, fatfury3hack01,    fatfury3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Creamymami and Dodowang", "Fatal Fury 3 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, fatfury3hack02,    fatfury3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Fatal Fury 3 (Ancient Battles Resurgence 2015-03-13)", MACHINE_SUPPORTS_SAVE )
// Fight Fever
GAME( 1994, fightfevhack01,    fightfev, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Fight Fever (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Battle Flip Shot
GAME( 1998, flipshothack01,    flipshot, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Battle Flip Shot (Traduction Korean)", MACHINE_SUPPORTS_SAVE )
// Galaxy Fight
GAME( 2014, galaxyfghack01,    galaxyfg, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Galaxy Fight (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Ganryu
GAME( 1999, ganryud,           ganryu,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "bootleg",        "Ganryu (Decrypted C)", MACHINE_SUPPORTS_SAVE )
// Garou
GAME( 1999, garouhack01,       garou,    neogeo_noslot, neogeo, neogeo_state,     init_garou,     ROT0, "Ydmis",          "Garou - Mark of the Wolves (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, garouhack02,       garou,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Garou - Mark of the Wolves (Resurgence Lite 2015-03-10)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 1999, garoud,            garou,    neogeo_noslot, neogeo, neogeo_state,     init_garoud,    ROT0, "bootleg",        "Garou - Mark of the Wolves (Decrypted C)", MACHINE_SUPPORTS_SAVE )
// Voltage Fighter
GAME( 1995, gowcaizrhack01,    gowcaizr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Voltage Fighter (Add Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1995, gowcaizrhack02,    gowcaizr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Voltage Fighter (Easy Moves)", MACHINE_SUPPORTS_SAVE)
// Ghost Pilots
GAME( 2014, gpilotshack01,     gpilots,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "kawada7278",     "Ghost Pilots (Ex Super version 2014-10-24)", MACHINE_SUPPORTS_SAVE )
// Ironclad
GAME( 1996, ironcladhack01,    ironclad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Saurus",         "Ironclad (Conversion from Wii)", MACHINE_SUPPORTS_SAVE )
//Jockey Grand Prix
GAME( 2001, jockeygpd,         jockeygp, neogeo_noslot, jockeygp, neogeo_state,   init_jckeygpd,  ROT0, "bootleg",        "Jockey Grand Prix (decrypted C)", MACHINE_SUPPORTS_SAVE )
// Far East of Eden
GAME( 1995, kabukiklhack01,    kabukikl, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ydmis",          "Far East of Eden (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE ) 
// Karnov's Revenge
GAME( 1994, karnovrhack01,     karnovr,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Karnov's Revenge (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Kizuna Encounter
GAME( 1996, kizunahack01,      kizuna,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ydmis",          "Kizuna Encounter (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kizunahack02,      kizuna,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kizuna Encounter (Simplify Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '94
GAME( 1994, kof94hack01,       kof94,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Kof'94 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, kof94hack02,       kof94,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Neogeo BR team", "Kof'94 (Portuguese Edition)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, kof94hack03,       kof94,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'94 (Chinese Version)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, kof94hack04,       kof94,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'94 (Boss With Simple Attack Edition)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '95
GAME( 1995, kof95hack01,       kof95,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Creamymami",     "Kof'95 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof95hack02,       kof95,    gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Kof'95 (Special v1.0 2020-02-22)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '96
GAME( 1996, kof96hack01,       kof96,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ydmis",          "Kof'96 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof96hack02,       kof96,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "EGHT/EGCG",      "Kof'96 (The Aniversary Edition 2.3 Final Version 2020-05-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof96hack03,       kof96,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "EGHT/EGCG",      "Kof'96 (The Aniversary Edition 2.3 Final Version Simplify The Move 2020-05-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof96hack04,       kof96,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'96 (Remix Plus 2008SP 2020-02-09)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 1996, kof96ep,           kof96,    neogeo_noslot, neogeo, neogeo_state,     init_kof96ep,   ROT0, "bootleg",        "Kof'96 (bootleg / hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, kof96cn,           kof96,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Kof'96 (Chinese Edition)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '97
GAME( 1997, kof97hack01,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "kof1996",        "Kof'97 (Hidden Characters Enable)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack02,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Optimized Modified Version 2020 2020-06-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, kof97hack03,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Eddids",         "Kof'97 (Imitation Playstation final improved version 2016-10-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack04,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (10th Anniversary Build 2.1.1811 2020-08-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack05,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (10th Anniversary Simplified Build 2.1.1811 2020-08-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kof97hack06,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Combo Training Version 2018-05-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, kof97hack07,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Final Battle)", MACHINE_SUPPORTS_SAVE )
GAME( 2010, kof97hack08,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Random Combo 2010-02-07)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof97hack09,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "dream",          "Kof'97 (Iori with Cloak 2019-01-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof97hack10,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "dream",          "Kof'97 (Iori with Cloak, Moves Simplified 2019-02-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack11,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK World Buwu 83 Modified Edition Original 2020-07-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack12,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK World Buwu 83 Modified Edition Simplify 2020-07-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack13,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK World Buwu 85 Modified Edition Original 2020-07-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack14,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK World Buwu 85 Modified Edition Simplify 2020-07-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack15,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK Clone Zero Modified Version Original 2020-07-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack16,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (YK Clone Zero Modified Version Simplify 2020-07-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kof97hack17,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Rugal Edition 2018-07-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack18,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (14 Windbreaker Eight Gods Second Impact Original 2020-06-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack19,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (14 Windbreaker Eight Gods Second Impact Simplified 2020-06-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack20,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (SP Special Edition 2015 2020-06-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack21,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Ultimate Battle Plus Original 2019 2020-07-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack22,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Ultimate Battle Plus Simplify 2019 2020-07-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack23,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Angry Athena-Snake Slayer War Original 2020-09-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack24,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Angry Athena-Snake Slayer War Simplified 2020-09-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof97hack25,       kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'97 (Combo Simplified Edition 2020-03-10)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2003, kof97pla,          kof97,    neogeo_noslot, neogeo, neogeo_state,     init_kof97pla,  ROT0, "bootleg",        "Kof'97 (Plus)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '98
GAME( 1998, kof98hack01,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "FoxUU",          "Kof'98 (Chinese Translation)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, kof98hack02,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Arkatrad",       "Kof'98 (Translation French 2004)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, kof98hack03,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "EGHT",           "Kof'98 (Anniversary Edition 2016-08-27)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 1998, kof98hack04,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'98 (Anniversary Edition EGHT hack)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof98hack05,       kof98,    gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'98 (Combo Plus 2020-02-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, kof98hack06,       kof98,    gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Kof'98 (Plus Final Edition 2019-09-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof98hack07,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'98 (Ultimate Match)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof98hack08,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ivex",           "Kof'98 (ECK Dragon Edition 2020-04-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof98hack09,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ivex",           "Kof'98 (Combo 2019-07-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof98hack10,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ivex",           "Kof'98 (Combo Simplify 2019-07-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof98hack11,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'98 (BC Combo Version Simplified 2020-03-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof98hack12,       kof98,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'98 (C Dragon Edition Simplified 2020-03-16)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '99
GAME( 1999, kof99hack01,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_kof99,     ROT0, "Dodowang",       "Kof'99 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kof99hack02,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yashional",      "Kof'99 (Evolution Ultra Remix 2016-01-04)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kof99hack03,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "FCHT",           "Kof'99 (Remix Pro Final)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof99hack04,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'99 (Remix Pro Simplified 2020-03-15)", MACHINE_SUPPORTS_SAVE )
GAME( 1999, kof99hack05,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "FCHT",           "Kof'99 (Summer Revolution)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof99hack06,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yashional",      "Kof'99 (Anniversary Edition 2020-04-07)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof99hack07,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'99 (Imitation PS Enhanced Edition Original 2020-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof99hack08,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'99 (Imitation PS Enhanced Edition Simplified 2020-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof99hack09,       kof99,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof'99 (Evolution Ultimate Edition 2020-03-03)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 1999, kof99nd,           kof99,    neogeo_noslot, neogeo, neogeo_state,     init_kof99,     ROT0, "bootleg",        "Kof'99 (Non encrypted P,decrypted C)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '2000
GAME( 2000, kof2000hack01,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "0 Day-S",        "Kof2000 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2000hack02,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "hack",           "Kof2000 (OTC Original 2020-06-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2000hack03,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "hack",           "Kof2000 (OTC Simplified 2020-06-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, kof2000hack04,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "hack",           "Kof2000 (Like PS2 Version V1.0 2013)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2000hack05,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "hack",           "Kof2000 (Resurgence Simplify 2020-07-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, kof2000hack06,     kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "hack",           "Kof2000 (Special Edition Final Version 2021)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2000, kof2000d,          kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "bootleg",        "Kof2000 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, kof2knd,           kof2000,  neogeo_noslot, neogeo, neogeo_state,     init_kof2000,   ROT0, "bootleg",        "Kof2000 (Non encrypted P,decrypted C)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '2001
GAME( 2001, kof2001hack01,     kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_kof2001,   ROT0, "hack",           "Kof2001 (Enable Hidden Characters v3)" , MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof2001hack02,     kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2001 (PS2 Krizalid Edition 2019-12-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2001hack03,     kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2001 (Plus Blue Simplified Edition 2020-02-26)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2001, kof2001d,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_kof2001,    ROT0, "bootleg",       "Kof2001 (NGM-262)(decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, kof2001n,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "bootleg",       "Kof2001 (Fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2k1pls,         kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_kof2001,    ROT0, "bootleg",       "Kof2001 Plus (Set 1, bootleg / hack)", MACHINE_SUPPORTS_SAVE ) //Plus Plus + Resurgence plus 3 Simplified BOSS
GAME( 2003, kof2k1pa,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_kf2k1pa,    ROT0, "bootleg",       "Kof2001 Plus (set 2, bootleg / hack)", MACHINE_SUPPORTS_SAVE ) //Plus Plus + Resurgence plus 3 Simplified BOSS
GAME( 2016, kof2k1nd,          kof2001,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "bootleg",       "Kof2001 (decrypted C)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '2002
GAME( 2005, kof2002hack01,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002,    ROT0, "Eddids",        "Kof2002 (Enable Hidden Characters)" , MACHINE_SUPPORTS_SAVE )
GAME( 2012, kof2002hack02,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "KOF-ON Team",   "Kof2002 (Omega v0.9)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kof2002hack03,     kof2002,  gsc,           neogeo, neogeo_state,     init_neogeo,     ROT0, "EGCG",          "Kof2002 (PlayStation 2 Hack Ver.1.0 2018-12-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, kof2002hack04,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "hack",          "Kof2002 (PlayStation 2 Original 2018-04-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2002hack05,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "hack",          "Kof2002 (PlayStation 2 Simplified 2020-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2002hack06,     kof2002,  gsc,           neogeo, neogeo_state,     init_kof2k2pl17, ROT0, "hack",          "Kof2002 (Come Back 2017 2020-10-22)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2002hack07,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "hack",          "Kof2002 (Third Edition of the Kingdom of Gods 2020-06-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2002hack08,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "Jason / FCHT",  "Kof2002 (Remix Ultra v3.5)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof2002hack09,     kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,     ROT0, "hack",          "Kof2002 (All Mix Version 2018-04-22)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2017, kof2002hack10,     kof2002,  gsc,           neogeo, neogeo_state,     init_kof2k2pl17, ROT0, "GSC2007",       "Kof2002 Plus 2017-12-25" , MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2002, kof2k2pls,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002,    ROT0, "bootleg",       "Kof2002 Plus (set 1 bootleg)" , MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2k2pla,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002,    ROT0, "bootleg",       "Kof2002 Plus (Set 2 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2k2plb,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002,    ROT0, "bootleg",       "Kof2002 Plus (Set 3 bootleg)" , MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2k2plc,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002s20, ROT0, "bootleg",       "Kof2002 Plus (Set 4 bootleg)" , MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2002, kof2002d,          kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof2002,    ROT0, "Eolith / Playmore", "Kof2002 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
// The King of Fighters '2003
GAME( 2003, kof2003hack01,     kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_kof2003,   ROT0, "Neogeo BR team", "Kof2003 (Portuguese Brazilian)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, kof2003hack02,     kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_kof2003,   ROT0, "hack",           "Kof2003 (PS2 Full Character Edition Simplified 2019-09-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2003hack03,     kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2004 EX Ultra Plus (Fengyun Plus Simplified 2020-02-29)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2003, kof2003d,          kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_kof2003,   ROT0, "hack",           "Kof2003 (Decrypted P&C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2003f,          kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2003 (Fully Decrypted - Fixed)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2k3d,           kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_kof2003,   ROT0, "hack",           "Kof2003 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2k3fd,          kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2003 (Bootleg, fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2k3hd,          kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_kof2k3hd,  ROT0, "hack",           "Kof2003 (NGH-2710)(decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2k3nd,          kof2003,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof2003 (Fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kof2k3pcd,         kf2k3pcb, neogeo_noslot, neogeo, neogeo_state,     init_kof2k3pcd, ROT0, "hack",           "Kof2003 (Japan, JAMMA PCB / decrypted C&Bios)", MACHINE_SUPPORTS_SAVE )
// King of Gladiator
GAME( 2020, koghack01,         kof97,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "King of Gladiator (Resurgence Plus Dog Slaughter Edition 2020-01-23)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 1997, kogd,              kof97,    hbmame_kog,    neogeo, neogeo_state,     init_kogd,      ROT0, "bootleg",        "King of Gladiator (The King of Fighters '97 bootleg)(fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 1997, kogplus,           kof97,    neogeo_kog,    neogeo, neogeo_state,     init_kog,       ROT0, "bootleg",        "King of Gladiator Plus (set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1997, kogplus1,          kof97,    hbmame_kog,    neogeo, neogeo_state,     init_kogd,      ROT0, "bootleg",        "King of Gladiator Plus (set 2)", MACHINE_SUPPORTS_SAVE )
// 10th Anniversary
GAME( 2015, kof10thephack01,   kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kf10thep,  ROT0, "hack",           "Kof 10th Anniversary Extra Plus (Simplify Edition)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2005, kof10thd,          kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof 10th Anniversary (The King of Fighters 2002 bootleg / fully decrypted)", MACHINE_SUPPORTS_SAVE )
// Kof 10th Anniversary 2005 Uniques
GAME( 2020, kf2k5unihack01,    kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof 10th Anniversary 2005 Unique (Edition Optimized Simplify 2019 2020-11-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kf2k5unihack02,    kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof 10th Anniversary 2005 Unique (Omega Edition Simplified 2020-02-27)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2008, kof2k8uni,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Kof 10th Anniversary 2008 Unique (The King of Fighters 2002 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2005, kof10thu,          kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_kof10thu,  ROT0, "bootleg",        "Kof 10th Anniversary 2005 Unique (bootleg, set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2005, kof10thud,         kof2002,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Kof 10th Anniversary 2005 Unique (The King of Fighters 2002 bootleg)(decrypted)", MACHINE_SUPPORTS_SAVE )
// Kof Special Edition 2004
GAME( 2020, kof2k4sehack01,    kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof Special Edition 2004 (Omega Edition 2020-01-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, kof2k4sehack02,    kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Kof Special Edition 2004 (Resurgence Plus Enhanced 2020-01-23)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2004, kof2k4mp,          kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_kof2k4se,  ROT0, "bootleg",        "Kof Special Edition 2004 Magic Plus (The King of Fighters 2002 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, kof2k4mp2,         kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_kof2k4se,  ROT0, "bootleg",        "Kof Special Edition 2004 Magic Plus 2 (The King of Fighters 2002 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, kof2k4pls,         kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_kof2k4pls, ROT0, "bootleg",        "Kof Special Edition 2004 Plus (Set 1)(The King of Fighters 2002 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, kof2k4spl,         kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_kof2k4se,  ROT0, "bootleg",        "Kof Special Edition 2004 Plus (Set 2)(The King of Fighters 2002 bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, kof2k4se1,         kof2k4se, neogeo_noslot, neogeo, neogeo_state,     init_kof2k4se,  ROT0, "bootleg",        "Kof Special Edition 2004 (Bootleg)", MACHINE_SUPPORTS_SAVE )
// Lansquenet 2004
GAME( 2004, lans2k4d,          shocktr2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Lansquenet 2004 (Shock Troopers - 2nd Squad Bootleg / fully decrypted)", MACHINE_SUPPORTS_SAVE )
// Last Blade
GAME( 1997, lastbladhack01,    lastblad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Creamymami",     "The Last Blade (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1997, lastbladhack02,    lastblad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "The Last Blade (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 1997, lastbladhack03,    lastblad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "The Last Blade (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, lastbladhack04,    lastblad, gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "The Last Blade (Special v1.1)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, lastbladhack05,    lastblad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "The Last Blade (Ex Super Version 2018-03-20)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 1997, lba,               lastblad, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "SNK",            "The Last Blade / Bakumatsu Roman - Gekka no Kenshi (NGM-2340, alternate board)", MACHINE_SUPPORTS_SAVE )
// The Last Blade 2
GAME( 1998, lastbld2hack01,    lastbld2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Eddids[FCHT]",   "The Last Blade 2 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, lastbld2hack02,    lastbld2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "The Last Blade 2 (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 1998, lastbld2hack03,    lastbld2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "The Last Blade 2 (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, lastbld2hack04,    lastbld2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GCS2007",        "The Last Blade 2 (Team Edition Ver 1.1 2018-05-09)", MACHINE_SUPPORTS_SAVE )
// Last Resort
GAME( 1992, lresorthack01,     lresort,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Last Resort (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Magical Drop III
GAME( 2018, magdrop3hack01,    magdrop3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Magical Drop III (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, magdrop3hack02,    magdrop3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Shyma.X",        "Magical Drop III (Translation French 2014-07-29)", MACHINE_SUPPORTS_SAVE )
// Magician Lord
GAME( 1990, maglordhack01,     maglord,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Magician Lord (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Matrimelee
GAME( 2002, matrimhack01,      matrim,   neogeo_noslot, neogeo, neogeo_state,     init_matrim,    ROT0, "Creamymami",     "Matrimelee (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, matrimhack02,      matrim,   neogeo_noslot, neogeo, neogeo_state,     init_matrim,    ROT0, "hack",           "Matrimelee (Simplification Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2002, matrimd,           matrim,   neogeo_noslot, neogeo, neogeo_state,     init_matrimd,   ROT0, "bootleg",        "Matrimelee (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, matrima,           matrim,   neogeo_noslot, neogeo, neogeo_state,     init_matrima,   ROT0, "bootleg",        "Matrimelee (Bootleg/Fixed)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, matrimeh,          matrim,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Matrimelee (Bootleg, Hero Hack)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, matrimnd,          matrim,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Matrimelee (Bootleg)", MACHINE_SUPPORTS_SAVE )
// Money Puzzle Exchanger
GAME( 2005, miexchnghack01,    miexchng, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Arkatrad",       "Money Puzzle Exchanger (Translation French)", MACHINE_SUPPORTS_SAVE )
// Metal Slug
GAME( 2016, mslughacks01,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",           "Metal Slug (Change Weapon 2016-03-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslughacks02,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",           "Metal Slug (Jump In Mid Air 2017-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslughacks03,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",           "Metal Slug (War Chariot 2016-03-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslughacks04,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",         "Metal Slug (Multi-Function Version 2018-04-30)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslughacks05,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "I love Nimes",  "Metal Slug (Burst Enhanced Edition 2021-09-19)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslughacks06,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",          "Metal Slug (Easy Mode Difficulty 2018-11-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslughacks07,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "KofKill, Wang Hua, Czk", "Metal Slug (Unity Time 2021-02-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslughacks08,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",           "Metal Slug (Stage Select 2016-10-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslughacks09,      mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",          "Metal Slug (Random Items Explosives 2021-06-08)", MACHINE_SUPPORTS_SAVE )
// Decrypted And Bootleg
GAME( 2005, msboot,            mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",       "Metal Slug (Bootleg)", MACHINE_SUPPORTS_SAVE )
//Version Remastering
GAME( 2021, msluger01,         mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                      "Metal Slug (Multi-Function Version 2018-04-30)(Items Explosives Edition Remastering 2022-10-18)", MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2021, mslugla01,         mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                 "Metal Slug (Multi-Function Version 2018-04-30)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2021, mslugrmxe0s01,     mslug,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, DDJ And I love Nimes)",            "Metal Slug (Multi-Function Version 2018-04-30)(Revised Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
// Metal Slug 2
GAME( 2014, mslug2hacks01,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Enemies Resetting Version 2014-09-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug2hacks02,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Generation 1V2 2019-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug2hacks03,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",             "Metal Slug 2 (Stage Select 2016-10-19)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mslug2hacks04,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Shyma.X",         "Metal Slug 2 (Traduction French Version 2004)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, mslug2hacks05,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "System11",        "Metal Slug 2 (Turbo) ", MACHINE_SUPPORTS_SAVE )
GAME( 2008, mslug2hacks06,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",            "Metal Slug 2 (Hybrid bullets version 2008-12-04)", MACHINE_NOT_WORKING | MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug2hacks07,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",             "Metal Slug 2 (Jump In Mid Air 2017-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug2hacks08,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",             "Metal Slug 2 (Ward Charior 2018-01-25)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug2hacks09,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",             "Metal Slug 2 (Change Weapon And Mummy 2018-08-25)", MACHINE_SUPPORTS_SAVE ) //The game crashes when you press the 1 + D button.
GAME( 2021, mslug2hacks10,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "I love Nimes",    "Metal Slug 2 (Burst Enhanced Edition 2021-04-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug2hacks11,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Multi-Function Version 2017-05-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug2hacks12,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Survival Version 2018-07-31)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2006, mslug2hacks13,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",             "Metal Slug 2 (Violence Edition 2016-03-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2hacks14,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "phengzheng And Gaston90",  "Metal Slug 2 (Enemies Resetting Extraction Green Turbo 2021-09-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug2hacks15,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",            "Metal Slug 2 (Easy Mode Difficulty 2018-11-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, mslug2hacks16,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "lb70",            "Metal Slug 2 (Crazy Fire Edition 2013-02-07)", MACHINE_SUPPORTS_SAVE )
//GAME( 2021, mslug2hacks17,     mslug2,   neogeo_noslot, neogeo, neogeo_state,   init_neogeo,    ROT0, "CZXINC",          "Metal Slug 2 (Friendly Fire FC2 2021-04-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2hacks18,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "KofKill, Wang Hua, Czk",  "Metal Slug 2 (Unity Time 2021-01-22)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug2hacks19,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",            "Metal Slug 2 (Removal Of Background Music 2018-08-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2hacks20,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Friendly Fire Edition 2021-04-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2hacks21,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",           "Metal Slug 2 (Early Summer Starry Sky 2022-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2hacks22,     mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "phengzheng",      "Metal Slug 2 (Extraction Green Turbo 2021-09-16)", MACHINE_SUPPORTS_SAVE )
//Version Remastering
GAME( 2022, mslug2er01,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, System11)",                                              "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Improved Edition Remastering 2022-10-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2er02,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, System11)",                                              "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Multi-Function Remix Standard Edition Remastering 2022-10-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2er03,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, System11)",                                              "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Survival Version Remix Standard Edition Remastering 2022-10-17)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
//All Apocalyptic Time Edition
GAME( 2021, mslug2at01,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Gaston90",                                                                              "Metal Slug 2 Turbo (Apocalyptic Time Ports 0.2 2021-07-17)", MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2021, mslug2la01,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Revised XR Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2la02,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Revised XR Multi-Function Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2la03,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Revised XR Survival Version Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2021, mslug2la04,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Revised XR Survival Hybrid Bullets Version Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2021, mslug2la05,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Revised XR Hybrid Bullets Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2lb01,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Improved Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2lb02,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Multi-Function Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2lb03,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Survival Version Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2022, mslug2lb04,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Survival Hybrid Bullets Version Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2022, mslug2lb05,        mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis, kof1996, System11 And I love Nimes)",                      "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Hybrid Bullets Remix Standard Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2021, mslug2rmxe0s01,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Shyma.X, kof1996, Ydmis, DDJ, System11 And I love Nimes)",        "Metal Slug 2 Turbo (Revised XR Heavy Machine Gun Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2rmxe0s02,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Shyma.X, kof1996, Ydmis, DDJ, System11 And I love Nimes)",        "Metal Slug 2 Turbo (Revised XR Multi-Function Version 2017-05-09)(Revised Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug2rmxe0s03,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Revised XR Survival Version 2018-07-31)(Revised Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2021, mslug2rmxe0s04,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Revised XR Survival Version 2018-07-31)(Hybrid Bullets Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )//To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2021, mslug2rmxe0s05,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Revised XR Hybrid Bullets Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2rmxe1s01,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Shyma.X, kof1996, Ydmis, DDJ, System11 And I love Nimes)",        "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Version Heavy Machine Gun Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2rmxe1s02,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Shyma.X, kof1996, Ydmis, DDJ, System11 And I love Nimes)",        "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Multi-Function Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug2rmxe1s03,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Survival Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE ) //To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2022, mslug2rmxe1s04,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Survival Hybrid Bullets Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE ) //To enable Mods random you need to apply a command you have to press the Stars + D button to change Stars + C.
GAME( 2022, mslug2rmxe1s05,    mslug2,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, kof1996, Ydmis, DDJ, System11 And I love Nimes)",                 "Metal Slug 2 Turbo (Enemies Resetting Version 2014-09-14)(Revised XR Hybrid Bullets Extreme MegaMods Edition 2021-11-21)", MACHINE_SUPPORTS_SAVE )
// Metal Slug 3
GAME( 2000, mslug3hacks01,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3n,   ROT0, "HappyAsr",               "Metal Slug 3 (Super D version)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3hacks02,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "I love Nimes",           "Metal Slug 3 (Burst Enhanced Edition 2022-10-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug3hacks03,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "DDJ",                    "Metal Slug 3 (Jump In Mid Air 2017-08-23)" ,  MACHINE_SUPPORTS_SAVE )
GAME( 2012, mslug3hacks04,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "ms6boot",                "Metal Slug 3 (Change Gun Version 2012-04-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, mslug3hacks05,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "LB70",                   "Metal Slug 3 (Crazy Fire Version V1.1 2014-10-04)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3hacks06,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (SE Special Edition 2022-03-31)", MACHINE_SUPPORTS_SAVE ) // It is modified by another author 
GAME( 2013, mslug3hacks07,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3,    ROT0, "GOTVG",                  "Metal Slug 3 (Green Blue Edition 2013-02-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug3hacks08,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Shop Version 2017-12-16)" , MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug3hacks09,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "DDJ",                    "Metal Slug 3 (Change Weapons 2017-05-22)", MACHINE_NOT_WORKING | MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug3hacks10,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Summon Mount Edition 2017-04-04)" , MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug3hacks11,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "GOTVG",                  "Metal Slug 3 (SE Special Edition 2019-07-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mslug3hacks12,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Shyma.X",                "Metal Slug 3 (French Traduction Version 2006)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug3hacks13,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Onimusha Samanosuke 2018-05-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug3hacks14,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "DDJ",                    "Metal Slug 3 (Change Zombie 2016-03-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2012, mslug3hacks15,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3b6,  ROT0, "Eezezy",                 "Metal Slug 3 (Violence Edition 2012-03-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug3hacks16,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "hack",                   "Metal Slug 3 (Enemy Soldiers Reset 2019-01-02)" , MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug3hacks17,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Ultimate Simplified Edition 2020-08-25)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, mslug3hacks18,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "hack",                   "Metal Slug 3 (Hybrid Bullets Version 2013-01-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug3hacks19,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "DDJ",                    "Metal Slug 3 (War Chariot 2016-02-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug3hacks20,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "hack",                   "Metal Slug 3 (The Future 2020-09-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug3hacks21,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "hack",                   "Metal Slug 3 (Enemy Remake 2017-09-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug3hacks22,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "hack",                   "Metal Slug 3 (The Last Warhead 2020-09-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3hacks23,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "GOTVG",                  "Metal Slug 3 (Unlimited Firepower 2021-03-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug3hacks24,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Enemies Resetting Version 2019-07-20)" , MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug3hacks25,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",                   "Metal Slug 3 (Random Enemies Resetting Version 2017-12-08)" , MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug3hacks26,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",                   "Metal Slug 3 (Random Enemies Resetting Version 2018-03-01)" , MACHINE_SUPPORTS_SAVE ) //(It is unknown if it is an update or a prototype )
GAME( 2020, mslug3hacks27,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "DDJ",                    "Metal Slug 3 (Super G Version 2020-12-12)" , MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug3hacks28,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",                  "Metal Slug 3 (Multi-Function Version 2019-03-05)" , MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug3hacks29,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Wanghua123",             "Metal Slug 3 (Summon Mount Slug 2017-10-09)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug3hacks30,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3,    ROT0, "GOTVG",                  "Metal Slug 3 (Generation 1V2 2019-05-11)" , MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug3hacks31,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "hack",                   "Metal Slug 3 (Easy Mode Difficulty 2018-11-16)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3hacks32,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3,    ROT0, "KofKill, Wang Hua, Czk", "Metal Slug 3 (Unity Time 2021-02-21)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3hacks33,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "hack",                   "Metal Slug 3 (Ammo Increases 2021-06-13)", MACHINE_SUPPORTS_SAVE )
//GAME( 2021, mslug3hacks34,      mslug3,   neogeo_noslot, neogeo, neogeo_state,  init_mslug3,    ROT0, "hack",                 "Metal Slug 3 (I 2021-03-17)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3hacks35,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "GOTVG",                  "Metal Slug 3 (Early Summer Starry Sky 2022-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3hacks36,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "GOTVG",                  "Metal Slug 3 (Komorebi 2022-05-11)", MACHINE_SUPPORTS_SAVE )
// Decrypted And Bootleg
GAME( 2000, mslug3d,           mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3d,   ROT0, "bootleg",                "Metal Slug 3 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mslug3hd,          mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "bootleg",                "Metal Slug 3 (Non-encrypted P, decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, mslug3nd,          mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "bootleg",                "Metal Slug 3 (Fully Decrypted C)", MACHINE_SUPPORTS_SAVE )
// Metal Slug 6" is a hack/bootleg of Metal Slug 3
GAME( 2019, mslug6hacks01,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_ms6s16,    ROT0, "XiaShuiDaoMeiRenYu",     "Metal Slug 6 (Enemies Resetting Version 2019-07-20)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug6hacks02,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_ms6s16,    ROT0, "hack",                   "Metal Slug 6 (The Last Warhead 2020-09-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug6hacks03,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_ms6s16,    ROT0, "hack",                   "Metal Slug 6 (The Future 2020-09-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug6hacks04,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_ms6s16,    ROT0, "hack",                   "Metal Slug 6 (Enemy Remake 2017-09-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug6hacks05,     mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_ms6s16,    ROT0, "hack",                   "Metal Slug 6 (Enemy Soldiers Reset 2019-01-02)", MACHINE_SUPPORTS_SAVE )
// Version Remastering
GAME( 2022, mslug3er01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #1 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #2 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #3 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #4 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #5 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er06,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #6 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug3er07,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Unknown Author)",                           "Metal Slug 3 (Enemies Resetting #7 Version Remastering 2021-10-10)(Revised Multi-Function Remastering Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2021, mslug3la01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3la02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3la03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3la04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3la05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90 And Wanghua123)",                            "Metal Slug 3 (Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lb01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lb02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lb03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lb04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lb05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lc01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lc02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lc03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lc04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lc05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3ld01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3ld02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3ld03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3ld04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3ld05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3le01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3le02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3le03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3le04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3le05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lf01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lf02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lf03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lf04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lf05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lg01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lg02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lg03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lg04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lg05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90  And Wanghua123)",                           "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lh01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-10)(Revised Multi-Function Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lh02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-10)(Revised Multi-Function Zombie Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lh03,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-10)(Revised Multi-Function Mummy Players Mode Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lh04,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, Ydmis And I love Nimes)",                      "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-10)(Revised Multi-Function Hybrid Bullets Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3lh05,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Team Remix (Gaston90 And Wanghua123)",                            "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-10)(Revised Summon Mount Remix Standard Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3li01,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, I love Nimes And Ydmis)",                      "Metal Slug 3 (Random Enemies Resetting Version 2017-12-08)(Revised Remix Standard Edition 2021-11-18)(Set 1)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3li02,        mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, I love Nimes And Ydmis)",                      "Metal Slug 3 (Random Enemies Resetting Version 2017-12-08)(Revised Remix Standard Edition 2021-11-18)(Set 2)" , MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2021, mslug3rmxe0s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe0s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe0s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe0s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe1s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe1s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe1s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe1s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #1 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe2s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe2s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe2s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe2s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #2 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe3s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe3s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe3s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe3s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #3 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe4s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe4s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe4s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe4s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #4 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe5s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe5s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe5s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe5s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #5 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe6s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe6s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe6s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe6s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #6 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe7s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-09)(Revised Multi-Function Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe7s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-09)(Revised Multi-Function Zombie Players Mode Extreme Remix Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe7s03,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-09)(Revised Multi-Function Mummy Players Mode Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe7s04,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_mslug3h,   ROT0, "Team Remix (Gaston90, Shyma.X, Ydmis And I love Nimes)",    "Metal Slug 3 (Enemies Resetting #7 Version 2021-10-09)(Revised Multi-Function Hybrid Bullets Extreme MegaMods Edition 2021-11-18)" , MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug3rmxe8s01,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, I love Nimes And Ydmis)",             "Metal Slug 3 (Random Enemies Resetting Version 2017-12-08)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)(Set 1)" , MACHINE_SUPPORTS_SAVE ) //Update Date 2021
GAME( 2021, mslug3rmxe8s02,    mslug3,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90, I love Nimes And Ydmis)",             "Metal Slug 3 (Random Enemies Resetting Version 2017-12-08)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-18)(Set 2)" , MACHINE_SUPPORTS_SAVE ) //Update Date 2021
// Metal Slug 4
GAME( 2007, mslug4hacks01,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "HappyASR",           "Metal Slug 4 (Super D Version)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mslug4hacks02,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Shyma.X",            "Metal Slug 4 (French Traduction 2006 Version)", MACHINE_SUPPORTS_SAVE )
GAME( 2007, mslug4hacks03,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Evoboy",             "Metal Slug 4 (Traduction Italian)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4hacks04,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "I love Nimes",       "Metal Slug 4 (Burst Enhanced Version 2021-04-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug4hacks05,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 4 (Last Bullet Remix 2017-07-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug4hacks06,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (Ammunition Random Edition 2016-07-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug4hacks07,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 4 (Enemies Resetting Version 2020-02-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug4hacks08,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (Multi-Function Version 2017-12-25)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4hacks09,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "hack",               "Metal Slug 4 (Children's Version 2021-06-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2010, mslug4hacks10,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "DDJ",                "Metal Slug 4 (Change Weapon 2016-03-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug4hacks11,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "DDJ And Gaston90",   "Metal Slug 4 (Change Zombie Fix 2017-05-25)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug4hacks12,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "DDJ",                "Metal Slug 4 (Jump In Mid Air 2017-08-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug4hacks13,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "DDJ",                "Metal Slug 4 (War Chariot 2016-02-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug4hacks14,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (Generation 1V2 2019-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug4hacks15,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 4 (Last Bullet Remix Bulletproof Vest Removed 2020-09-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug4hacks16,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Gaston90 And XiaShuiDaoMeiRenYu", "Metal Slug 4 (20th The Longest Fight Enemies Resetting Version 2022-10-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, mslug4hacks17,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "hack",               "Metal Slug 4 (Super G Edition Fix 2014-10-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4hacks18,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "KofKill, Wang Hua, Czk",  "Metal Slug 4 (Unity Time 2021-01-30)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4hacks19,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (20th Anniversary Edition 2022-03-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug4hacks20,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (Early Summer Starry Sky 2022-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug4hacks21,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (Komorebi 2022-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug4hacks22,     mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "GOTVG",              "Metal Slug 4 (The Longest Fight 2022-06-23)", MACHINE_SUPPORTS_SAVE )
// Decrypted And Bootleg
GAME( 2002, ms4boot,           mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",          "Metal Slug 4 (Bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mslug4d,           mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "bootleg",          "Metal Slug 4 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mslug4hd,          mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "bootleg",          "Metal Slug 4 (Custom Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mslug4nd,          mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",          "Metal Slug 4 (Fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, mslug4dd,          mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",          "Metal Slug 4 (Decrypted LW)", MACHINE_SUPPORTS_SAVE )
// Version Remastering
GAME( 2022, mslug4er01,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu)",                         "Metal Slug 4 (Enemies Resetting Plus Style Remastering Edition 2022-10-22)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug4er02,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu)",                         "Metal Slug 4 (Enemies Resetting Multi-Function Remastering Edition 2022-10-22)", MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2021, mslug4la01,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Plus Style Model Normal Players Mode Edition 2020-10-01)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4la02,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Plus Style Model Zombie Players Mode Edition 2020-10-01)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4la03,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Plus Style Model Mummy Players Mode Edition 2020-10-01)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4la04,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Plus Style Model Monkeys Players Mode Edition 2020-10-01)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lb01,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Plus Style Model Normal Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lb02,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Plus Style Model Zombie Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lb03,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Plus Style Model Mummy Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lb04,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Plus Style Model Monkeys Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lc01,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Multi-Function Normal Players Mode Edition 2017-12-25)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lc02,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Multi-Function Zombie Players Mode Edition 2017-12-25)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lc03,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Multi-Function Mummy Players Mode Edition 2017-12-25)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4lc04,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90 And I love Nimes)",                            "Metal Slug 4 (Multi-Function Monkeys Players Mode Edition 2017-12-25)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4ld01,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Multi-Function Normal Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4ld02,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Multi-Function Zombie Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE ) 
GAME( 2021, mslug4ld03,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Multi-Function Mummy Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4ld04,        mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu And I love Nimes)",        "Metal Slug 4 (Enemies Resetting Multi-Function Monkeys Players Mode Edition 2020-02-10)(Revised Remix Standard Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2021, mslug4rmxe0s01,    mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, Shyma.X, Creamymami[EGCG], DDJ And I love Nimes)", "Metal Slug 4 (Plus Style Model Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4rmxe1s01,    mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu, Shyma.X, Creamymami[EGCG], DDJ And I love Nimes)", "Metal Slug 4 (Plus Style Model Enemies Resetting Version 2020-02-10)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4rmxe2s01,    mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, Shyma.X, Creamymami[EGCG] And I love Nimes)",   "Metal Slug 4 (Multi-Function Version 2017-12-25)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4rmxe3s01,    mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_mslug4hb,  ROT0, "Team Remix (Gaston90, Creamymami[EGCG] And I love Nimes)",            "Metal Slug 4 (Enemies Resetting Multi-Function Version 2020-02-10)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug4rmxe4s01,    mslug4,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Team Remix (Gaston90 And Creamymami[EGCG])",                          "Metal Slug 4 (Last Bullet 2017-07-27)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-11-01)", MACHINE_SUPPORTS_SAVE )
// Metal Slug 5
GAME( 2016, mslug5hacks01,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "DDJ",                "Metal Slug 5 (Change Weapons 2016-02-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug5hacks02,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",               "Metal Slug 5 (Hybrid Bullets Version 2020-09-14)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mslug5hacks03,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Arkatrad",           "Metal Slug 5 (French Traduction Edition 2006)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, mslug5hacks04,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "GOTVG",              "Metal Slug 5 (The New Campaign 2016-03-10)", MACHINE_SUPPORTS_SAVE ) //It is removed by the flash from the shotgun edition 2016-10-19
GAME( 2021, mslug5hacks05,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "hack",               "Metal Slug 5 (Remake Final Fightcade2 Version 2021-11-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslug5hacks06,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "GOTVG",              "Metal Slug 5 (Fierce Battle 2016-03-10)", MACHINE_SUPPORTS_SAVE )  //It is removed by the flash from the shotgun edition 2016-10-19
GAME( 2020, mslug5hacks07,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Final Edition 2020-09-19)",  MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5hacks08,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "KofKill, Wang Hua, Czk",    "Metal Slug 5 (Unity Time! 2021-04-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks09,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (5X Final 2022-06-06)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks10,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Extremely Simplified Edition 2022-08-20)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks11,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "I love Nimes",       "Metal Slug 5 (Burst Enhanced Edition 2022-10-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug5hacks12,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Enemies Resetting Version 2019-07-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug5hacks13,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Enemy Soldiers Enhanced Version 2020-04-06)",  MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug5hacks14,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Enemy Soldiers Magic Version 2019-12-07)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslug5hacks15,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Generation 1V2 2019-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug5hacks16,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Summon Slug Edition 2018-09-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslug5hacks17,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "GOTVG",              "Metal Slug 5 (Multi-Function Version 2017-05-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks18,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Stone Tortoise Edition 2022-10-17)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug5hacks19,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",               "Metal Slug 5 (Crazy Fire Edition 2020-09-02)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, mslug5hacks20,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "hack",               "Metal Slug 5 (War Chariot 2015-05-20)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslug5hacks21,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "hack",               "Metal Slug 5 (Easy Mode Difficulty 2018-11-16)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks22,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Early Summer Starry Sky 2022-08-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks23,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Komorebi 2022-08-10)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslug5hacks24,     mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GOTVG",              "Metal Slug 5 (Extender Ver. 2022 2022-10-02)", MACHINE_SUPPORTS_SAVE )
// Decrypted And Bootleg
GAME( 2003, ms5boot,           mslug5,   neogeo_noslot, neogeo,   neogeo_state,   init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Bootleg, Set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, ms5pcbd,           mslug5,   neogeo_noslot, dualbios, neogeo_state,   init_ms5pcbd,   ROT0, "bootleg",            "Metal Slug 5 (JAMMA PCB)(Decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5nd,          mslug5,   neogeo_noslot, neogeo,   neogeo_state,   init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Fully Decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5d,           mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5d1,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Non Encrypted P, Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5b1,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5b1,  ROT0, "bootleg",            "Metal Slug 5 (Bootleg, Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, ms5plusd,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",            "Metal Slug 5 Plus (Bootleg, Fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5n,           mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Not Encrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5b3,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5b1,  ROT0, "bootleg",            "Metal Slug 5 (Bootleg, Set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5n2,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (Not Encrypted P & D)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslug5hd,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "bootleg",            "Metal Slug 5 (NGH-2680)(Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, ms5plusc,          mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_ms5plus,   ROT0, "bootleg",            "Metal Slug 5 Plus (Bootleg, Hacks Chinese)", MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2021, mslug5la01,        mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, YouGuDuoLa, C.B, I love Nimes And remikare)",                          "Metal Slug 5 (Enemy Soldiers Enhanced Multi-Function Version 2018-07-31)(Revised Remix Standard Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5lb01,        mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu, C.B, I love Nimes And remikare)",                  "Metal Slug 5 (Enemies Resetting Multi-Function Version 2019-01-01)(Revised Remix Standard Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslug5lc01,        mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu, C.B And remikare)",                                "Metal Slug 5 (Enemies Resetting Version 2019-07-28)(Revised Hybrid bullets Remix Standard Edition 2020-12-24)", MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2021, mslug5rmxe0s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, Creamymami[EGCG], I love Nimes, C.B And remikare)",                    "Metal Slug 5 (Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-12-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5rmxe1s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, YouGuDuoLa, Creamymami[EGCG], I love Nimes, C.B And remikare)",        "Metal Slug 5 (Enemy Soldiers Enhanced Multi-Function Version 2018-07-31)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5rmxe2s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, YouGuDuoLa, Creamymami[EGCG], C.B, I love Nimes And remikare)",        "Metal Slug 5 (Enemy Soldiers Enhanced Version 2020-01-30)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5rmxe3s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, Creamymami[EGCG], C.B And remikare)",                                  "Metal Slug 5 (Enemy Soldiers Magic Version 2019-12-07)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5rmxe4s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, C.B, Creamymami[EGCG] And remikare)",                                  "Metal Slug 5 (Fighting Machine Latest Version 2015-01-20)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslug5rmxe5s01,    mslug5,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Team Remix (Gaston90, XiaShuiDaoMeiRenYu, Creamymami[EGCG], I love Nimes, C.B And remikare)","Metal Slug 5 (Enemies Resetting Version 2019-07-28)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2020-11-01)", MACHINE_SUPPORTS_SAVE )
// Metal Slug X
GAME( 2013, mslugxhacks01,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Level Reset Special Edition 2013-10-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslugxhacks02,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "DDJ",               "Metal Slug X (Jump In Mid Air 2017-08-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslugxhacks03,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Azstar Soda Remix", "Metal Slug X (AzStar Soda Remix FC2 Version 2021-02-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2021, mslugxhacks04,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "KofKill, Wang Hua, Czk", "Metal Slug X (Unity Time! 2021-01-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, mslugxhacks05,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Green Edition 2013-02-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2006, mslugxhacks06,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Shyma.X",           "Metal Slug X (Translator French Version 2006)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, mslugxhacks07,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "PlayerX China",     "Metal Slug X (Enemy Heavy Plate 2013-06-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslugxhacks08,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "DDJ",               "Metal Slug X (Change Weapon And Fat Guy 2018-09-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, mslugxhacks09,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Clomax & Ombra",    "Metal Slug X (Translator Italian Beta)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslugxhacks10,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "DDJ",               "Metal Slug X (War Chariot 2016-03-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslugxhacks11,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "PlayerX China",     "Metal Slug X (Enemy Soldiers Remake Version 3 2017-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslugxhacks12,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "PlayerX China",     "Metal Slug X (Blue Out Of The Blue 2ND Edition 2017-12-29)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslugxhacks13,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "PlayerX China",     "Metal Slug X (Plus 2016-01-04)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxhacks14,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "I love Nimes",      "Metal Slug X (Burst Enhanced Edition 2022-10-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, mslugxhacks15,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "lb70 (Eezezy)",     "Metal Slug X (Violence Version (New) 2017-01-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, mslugxhacks16,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "hack",              "Metal Slug X (Hybrid Bullets Version 2014-11-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2019, mslugxhacks17,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Generation 1V2 2019-05-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslugxhacks18,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "hack",              "Metal Slug X (Big Heavy Machine Gun Flame Edition 2018-12-08)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks19,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (2R Version 2020-04-27)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxhacks20,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Early Summer Starry Sky 2022-08-10)", MACHINE_SUPPORTS_SAVE )
//GAME( 2020, mslugxhacks21,      mslugx,   neogeo_noslot, neogeo, neogeo_state,  init_mslugx,    ROT0, "Azstar China Soda", "Metal Slug X (2R Hybrid Bullets Version 2020-04-26)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks22,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (2R Blue Edition 2020-06-24)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks23,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (2R Generation 1V2 2020-05-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2013, mslugxhacks24,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Multi-Carrier Version 2013-05-28)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, mslugxhacks25,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Extreme Space Edition 2015-04-21)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, mslugxhacks26,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "lb70 (Eezezy)",     "Metal Slug X (Violence Version 2016-01-04)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks27,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Enemy Soldiers Reset Version 2.7 2021-04-03)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks28,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Unlimited Firepower 2020-10-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks29,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Infinite Fire Edition 2021-03-20 )", MACHINE_SUPPORTS_SAVE )
GAME( 2020, mslugxhacks30,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Generation SV 2020-11-04)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxhacks31,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "GOTVG",             "Metal Slug X (Survival Edition 2022-07-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, mslugxhacks32,     mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "hack",              "Metal Slug X (Easy Mode Difficulty 2018-11-26)", MACHINE_SUPPORTS_SAVE )
//Version Remastering
GAME( 2022, mslugxer01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Wang Hua)",                                  "Metal Slug X (Unlimited Firepower 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Wang Hua)",                                  "Metal Slug X (Enemy Heavy Plate 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Wang Hua)",                                  "Metal Slug X (Enemy Soldiers Remake Version 3 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer04,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Wang Hua)",                                  "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer05,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Metroid, Wang Hua)",                         "Metal Slug X (Multi-Carrier 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer06,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Wang Hua)",                                  "Metal Slug X (Level Reset Special 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxer07,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Azstar China Soda, Wang Hua)",               "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Survival Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp01,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Wang Hua)",                   "Metal Slug X (Unlimited Firepower 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp02,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Wang Hua)",                   "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp03,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Wang Hua)",                   "Metal Slug X (Enemy Soldiers Remake Version 3 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp04,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Wang Hua)",                   "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp05,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Metroid, Wang Hua)",                         "Metal Slug X (Multi-Carrier 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp06,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Wang Hua)",                   "Metal Slug X (Extreme Space 2R Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxesp07,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, Azstar China Soda, PlayerX China, Wang Hua)","Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Remastering Edition 2022-10-12)", MACHINE_SUPPORTS_SAVE )
//All Apocalyptic Time Edition, Test, Allen O'Neil See You In Hell Edition
GAME( 2021, mslugxtst01,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Testing My First Level 2020-04-07)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2020, mslugxtao01,       mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Allen O'Neil See You In Hell 1.0 2R Edition 2020-07-16)", MACHINE_SUPPORTS_SAVE ) // Update 2021 
GAME( 2021, mslugxat01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 0.1 2R Edition 2020-03-22)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2021, mslugxat02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 1.0 2R Edition 2020-04-20)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2021, mslugxat03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 2.1 2R Edition 2020-05-01)", MACHINE_SUPPORTS_SAVE ) // Update 2021 
GAME( 2021, mslugxat04,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 2.2 2R Edition 2020-05-11)", MACHINE_SUPPORTS_SAVE ) // Update 2021 
GAME( 2021, mslugxat05,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 3.0 2R Edition 2020-05-21)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2021, mslugxat06,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 4.0 2R Edition 2020-06-23)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2021, mslugxat07,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time 5.5 2R Edition 2020-08-04)", MACHINE_SUPPORTS_SAVE ) // Update 2021
GAME( 2021, mslugxat08,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time The 1 Beginning Of The End Of Time 6.0 2R Edition 2021-04-20)", MACHINE_SUPPORTS_SAVE ) 
GAME( 2021, mslugxat09,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time The 2 Beginning Of The End Of Time 7.1 2R Edition 2021-12-05)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxat10,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Gaston90",                       "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)", MACHINE_SUPPORTS_SAVE )
// Revised Remix Standard Edition
GAME( 2022, mslugxla01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxla02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxla03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlb01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlb02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlb03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlc01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Soldiers Remake Version 3 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlc02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Soldiers Remake Version 3 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlc03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Enemy Soldiers Remake Version 3 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxld01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxld02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxld03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxle01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxle02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxle03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlf01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlf02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlf03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlg01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlg02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlg03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, I love Nimes And Wang Hua)",                                                     "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlh01,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, I love Nimes And Wang Hua)",                                  "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlh02,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, I love Nimes And Wang Hua)",                                  "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxlh03,        mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, I love Nimes And Wang Hua)",                                  "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Remix Standard Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
// Extreme MegaMods Edition
GAME( 2022, mslugxrmxe0s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe0s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe0s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Unlimited Firepower 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe1s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe1s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe1s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Heavy Plate 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe2s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Soldiers Remake 2R Version 3 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe2s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Soldiers Remake 2R Version 3 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe2s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Enemy Soldiers Remake 2R Version 3 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe3s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe3s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe3s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Apocalyptic Time The 3 Beginning Of The End Of Time 8.1 2R Edition 2022-02-20)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe4s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe4s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe4s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Allen O'Neil See You In Hell 1.1 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe5s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe5s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe5s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Multi-Carrier 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe6s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe6s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe6s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, DDJ, julee, I love Nimes And Wang Hua)",                                      "Metal Slug X (Extreme Space 2R Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe7s01,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe7s02,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Big Heavy Machine Gun Flame Edition Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
GAME( 2022, mslugxrmxe7s03,    mslugx,   neogeo_noslot, neogeo, neogeo_state,     init_mslugx,    ROT0, "Team Remix (Gaston90, PlayerX China, Azstar China Soda, DDJ, julee, I love Nimes And Wang Hua)",                   "Metal Slug X (Enemy Soldiers Reset Version 2.7 2R Extreme Space Edition 2022-10-14)(Revised Hybrid Bullets Extreme MegaMods Edition 2021-12-31)", MACHINE_SUPPORTS_SAVE )
// Metal Slug HomeBrew
GAME( 2022, mslughw,           neogeo,   neogeo_noslot, neogeo, neogeo_state,     init_mslug5hb,  ROT0, "CB, Willnie",    "Metal Slug HomeBrew (Mothership Armageddon Easter Egg 2021-06-13)", MACHINE_SUPPORTS_SAVE )
// Mutation Nation
GAME( 1992, mutnathack01,      mutnat,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Pipi899",        "Mutation Nation (Ex version)", MACHINE_SUPPORTS_SAVE )
// NAM-1975
GAME( 2008, nam1975hack01,     nam1975,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Blast",          "NAM-1975 (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Ninja Combat
GAME( 1990, ncombathack01,     ncombat,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Krizal chen",    "Ninja Combat (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Ninja Commando
GAME( 2009, ncommandhack01,    ncommand, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "e107",           "Ninja Commando (Super Plus 2009-08-16)", MACHINE_SUPPORTS_SAVE )
// Neo Bomberman
GAME( 1997, neobombehack01,    neobombe, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Eezezy(Lb70)",   "Neo Bomberman (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Ninja Master's
GAME( 1996, ninjamashack01,    ninjamas, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Dodowang",       "Ninja Master's (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, ninjamashack02,    ninjamas, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "ZKW",            "Ninja Master's (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Nightmare in the Dark
GAME( 2000, nitdhack01,        nitd,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Nightmare in the Dark (Translation French)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, nitdhack02,        nitd,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Meganiuz",       "Nightmare in the Dark (Translation Spanish)", MACHINE_SUPPORTS_SAVE )
GAME( 2000, nitdhack03,        nitd,     neogeo_noslot, neogeo, neogeo_state,     init_nitd,      ROT0, "Xyahzhs",        "Nightmare in the Dark (Plus Max)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2000, nitdd,             nitd,     neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "bootleg",        "Nightmare in the Dark (Decrypted C)", MACHINE_SUPPORTS_SAVE )
// Puzzle Bobble
GAME( 2009, pbobblenhack01,    pbobblen, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Elrayzeur",      "Puzzle Bobble (Crazy beta 2 2009-07-23)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, pbobblenhack02,    pbobblen, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Elrayzeur",      "Puzzle Crazy", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2000, pbobblendx,        pbobblen, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Puzzle Bobble Deluxe (bootleg)", MACHINE_SUPPORTS_SAVE )
// Pochi & Nyaa
//bootleg
GAME( 2003, pnyaad,            pnyaa,    neogeo_noslot, neogeo, neogeo_state,     init_pnyaad,    ROT0, "Aiky",           "Pochi & Nyaa (decrypted C Set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, pnyaadd,           pnyaa,    neogeo_noslot, neogeo, neogeo_state,     init_pnyaad,    ROT0, "Aiky",           "Pochi & Nyaa (decrypted C Set 2)", MACHINE_SUPPORTS_SAVE )
// Prehistoric Isle 2
GAME( 1999, preisle2hack01,    preisle2, neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Xyahzhs",        "Prehistoric Isle 2 (Plus Max)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 1999, preisle2d,         preisle2, neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Yumekobo",       "Prehistoric Isle 2 (decrypted C)", MACHINE_SUPPORTS_SAVE )
// Ragnagard
GAME( 1996, ragnagrdhack01,    ragnagrd, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Operation Ragnagard (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, ragnagrdhack02,    ragnagrd, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "invgod",         "Operation Ragnagard (Moves Easy)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, ragnagrdhack03,    ragnagrd, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Operation Ragnagard (Boss With Simple Attack Edition 2016-02-14)", MACHINE_SUPPORTS_SAVE )
// Real Bout Fatal Fury Special
GAME( 1996, rbffspechack01,    rbffspec, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Creamymami & Dodowang",   "Real Bout Fatal Fury Special (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, rbffspechack02,    rbffspec, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Real Bout Fatal Fury Special (Edition Renewed Edition 2014-12-12)", MACHINE_SUPPORTS_SAVE )
// Real Bout Fatal Fury
GAME( 2014, rbff1hack01,       rbff1,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Dodowang",       "Real Bout Fatal Fury (Legends Renewed Edition 2014-12-11)", MACHINE_SUPPORTS_SAVE ) // He is also known Easy moves
// Real Bout Fatal Fury 2
GAME( 1998, rbff2hack01,       rbff2,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Eddids - FCHT",  "Real Bout Fatal Fury 2 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, rbff2hack02,       rbff2,    neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Eddids[FCHT]",   "Real Bout Fatal Fury 2 (Resurgence Enhanced Edition 2014-12-11)", MACHINE_SUPPORTS_SAVE )
// Rage of the Dragons
GAME( 2002, rotdhack01,        rotd,     neogeo_noslot, neogeo, neogeo_state,     init_rotd,      ROT0, "Dodowang[EGCG]", "Rage of the Dragons (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, rotdhack02,        rotd,     neogeo_noslot, neogeo, neogeo_state,     init_rotdb,     ROT0, "BisonSAS",       "Rage of the Dragons (Portuguese)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, rotdhack03,        rotd,     neogeo_noslot, neogeo, neogeo_state,     init_rotd,      ROT0, "DDJ",            "Rage of the Dragons (Easy Move)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2002, rotdd,             rotd,     neogeo_noslot, neogeo, neogeo_state,     init_rotdd,     ROT0, "bootleg",        "Rage of the Dragons (decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, rotdnd,            rotd,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Rage of the Dragons (Fully Decrypted)", MACHINE_SUPPORTS_SAVE )
// Strikers 1945 Plus
GAME( 1999, s1945phack01,      s1945p,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "Yumeji",         "Strikers 1945 Plus (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2015, s1945phack02,      s1945p,   neogeo_noslot, neogeo, neogeo_state,     init_s1945p,    ROT0, "wesker[FGCH]",   "Strikers 1945 Plus (Ex Super version 2015-01-27)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 1999, s1945pd,           s1945p,   neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "Psikyo",         "Strikers 1945 Plus (decrypted C)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown
GAME( 1993, samshohack01,      samsho,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Ydmis",          "Samurai Shodown (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1993, samshohack02,      samsho,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "Samurai Shodown (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samshohack03,      samsho,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown (Boss With Simple Attack Edition 2018-07-24)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown II
GAME( 1994, samsho2hack01,     samsho2,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji[2ch]",    "Samurai Shodown II (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, samsho2hack02,     samsho2,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "Samurai Shodown II (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, samsho2hack03,     samsho2,  gsc,           neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Samurai Shodown II (Special Edition v1.1)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, samsho2hack04,     samsho2,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown II (Boss With Simple Attack Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown III
GAME( 1995, samsho3hack01,     samsho3,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji[2ch]",    "Samurai Shodown III (Hidden Characters-Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1995, samsho3hack02,     samsho3,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "Samurai Shodown III (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samsho3hack03,     samsho3,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown III (Boss With Simple Attack Edition 2018-11-11)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2010, samsho3hb,         samsho3,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Samurai Shodown III (bootleg / hack)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown IV
GAME( 1996, samsho4hack01,     samsho4,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji[2ch]",    "Samurai Shodown IV (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, samsho4hack02,     samsho4,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "Samurai Shodown IV (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 1996, samsho4hack03,     samsho4,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "DDJ",            "Samurai Shodown IV (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, samsho4hack04,     samsho4,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "GSC2007",        "Samurai Shodown IV (Special 2017 2017-06-30)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samsho4hack05,     samsho4,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown IV (Boss With Simple Attack Edition 2018-11-12)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown V
GAME( 2003, samsho5hack01,     samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_samsho5,   ROT0, "Dodowang",       "Samurai Shodown V (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5hack02,     samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_samsho5,   ROT0, "DDJ",            "Samurai Shodown V (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5hack03,     samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_samsho5,   ROT0, "DDJ",            "Samurai Shodown V (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samsho5hack04,     samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_samsho5,   ROT0, "hack",           "Samurai Shodown V (Boss With Simple Attack Edition 2018-11-13)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5hack05,     samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown V (XBOX version)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2003, samsho5bd,         samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Samurai Shodown V (Fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5cd,         samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_sam5hb,    ROT0, "bootleg",        "Samurai Shodown V (Custom Decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5d,          samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_sam5hb,    ROT0, "bootleg",        "Samurai Shodown V (Non encrypted P, Decrypted C)" , MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5d1,         samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_sam5hb,    ROT0, "bootleg",        "Samurai Shodown V (No encrypted P)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, samsho5hd,         samsho5,  neogeo_noslot, neogeo, neogeo_state,     init_sam5hb,    ROT0, "bootleg",        "Samurai Shodown V (Decrypted C)", MACHINE_SUPPORTS_SAVE )
// Samurai Shodown V Special
GAME( 2004, samsh5sphack01,    samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "DDJ",            "Samurai Shodown V Special (Always Have Super Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5sphack02,    samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "DDJ",            "Samurai Shodown V Special (Easy Move)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, samsh5sphack03,    samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "hack",           "Samurai Shodown V Special (Simplify Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, samsh5sphack04,    samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown V Special (Edition Final, location test 2020-06-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, samsh5sphack05,    samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Samurai Shodown V Special (Perfect Edition 2020)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2004, samsh5spn,         samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "bootleg",        "Samurai Shodown V Special (fully decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5spd,         samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "bootleg",        "Samurai Shodown V Special (decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5spd1,        samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "bootleg",        "Samurai Shodown V Special (non encrypted P, decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5spd2,        samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "bootleg",        "Samurai Shodown V Special (no encrypted P)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5sphd,        samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "bootleg",        "Samurai Shodown V Special (1st release, censored)", MACHINE_SUPPORTS_SAVE )
GAME( 2004, samsh5spnd,        samsh5sp, neogeo_noslot, neogeo, neogeo_state,     init_sam5sphb,  ROT0, "bootleg",        "Samurai Shodown V Special (2nd release, less censored)", MACHINE_SUPPORTS_SAVE )
// Savage Reign
GAME( 1995, savagerehack01,    savagere, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Savage Reign (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, savagerehack02,    savagere, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Savage Reign (Simplify Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
// Super Dodge Ball
GAME( 1996, sdodgebhac01,      sdodgeb,  neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "PlayerX",        "Super Dodge Ball (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Sengoku 3
GAME( 2001, sengoku3hack01,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "HappyASR[EGCG]", "Sengoku 3 (Unlock All Stages And Hidden Character)" , MACHINE_SUPPORTS_SAVE )
GAME( 2017, sengoku3hack02,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "Kim[FGCH]",      "Sengoku 3 (Warring States Tradition 3 Qing Version 2017-04-02)" , MACHINE_SUPPORTS_SAVE )
GAME( 2018, sengoku3hack03,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "hack",           "Sengoku 3 (Blue Out Of Blue 2018-06-04)" , MACHINE_SUPPORTS_SAVE )
GAME( 2020, sengoku3hack04,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "hack",           "Sengoku 3 (Fengshen Edition 2020-05-15)", MACHINE_SUPPORTS_SAVE )
GAME( 2020, sengoku3hack05,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "hack",           "Sengoku 3 (Full Attack Enhanced Edition 2020-09-18)", MACHINE_SUPPORTS_SAVE )
GAME( 2014, sengoku3hack06,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "hack",           "Sengoku 3 (Green version 2014-01-11)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, sengoku3hack07,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "DDJ",            "Sengoku 3 (Always Have Super Move)" , MACHINE_SUPPORTS_SAVE )
GAME( 2001, sengoku3hack08,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_sengoku3,  ROT0, "DDJ",            "Sengoku 3 (Easy Move)" , MACHINE_SUPPORTS_SAVE )
GAME( 2001, sengoku3hack09,    sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Sengoku 3 (Evolution v1.0)", MACHINE_SUPPORTS_SAVE )
// bootleg
GAME( 2001, sengoku3d,         sengoku3, neogeo_noslot, neogeo, neogeo_state,     init_cmc42sfix, ROT0, "bootleg",        "Sengoku 3 (decrypted C)", MACHINE_SUPPORTS_SAVE )
// Shock Troopers - 2nd Squad
GAME( 1998, shocktr2hack01,    shocktr2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Shock Troopers - 2nd Squad (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Shock Troopers
GAME( 2009, shocktrohack01,    shocktro, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "e107",           "Shock Troopers (Super Plus 2009-08-18)", MACHINE_SUPPORTS_SAVE )
// Aero Fighters 2
GAME( 1994, sonicwi2hack01,    sonicwi2, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Aero Fighters 2 (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Aero Fighters 3
GAME( 1995, sonicwi3hack01,    sonicwi3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "wesker",         "Aero Fighters 3 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1995, sonicwi3hack02,    sonicwi3, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Aero Fighters 3 (Plus Max)", MACHINE_SUPPORTS_SAVE )
// SNK vs. Capcom
GAME( 2016, svcsplushack01,    svc,      neogeo_noslot, neogeo, neogeo_state,     init_svcsplus,  ROT0,  "hack",          "SvC Chaos Super Plus (Edition Simplified 2016 2016-03-15)", MACHINE_SUPPORTS_SAVE )
//bootleg
GAME( 2003, svcfd,             svc,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "bootleg",       "SvC Chaos (MVS / Fully Decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcd,              svc,      neogeo_noslot, neogeo, neogeo_state,     init_svchb,     ROT0,  "hack",          "SvC Chaos (Non encrypted P, Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcd1,             svc,      neogeo_noslot, neogeo, neogeo_state,     init_svchb,     ROT0,  "hack",          "SvC Chaos (Decrypted C set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcnd,             svc,      neogeo_noslot, neogeo, neogeo_state,     init_svchb,     ROT0,  "hack",          "SvC Chaos (Decrypted C)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcplusn,          svc,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "bootleg",       "SvC Chaos (Bootleg, set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcplusb,          svc,      neogeo_noslot, neogeo, neogeo_state,     init_svchb,     ROT0,  "bootleg",       "SvC Chaos Plus (Bootleg, set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, svcplusd,          svc,      neogeo_noslot, neogeo, neogeo_state,     init_svchb,     ROT0,  "bootleg",       "SvC Chaos Plus (bootleg, fully decrypted)", MACHINE_SUPPORTS_SAVE )
// Spin Master
GAME( 1993, spinmasthack01,    spinmast, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "kawada7278",     "Spin Master (Enhanced Version)", MACHINE_SUPPORTS_SAVE )
// Top Hunter
GAME( 1994, tophuntrhack01,    tophuntr, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Xyahzhs",        "Top Hunter (Plus Max)", MACHINE_SUPPORTS_SAVE )
// Twinkle Star Sprites
GAME( 1996, twinsprihack01,    twinspri, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Yumeji",         "Twinkle Star Sprites (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
// Viewpoint
GAME( 2004, viewpoinhack01,    viewpoin, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Shyma.X",        "Viewpoint (Traduction French 07-29-2004)", MACHINE_SUPPORTS_SAVE )
GAME( 1992, viewpoinhack02,    viewpoin, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "KiWi Starlight", "Viewpoint (Enhanced Version)", MACHINE_SUPPORTS_SAVE )
// Waku Waku 7
GAME( 1996, wakuwak7hack01,    wakuwak7, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "yumeji",         "Waku Waku 7 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, wakuwak7hack02,    wakuwak7, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "hack",           "Waku Waku 7 (Simplify Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
// Windjammers
GAME( 1994, wjammershack01,    wjammers, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Elrayzeur",      "Windjammers (Traduction French Version 0.1 beta)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, wjammershack02,    wjammers, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0, "Aneue Bannzai",  "Windjammers (Traduction Korean)", MACHINE_SUPPORTS_SAVE )
// World Heroes Perfect
GAME( 1995, whphack01,         whp,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "Dodowang[EGCG]","World Heroes Perfect (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, whphack02,         whp,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "hack",          "World Heroes Perfect (Simplify Edition 2018-06-04)", MACHINE_SUPPORTS_SAVE )
// World Heroes
GAME( 1992, wh1hack01,         wh1,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "yumeji[2ch]",   "World Heroes (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, wh1hack02,         wh1,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "hack",          "World Heroes (Simple Attack Edition 2018-09-12)", MACHINE_SUPPORTS_SAVE )
// World Heroes 2
GAME( 1993, wh2hack01,         wh2,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "yumeji[2ch]",   "World Heroes 2 (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1993, wh2hack02,         wh2,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "kawada7278",    "World Heroes 2 (Easy Special Attacks)", MACHINE_SUPPORTS_SAVE )
GAME( 2018, wh2hack03,         wh2,      neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "hack",          "World Heroes 2 (Boss With Simple Attack Edition 2018-09-12)", MACHINE_SUPPORTS_SAVE )
// World Heroes 2
GAME( 1994, wh2jhack01,        wh2j,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "yumeji[2ch]",   "World Heroes 2 Jet (Enable Hidden Characters)", MACHINE_SUPPORTS_SAVE )
GAME( 1994, wh2jhack02,        wh2j,     neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "kawada7278",    "World Heroes 2 Jet (Easy Special Attacks)", MACHINE_SUPPORTS_SAVE )
// ZinTricK
GAME( 2009, zintrckbhack01,    zintrckb, neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "Yumeji",        "ZinTricK (Enable hidden characters)", MACHINE_SUPPORTS_SAVE )
// Zupapa!
//bootleg
GAME( 2001, zupapad,           zupapa,   neogeo_noslot, neogeo, neogeo_state,     init_neogeo,    ROT0,  "bootleg",       "Zupapa! (Custom decrypted C)", MACHINE_SUPPORTS_SAVE )
