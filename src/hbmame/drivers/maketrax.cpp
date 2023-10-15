// license:BSD-3-Clause
// copyright-holders:Robbbert
/*************************************
 *
 *  Make Trax input handlers
 *
 *************************************/

void puckman_state::maketrax_protection_w(u8 data)
{
	if (data == 0) // disable protection / reset?
	{
		m_maketrax_counter = 0;
		m_maketrax_offset = 0;
		m_maketrax_disable_protection = 1;
		return;
	}

	if (data == 1)
	{
		m_maketrax_disable_protection = 0;

		m_maketrax_counter++;
		if (m_maketrax_counter == 0x3c)
		{
			m_maketrax_counter = 0;
			m_maketrax_offset++;

			if (m_maketrax_offset == 0x1e)
				m_maketrax_offset = 0;
		}
	}
}

u8 puckman_state::maketrax_special_port2_r(offs_t offset)
{
	u8 data = ioport("DSW1")->read() & 0x3f;

	if (m_maketrax_disable_protection == 0)
		return m_p_maincpu[0xebe + m_maketrax_offset*2] | data;

	switch (offset)
	{
		case 0x01:
		case 0x04:
			data |= 0x40; break;
		case 0x05:
			data |= 0xc0; break;
		default:
			break;
	}

	return data;
}

u8 puckman_state::maketrax_special_port3_r(offs_t offset)
{

	if (m_maketrax_disable_protection == 0)
		return m_p_maincpu[0xebd + m_maketrax_offset*2];

	switch (offset)
	{
		case 0x00:
			return 0x1f;
		case 0x09:
			return 0x30;
		case 0x0c:
			return 0x00;
		default:
			return 0x20;
	}
}

void puckman_state::piranha_interrupt_vector_w(u8 data)
{
	if (data == 0xfa)
		data = 0x78;

	m_maincpu->set_input_line_vector(0, data );
}



/*************************************
 *
 *  Main CPU memory handlers
 *
 *************************************/

/*************************************
 *
 *  Main CPU port handlers
 *
 *************************************/

void puckman_state::piranha_writeport(address_map &map) {
	map.global_mask(0xff);
	map(0x00,0x00).w(FUNC(puckman_state::piranha_interrupt_vector_w));
}


/*************************************
 *
 *  Port definitions
 *
 *************************************/

static INPUT_PORTS_START( eyes )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_SERVICE( 0x10, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_TILT )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_COIN2 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x08, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x0c, "2" )
	PORT_DIPSETTING(    0x08, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x00, "5" )
	PORT_DIPNAME( 0x30, 0x30, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x30, "50000" )
	PORT_DIPSETTING(    0x20, "75000" )
	PORT_DIPSETTING(    0x10, "100000" )
	PORT_DIPSETTING(    0x00, "125000" )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )  /* Not accessed */
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START ("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( maketrax )
	PORT_START ("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Cocktail ) )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	PORT_START ("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */

	PORT_START ("DSW1")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x08, "5" )
	PORT_DIPSETTING(    0x0c, "6" )
	PORT_DIPNAME( 0x10, 0x10, "First Pattern" )
	PORT_DIPSETTING(    0x10, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hard ) )
	PORT_DIPNAME( 0x20, 0x20, "Teleport Holes" )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_BIT( 0xc0, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection */

	PORT_START ("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END



/*************************************
 *
 *  Graphics layouts
 *
 *************************************/

/*************************************
 *
 *  Machine drivers
 *
 *************************************/

void puckman_state::maketrax(machine_config &config)
{
	pacman(config);
	MCFG_MACHINE_RESET_OVERRIDE(puckman_state,maketrax)
}

void puckman_state::piranha(machine_config &config)
{
	pacman(config);
	m_maincpu->set_addrmap(AS_IO, &puckman_state::piranha_writeport);
}


/*************************************
 *
 *  Driver initialization
 *
 *************************************/

MACHINE_RESET_MEMBER(puckman_state,maketrax)
{
	m_maketrax_counter = 0;
	m_maketrax_offset = 0;
	m_maketrax_disable_protection = 0;
}

void puckman_state::init_maketrax()
{
	/* set up protection handlers */
	m_maincpu->space(AS_PROGRAM).install_write_handler(0x5004, 0x5004, write8smo_delegate(*this, FUNC(puckman_state::maketrax_protection_w)));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x5080, 0x50bf, read8sm_delegate(*this, FUNC(puckman_state::maketrax_special_port2_r)));
	m_maincpu->space(AS_PROGRAM).install_read_handler(0x50c0, 0x50ff, read8sm_delegate(*this, FUNC(puckman_state::maketrax_special_port3_r)));

	save_item(NAME(m_maketrax_disable_protection));
	save_item(NAME(m_maketrax_offset));
	save_item(NAME(m_maketrax_counter));
}

/*************************************
 *
 *  ROM definitions
 *
 *************************************/

ROM_START( 100doors )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "100doors.6e",  0x0000, 0x1000, CRC(b8c27195) SHA1(0f5eab4fa3f5cb44ee5cfff58e57b5694bc18645) )
	ROM_LOAD( "100doors.6f",  0x1000, 0x1000, CRC(8f936d42) SHA1(e94d8c24096c45bdf9c0c434883ee4a068340d30) )
	ROM_LOAD( "100doors.6h",  0x2000, 0x1000, CRC(10068710) SHA1(d3830a095bbd40673b83d58723e4aaafbb0e557d) )
	ROM_LOAD_OPTIONAL( "100doors.6j",  0x3000, 0x1000, CRC(c71c0011) SHA1(1ceaf73df40e531df3bfb26b4fb7cd95fb7bff1d) ) // empty

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "100doors.5e",  0x0000, 0x1000, CRC(1a2bc077) SHA1(7e8f64c284d711ebcd26b83fa6d0c8b283b45fe6) )
	ROM_LOAD( "ghohunt.5f",   0x1000, 0x1000, CRC(3ad83b13) SHA1(d2ce7ab45cb540f35cb23264e7628ac0ee6b8559) )

	PACMAN_PROMS
ROM_END

ROM_START( aa )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "aa.1",         0x0000, 0x1000, CRC(7b73ff28) SHA1(3b05c9ecaa418291b9b3501fbfd4a1e48be7281e) )
	ROM_LOAD( "aa.2",         0x1000, 0x1000, CRC(848ca2fa) SHA1(d11e874a0bd0dcf88ed0781d7dc7b7d98b4ac1e8) )
	ROM_LOAD( "aa.3",         0x2000, 0x1000, CRC(b3d3ff37) SHA1(0df28470eb70f4a84f24c2a86b4b9d338b9b6a76) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "aa.5e",        0x0000, 0x1000, CRC(e69596af) SHA1(b53ef6fce2d9fa1163f722a1a6be56085bde415c) )
	ROM_LOAD( "aa.5f",        0x1000, 0x1000, CRC(c26ecd63) SHA1(40d618b171c7ea164384c2ded098520c77941cbc) )

	PACMAN_PROMS
ROM_END

ROM_START( abscam )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "as0.bin",      0x0000, 0x0800, CRC(0b102302) SHA1(97f6399182db4f84efe482bf3a710aa45ca104ec) )
	ROM_LOAD( "as4.bin",      0x0800, 0x0800, CRC(3116a8ec) SHA1(259169bcc8fbe9fc73ca5100c3835a233351f530) )
	ROM_LOAD( "as1.bin",      0x1000, 0x0800, CRC(bc0281e0) SHA1(bcc6d63ede728d9b29f903489bfa80d94ec5cc00) )
	ROM_LOAD( "as5.bin",      0x1800, 0x0800, CRC(428ee2e8) SHA1(1477d1a86b32483ac0fdeea93512f517c9f66ce2) )
	ROM_LOAD( "as2.bin",      0x2000, 0x0800, CRC(e05d46ad) SHA1(87da57dbbe6ab5e1dd005fd68a982f1df917459c) )
	ROM_LOAD( "as6.bin",      0x2800, 0x0800, CRC(3ae9a8cb) SHA1(72896ad32cbdde90793788182958a943e35672f9) )
	ROM_LOAD( "as3.bin",      0x3000, 0x0800, CRC(b39eb940) SHA1(e144a1553c76ddee1c22ad1ed0cca241c2d03998) )
	ROM_LOAD( "as7.bin",      0x3800, 0x0800, CRC(16cf1c67) SHA1(0015fe64d476de87f1a030e7f2e735380dfcfd41) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "as8.bin",      0x0000, 0x0800, CRC(61daabe5) SHA1(00503916d1d1011afe68898e3416718c0e63a298) )
	ROM_LOAD( "as10.bin",     0x0800, 0x0800, CRC(81d50c98) SHA1(6b61c666f68b5948e4facb8bac1378f986f993a7) )
	ROM_LOAD( "as9.bin",      0x1000, 0x0800, CRC(a3bd1613) SHA1(c59bb0a4d1fa5cbe596f41ee7b1a4a661ab5614b) )
	ROM_LOAD( "as11.bin",     0x1800, 0x0800, CRC(9d802b68) SHA1(4e8f37c2faedcfce91221a34c14f6490d578c80a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "as4a.bin",     0x0020, 0x0100, CRC(1605b324) SHA1(336fce22caedbe69bcba9cea2b43e00f6f8e8067) )

	PACMAN_SOUND_PROMS
ROM_END

/* This game is not complete. Should a better version emerge, it will replace this one */
/* QuadBlok works, you need to move joystick to select it, then insert coin */
/* No sound */
ROM_START( absurd )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "absurd.1",     0x0000, 0x1000, CRC(cbda6dcb) SHA1(e17628ecc06b8877c6c9e55e2c8983272dd38e1e) )
	ROM_LOAD( "absurd.2",     0x1000, 0x1000, CRC(3b669ec6) SHA1(94db6cb04f74384a2ef1830e48b9fb79e1636739) )
	ROM_LOAD( "absurd.3",     0x2000, 0x1000, CRC(beb9024b) SHA1(9fb296e6f27373a1364a7ef439e36276e19e09f6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "absurd.5e",    0x0000, 0x1000, CRC(0a4f5731) SHA1(0023b3175da4f25b5c59550c20b0435ac85cec2f) )
	ROM_LOAD( "absurd.5f",    0x1000, 0x1000, CRC(ae9805ee) SHA1(fd90b8bb2492a39f27216e0cda09f9594a9dada7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "glob.7f",      0x0000, 0x0020, CRC(1f617527) SHA1(448845cab63800a05fcb106897503d994377f78f) )
	ROM_LOAD( "absurd.4a",    0x0020, 0x0100, CRC(9379a246) SHA1(88c84668f67afc4a5ff12e794ac37a0636bd7153) )

	PACMAN_SOUND_PROMS
ROM_END

// Alien Rescue by MonstersGoBoom. No sound. Use 1 to jump.
ROM_START( alienres )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alienres.6e",  0x0000, 0x1000, CRC(154e2017) SHA1(026ec12d575dbb8cb6df5cb748a9dff833d3669f) )
	ROM_LOAD( "alienres.6f",  0x1000, 0x1000, CRC(75237455) SHA1(8af41e9e3f4e3fbda9586ea93c9db29526906d23) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alienres.5e",  0x0000, 0x1000, CRC(57ae12e7) SHA1(5c9de2cf75b183484c6d22b999ba823d69da7268) )
	ROM_LOAD( "alienres.5f",  0x1000, 0x1000, CRC(097eca05) SHA1(f863108e2cc5419b239e3cf02854438701f3ef28) )

	ROM_REGION( 0x0400, "user1", 0 ) // unused
	ROM_LOAD_OPTIONAL( "alienres.1k",  0x0000, 0x0400, CRC(b9d2e5e6) SHA1(82546b7ecf9e46374b1af1ad32f582b4d35bddd1) )

	PACMAN_PROMS
ROM_END

// Alien Rescue, stuck at first screen. Assumed to be a non-working prototype.
ROM_START( alienresp )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alienresp.6e",  0x0000, 0x1000, CRC(4d94ce2a) SHA1(c36f60b046f45da03f1a6218195d7ca0b2eb8ef8) )
	ROM_LOAD( "alienresp.6f",  0x1000, 0x1000, CRC(5f81d441) SHA1(dd8035fff171306f90ab382f1decca322a3223f3) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alienresp.5e",  0x0000, 0x1000, CRC(ab38f274) SHA1(bf623e0a2ac855186319be6f2e96fd1ecabab84a) )
	ROM_LOAD( "alienresp.5f",  0x1000, 0x1000, CRC(ce1c6cb2) SHA1(fae57b9105a3768536b71c9b07a33fdcea91df10) )

	ROM_REGION( 0x0400, "user1", 0 ) // unused
	ROM_LOAD_OPTIONAL( "alienres.1k",  0x0000, 0x0400, CRC(b9d2e5e6) SHA1(82546b7ecf9e46374b1af1ad32f582b4d35bddd1) )

	PACMAN_PROMS
ROM_END

ROM_START( alpaca7 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alpaca7.6e",   0x0000, 0x1000, CRC(AF4AFCBF) SHA1(F11E2FE309818B41CB2A28408B06D18419879C09) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alpaca.5e",    0x0000, 0x1000, CRC(3E879F02) SHA1(0b084DD449E57476231E59F15F85A209A919959C) )
	ROM_LOAD( "alpaca7.5f",   0x1000, 0x1000, CRC(A6103847) SHA1(4376996FF8C19AFD65F1757CE159B70071A4BD3B) )

	PACMAN_PROMS
ROM_END

ROM_START( alpaca8 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "alpaca8.6e",   0x0000, 0x1000, CRC(86027944) SHA1(C47FC62522A3BAE0D49F4B68C218F73C43ED19B5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "alpaca.5e",    0x0000, 0x1000, CRC(3E879F02) SHA1(0b084DD449E57476231E59F15F85A209A919959C) )
	ROM_LOAD( "alpaca8.5f",   0x1000, 0x1000, CRC(856E53AE) SHA1(95460212107B3371600569DBD4DA482EC631ABDB) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2FC650BD) SHA1(8D0268DEE78E47C712202B0EC4F1F51109B1F2A5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2BC5D339) SHA1(446E234DF94D9EF34C3191877BB33DD775ACFDF5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( bace )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "bace.1",       0x0000, 0x1000, CRC(8b60ff7c) SHA1(c787246f0ae080fc9088631acb923a2df37c14b2) )
	ROM_LOAD( "bace.2",       0x1000, 0x1000, CRC(25d8361a) SHA1(20792c3d4c583dfc0297c6b5db1c14194e053088) )
	ROM_LOAD( "bace.3",       0x2000, 0x1000, CRC(fc38d994) SHA1(319691caf79292f21282006337df3c637c033602) )
	ROM_LOAD( "bace.4",       0x3000, 0x1000, CRC(5853f341) SHA1(0794324ee1bab92baaa6f5bf2cd647ce10614200) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "bace.5e",      0x0000, 0x1000, CRC(6da99c7b) SHA1(dcd488e0117662caf77f453b73330740ae34ca55) )
	ROM_LOAD( "bace.5f",      0x1000, 0x1000, CRC(b81cdc64) SHA1(67f4cde661dbcdcf0192289e31ee80cbd0dc9dc5) )

	PACMAN_PROMS
ROM_END

ROM_START( crashh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "crashh.1",     0x0000, 0x1000, CRC(04353b41) SHA1(28b774fd5b429569f44651cfe4df6a9929310190) )
	ROM_LOAD( "crashh.2",     0x1000, 0x1000, CRC(e03205c0) SHA1(8c5ae42a9e2d551f9184e952723a750abc3dfb52) )
	ROM_LOAD( "crashh.3",     0x2000, 0x1000, CRC(b0fa8e46) SHA1(2caf94d1a14d96ddc401375b54bf2b1edb13c68b) )
	ROM_LOAD( "crashh.4",     0x3000, 0x1000, CRC(bfa4d2fe) SHA1(76ef449173c01f8943049330dba9c6a54a5e760a) )
	ROM_LOAD( "crashh.5",     0x8000, 0x1000, CRC(12f2f224) SHA1(95528aaef908173e1374a7c6ed1a9cda4b685a20) )
	ROM_LOAD( "pacman.6j",    0x9000, 0x1000, CRC(817d94e3) SHA1(d4a70d56bb01d27d094d73db8667ffb00ca69cb9) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "crashh.5e",    0x0000, 0x1000, CRC(0a25969b) SHA1(2cd54477a5cc15f91ae2cba3c4270002ac15967e) )
	ROM_LOAD( "crashh.5f",    0x1000, 0x1000, CRC(447ea79c) SHA1(886d0113608616b01e4395485c6106586adcb65a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( dderby )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "dderby.6e",    0x0000, 0x1000, CRC(6f373bd4) SHA1(e2c4f9def801c5664cf2b9684a36f762e97d12d6) )
	ROM_LOAD( "dderby.6f",    0x1000, 0x1000, CRC(2fbf16bf) SHA1(9f3714ecc40707cfa4ff1942d5fe87a03888f548) )
	ROM_LOAD( "dderby.6h",    0x2000, 0x1000, CRC(6e16cd16) SHA1(7f13a2b181e4a62d7d71294cac8fe09fd9ac64a9) )
	ROM_LOAD( "dderby.6j",    0x3000, 0x1000, CRC(f7e09874) SHA1(7080aaa47e0bc893cd8bd038de7af769ef4eddf6) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "dderby.5e",    0x0000, 0x1000, CRC(7e2c0a53) SHA1(c3c62b32927ac3c5948faeff28d4c2148fe18d0b) )
	ROM_LOAD( "dderby.5f",    0x1000, 0x1000, CRC(cb2dd072) SHA1(e5813cc4247b9799e9513ff1a624fabc066582e3) )

	PACMAN_PROMS
ROM_END

ROM_START( deathstar ) // No text, no scores. From level 3, you need to use a 2nd joystick (RDFG keys).
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "dstar.6e",     0x0000, 0x1000, CRC(decc3963) SHA1(cbc291c767def5c92b82d6e00ebb5e61e45cbc41) )
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( deathstar2 ) // No text, no scores. From level 3, you need to use a 2nd joystick (RDFG keys).
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "dstar2.6e",    0x0000, 0x1000, CRC(53a5428e) SHA1(db4c271e47b9b14e4b266f30ff145c2074944f0f) )
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "dstar2.5e",    0x0000, 0x1000, CRC(99ad7923) SHA1(03dcae6625335ae2abc58b1e3e7724e84c985c6e) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( eyes )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "eyes.7d",      0x0000, 0x1000, CRC(3b09ac89) SHA1(a8f1c918da74495bb73172f39364dada38ae4713) )
	ROM_LOAD( "eyes.7f",      0x1000, 0x1000, CRC(97096855) SHA1(10d3b164bbbe5eee86e881a1434f0c114ee8adff) )
	ROM_LOAD( "eyes.7h",      0x2000, 0x1000, CRC(731e294e) SHA1(96c0308c146dbd85e244c4530af9ae8df78c86de) )
	ROM_LOAD( "eyes.7j",      0x3000, 0x1000, CRC(22f7a719) SHA1(eb000b606ecedd52bebbb232e661fb1ef205f8b0) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "eyes.5d",      0x0000, 0x1000, CRC(d6af0030) SHA1(652b779533e3f00e81cc102b78d367d503b06f33) )
	ROM_LOAD( "eyes.5e",      0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	PACMAN_SOUND_PROMS
ROM_END

/* E204 - Rockola presents and copyright notice are all removed. Where it should say "EYES" on title screen,
          it has some corrupt gfx instead. The rest is fine. */
ROM_START( eyesb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "eyesb.7d",     0x0000, 0x1000, CRC(7e7dd02d) SHA1(a0026fec57c089dd500efa107b0e884ed9edd4d6) )
	ROM_LOAD( "eyesb.7f",     0x1000, 0x1000, CRC(d6d73eb5) SHA1(b0c51afc09dd62bdda70710d57ae5b90a5e981ac) )
	ROM_LOAD( "eyesb.7h",     0x2000, 0x1000, CRC(951ad5dd) SHA1(9aed1044256070ae6555d561856d0bea454f97a8) )
	ROM_LOAD( "eyesb.7j",     0x3000, 0x1000, CRC(acc9cd8b) SHA1(d7fcf1b4b3466ee2187f82080634346a5427385e) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "eyes.5d",      0x0000, 0x1000, CRC(d6af0030) SHA1(652b779533e3f00e81cc102b78d367d503b06f33) )
	ROM_LOAD( "eyes.5e",      0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( ghohunt )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ghohunt.6e",   0x0000, 0x1000, CRC(b9e9965c) SHA1(8026e792fd474c2cc8ad8917da4d57a7930d30b4) )
	ROM_LOAD( "ghohunt.6f",   0x1000, 0x1000, CRC(9da32395) SHA1(8b4d7309de9dab4f58fcdbb29d98ca6c03a2b6fe) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ghohunt.5e",   0x0000, 0x1000, CRC(c69b5669) SHA1(c4f641ff218aee92dd784968b3e1a030c85d61f6) )
	ROM_LOAD( "ghohunt.5f",   0x1000, 0x1000, CRC(3ad83b13) SHA1(d2ce7ab45cb540f35cb23264e7628ac0ee6b8559) )

	PACMAN_PROMS
ROM_END

ROM_START( kangaroh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "kangaroh.1",   0x0000, 0x1000, CRC(bcdd9dff) SHA1(0243d9aaeaa82afb04d40eee192fdd65197fb87e) )
	ROM_LOAD( "kangaroh.2",   0x1000, 0x1000, CRC(1a381ee4) SHA1(04c6ceb5baa9f94cfd7235a89adf0bc0dbc8b4d1) )
	ROM_LOAD( "kangaroh.3",   0x2000, 0x1000, CRC(d4018655) SHA1(fb1720045c7c7ab58af894a5d2b86d289a6cd32d) )
	ROM_LOAD( "kangaroh.4",   0x3000, 0x1000, CRC(6c3f60eb) SHA1(51fc5df0ea3effdff765ea20d69f9f14575794cf) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "kangaroh.5e",  0x0000, 0x1000, CRC(c2afaa18) SHA1(138321b5efca381fb6be053aa76b37fdc7ad6afb) )
	ROM_LOAD( "kangaroh.5f",  0x1000, 0x1000, CRC(dd792694) SHA1(ded7dd8e5d184ec09d25aa3429c29e9a4ead6e92) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( ladybugh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",    0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",    0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",    0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "ladybugh.4",   0x3000, 0x1000, CRC(4ac90450) SHA1(7777777777777777777777777777777777777777) ) /* unavailable */
	ROM_LOAD( "lazybug.5",    0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",    0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",   0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",   0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( lazybug )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",    0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",    0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",    0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "lazybug.4",    0x3000, 0x1000, CRC(22a4e136) SHA1(e41e4d3cc0b3ca29ae8cb9591bf36e009c76f2b2) )
	ROM_LOAD( "lazybug.5",    0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",    0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",   0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",   0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( lazybug1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",    0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazyalt.2",    0x1000, 0x1000, CRC(3df8767b) SHA1(67ee2c2357a9a6e1f4d6ab95427d0c44dc3ebc1f) )
	ROM_LOAD( "lazybug.3",    0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "lazybug.4",    0x3000, 0x1000, CRC(22a4e136) SHA1(e41e4d3cc0b3ca29ae8cb9591bf36e009c76f2b2) )
	ROM_LOAD( "lazybug.5",    0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",    0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",   0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",   0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

ROM_START( mtturbo )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "maketrax.6e",  0x0000, 0x1000, CRC(0150fb4a) SHA1(ba41582d5432670654479b4bf6d938d2168858af) )
	ROM_LOAD( "maketrax.6f",  0x1000, 0x1000, CRC(77531691) SHA1(68a450bcc8d832368d0f1cb2815cb5c03451796e) )
	ROM_LOAD( "mtturbo.6h",   0x2000, 0x1000, CRC(77e0e153) SHA1(8be5cf8c0337e05eaf4635f19580d6c1477e6bcc) )
	ROM_LOAD( "maketrax.6j",  0x3000, 0x1000, CRC(0b4b5e0a) SHA1(621aece612df612065f776696956ef3671421fac) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pachello )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pachello.6e",  0x0000, 0x1000, CRC(A8227949) SHA1(793B7BA579C7E4771094D281C2589435A1BF2FDA) )
	ROM_LOAD( "pachello.6f",  0x1000, 0x1000, CRC(D5690D97) SHA1(51A4D63265F1D848359CA8C78BA111D72A3BC2CF) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmatri )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmatri.6e",  0x0000, 0x1000, CRC(29B0497E) SHA1(E45B225AABDF2F0549718885C02AE8A8EEF3BAEB) )
	ROM_LOAD( "pacmatri.6f",  0x1000, 0x1000, CRC(EAA7B145) SHA1(4C0ABF30F2C962B6EB2BDDA833236B9D58544A89) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmtest )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pactest.1",    0x0000, 0x1000, CRC(c0423406) SHA1(51630780dd7fa32a827d81a060906130fbe57345) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ptest.5e",     0x0000, 0x1000, CRC(a650e73e) SHA1(e51df8e976259ea8b431dd13b6bc4728ba213d19) )
	ROM_LOAD( "ptest.5f",     0x1000, 0x1000, CRC(51aa0303) SHA1(59fdfc5393e883d65bf00f8b3bbfb7173440dec3) )

	PACMAN_PROMS
ROM_END

ROM_START( pactetris ) // No text, no scores. When the game ends, it goes crazy.
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pactetris.1",  0x0000, 0x1929, CRC(f67b4604) SHA1(21dfb16e524734cf180b7a3c514575dfc9cb8bbc) )
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "puckman.5e",   0x0000, 0x0800, CRC(2066a0b7) SHA1(6d4ccc27d6be185589e08aa9f18702b679e49a4a) )
	ROM_LOAD( "puckman.5h",   0x0800, 0x0800, CRC(3591b89d) SHA1(79bb456be6c39c1ccd7d077fbe181523131fb300) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

ROM_START( piranha )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "pir1.bin",     0x0000, 0x0800, CRC(69a3e6ea) SHA1(c54e5d039a03d3cbee7a5e21bf1e23f4fd913ea6) )
	ROM_LOAD( "pir5.bin",     0x0800, 0x0800, CRC(245e753f) SHA1(4c1183b8449e4e7995f81079953fe0e251251c60) )
	ROM_LOAD( "pir2.bin",     0x1000, 0x0800, CRC(62cb6954) SHA1(0e01c8463b130ab5518ce23368ad028c86cd0a32) )
	ROM_LOAD( "pir6.bin",     0x1800, 0x0800, CRC(cb0700bc) SHA1(1f5e91791ea25eb58d26b9627e98e0b6c1d9becf) )
	ROM_LOAD( "pir3.bin",     0x2000, 0x0800, CRC(843fbfe5) SHA1(6671a3c55ef70447f2a127438e0c39857f8bf6b1) )
	ROM_LOAD( "pir7.bin",     0x2800, 0x0800, CRC(73084d5e) SHA1(cb04a4c9dbf1672ddf478d2fe92b0ffd0159bb9e) )
	ROM_LOAD( "pir4.bin",     0x3000, 0x0800, CRC(4cdf6704) SHA1(97af8bbd08896dffd73e359ec46843dd673c4c9c) )
	ROM_LOAD( "pir8.bin",     0x3800, 0x0800, CRC(b86fedb3) SHA1(f5eaf7ccc1ecaa2417bcc077561efca8e7cb691a) )

	ROM_REGION( 0x2000, "gfx1" , 0)
	ROM_LOAD( "pir9.bin",     0x0000, 0x0800, CRC(0f19eb28) SHA1(0335189a06be01b97ca376d3682ed54df9b121e8) )
	ROM_LOAD( "pir11.bin",    0x0800, 0x0800, CRC(5f8bdabe) SHA1(eb6a0515a381a885b087d165aaefb0277a223715) )
	ROM_LOAD( "pir10.bin",    0x1000, 0x0800, CRC(d19399fb) SHA1(c0a75a08f77adb9d0010511c4b6ea99324c33c50) )
	ROM_LOAD( "pir12.bin",    0x1800, 0x0800, CRC(cfb4403d) SHA1(1642a4917be0621ebf5f705c7f68a2b75d1c78d3) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "piranha.4a",   0x0020, 0x0100, CRC(08c9447b) SHA1(5e4fbfcc7179fc4b1436af9bb709ffc381479315) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( piranhah )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pr1.cpu",      0x0000, 0x1000, CRC(bc5ad024) SHA1(a3ed781b514a1068b24a7146a28f0a2adfaa2719) )
	ROM_LOAD( "pacman.6f",    0x1000, 0x1000, CRC(1a6fb2d4) SHA1(674d3a7f00d8be5e38b1fdc208ebef5a92d38329) )
	ROM_LOAD( "pr3.cpu",      0x2000, 0x1000, CRC(473c379d) SHA1(6e7985367c3e544b4cb98ba8291908df88eafe7f) )
	ROM_LOAD( "pr4.cpu",      0x3000, 0x1000, CRC(63fbf895) SHA1(d328bf3b8f307fb774614834edec211117148e64) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pr5.cpu",      0x0000, 0x0800, CRC(3fc4030c) SHA1(5e45f0c19cf96daa17afd2fa1c628d7ac7f4a79c) )
	ROM_LOAD( "pr7.cpu",      0x0800, 0x0800, CRC(30b9a010) SHA1(b0ba8b6cd430feb32d11d092e1959b9f5d240f1b) )
	ROM_LOAD( "pr6.cpu",      0x1000, 0x0800, CRC(f3e9c9d5) SHA1(709a75b2457f21f0f1a3d9e7f4c8579468ee5cad) )
	ROM_LOAD( "pr8.cpu",      0x1800, 0x0800, CRC(133d720d) SHA1(8af75ed9e115a996379acedd44d0c09332ec5a03) )

	PACMAN_PROMS
ROM_END

ROM_START( piranhao )
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "p1.bin",       0x0000, 0x0800, CRC(c6ce1bfc) SHA1(da145d67331cee292654a185fb09e773dd9d40cd) )
	ROM_LOAD( "p5.bin",       0x0800, 0x0800, CRC(a2655a33) SHA1(2253dcf5c8cbe278118aa1569cf456b13d8cf029) )
	ROM_LOAD( "pir2.bin",     0x1000, 0x0800, CRC(62cb6954) SHA1(0e01c8463b130ab5518ce23368ad028c86cd0a32) )
	ROM_LOAD( "pir6.bin",     0x1800, 0x0800, CRC(cb0700bc) SHA1(1f5e91791ea25eb58d26b9627e98e0b6c1d9becf) )
	ROM_LOAD( "pir3.bin",     0x2000, 0x0800, CRC(843fbfe5) SHA1(6671a3c55ef70447f2a127438e0c39857f8bf6b1) )
	ROM_LOAD( "pir7.bin",     0x2800, 0x0800, CRC(73084d5e) SHA1(cb04a4c9dbf1672ddf478d2fe92b0ffd0159bb9e) )
	ROM_LOAD( "p4.bin",       0x3000, 0x0800, CRC(9363a4d1) SHA1(4cb4a86d92a1f9bf233cac01aa266485a8bb7a34) )
	ROM_LOAD( "p8.bin",       0x3800, 0x0800, CRC(2769979c) SHA1(581592da26199b325de51791ddab66b474ab0413) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "p9.bin",       0x0000, 0x0800, CRC(94eb7563) SHA1(c99741ce1aebdfb89628fbfaecf5ae6b2719a0ca) )
	ROM_LOAD( "p11.bin",      0x0800, 0x0800, CRC(a3606973) SHA1(72297e1a33102c6a48b4c65f2a0b9bfc75a2df36) )
	ROM_LOAD( "p10.bin",      0x1000, 0x0800, CRC(84165a2c) SHA1(95b24620fbf9bd0ec4dd2aeeb6d9305bd475dce2) )
	ROM_LOAD( "p12.bin",      0x1800, 0x0800, CRC(2699ba9e) SHA1(b91ff586defe65b200bea5ade7374c2c7579cd80) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "piranha.4a",   0x0020, 0x0100, CRC(08c9447b) SHA1(5e4fbfcc7179fc4b1436af9bb709ffc381479315) )

	PACMAN_SOUND_PROMS
ROM_END

ROM_START( pollen )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pollen.6e",      0x0000, 0x1000, CRC(cafb430f) SHA1(9ac81affec3c9f1dfbec722712f7e907905f5662) )
	ROM_LOAD( "pollen.6f",      0x1000, 0x1000, CRC(9ca9850b) SHA1(b2d48cba1a58e28f450aa4556139ac5021f7fa61) )
	ROM_LOAD( "pollen.6h",      0x2000, 0x1000, CRC(04108eb0) SHA1(4773af22762b2da2f992867bee706325a999c2b0) )
	ROM_LOAD_OPTIONAL( "pollen.1k",      0x3000, 0x0400, CRC(b9d2e5e6) SHA1(82546b7ecf9e46374b1af1ad32f582b4d35bddd1) )  // not used

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pollen.5e",      0x0000, 0x1000, CRC(c360bb29) SHA1(f220959c11a17d3f443743f88435553f9d7916c4) )
	ROM_LOAD( "pollen.5f",      0x1000, 0x1000, CRC(b049f6cd) SHA1(69bea38cf4c133c7f6ce8b52824d214e8d726c7c) )

	PACMAN_PROMS
ROM_END

ROM_START( ppong )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ppong.1",      0x0000, 0x1000, CRC(abf90af3) SHA1(785059f68b69d031e8017fc5c6f44d22208ba1d1) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ppong.5e",     0x0000, 0x1000, CRC(57fe0ace) SHA1(87b9e980c8c30b73ebf7c2750dd40806754bed51) )
	ROM_LOAD( "ppong.5f",     0x1000, 0x1000, CRC(8bbd8de5) SHA1(feec6c9d413c6767edf2a521da439b21ccdf3d9a) )

	PACMAN_PROMS
ROM_END

ROM_START( ppong2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "ppong2.1",     0x0000, 0x1000, CRC(b0d6d512) SHA1(1d1ccc0cf0436680fc3b2bbd1a77292506b7514b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "ppong.5e",     0x0000, 0x1000, CRC(57fe0ace) SHA1(87b9e980c8c30b73ebf7c2750dd40806754bed51) )
	ROM_LOAD( "ppong.5f",     0x1000, 0x1000, CRC(8bbd8de5) SHA1(feec6c9d413c6767edf2a521da439b21ccdf3d9a) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "ppong2.7f",    0x0000, 0x0020, CRC(e3a96194) SHA1(6eb54a64e9d1df6052fecb754852bd44dd9ed69e) )
	ROM_LOAD( "ppong2.4a",    0x0020, 0x0100, CRC(dbd17cc2) SHA1(072e20975ae0453df57cfc54c344238aa311c373) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "ppong2.1m",    0x0000, 0x0100, CRC(0d968558) SHA1(b376885ac8452b6cbf9ced81b1080bfd570d9b91) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

ROM_START( rainboh )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "rainboh.1",    0x0000, 0x1000, CRC(676b742e) SHA1(0ec4bd459533f3f66782d0b978a0972c66b69ed4) )
	ROM_LOAD( "rainboh.2",    0x1000, 0x1000, CRC(3c9f1ada) SHA1(8951170cf75e5397136193ed69d920d2b9744f29) )
	ROM_LOAD( "rainboh.3",    0x2000, 0x1000, CRC(fbad5604) SHA1(4cdc2a87712471ed2bba7dc215b584e9e2c37c39) )
	ROM_LOAD( "rainboh.4",    0x3000, 0x1000, CRC(1af32a9b) SHA1(9830816c11342b40c056d555d6208ef1c6537557) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "rainboh.5e",   0x0000, 0x1000, CRC(c3292da2) SHA1(11d2b8cb36dfa86c08f6072e603f18074d56c2c2) )
	ROM_LOAD( "rainboh.5f",   0x1000, 0x1000, CRC(2b7455d8) SHA1(9decb7671a97e7fd248cc05798119d82b7830097) )

	PACMAN_PROMS
ROM_END

ROM_START( scroller )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "scroller.6e",  0x0000, 0x1000, CRC(081b47c5) SHA1(dc3d1ae904dc56b31b585ac1022b46633c00b016) )
	ROM_LOAD( "scroller.6f",  0x1000, 0x1000, CRC(93ef7735) SHA1(9ea39352dc658cd6f40f9df0d3aca7cb411f86cf) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "scroller.5e",  0x0000, 0x1000, CRC(14473363) SHA1(2e8b73fc4737ac81e66f92ec2dd34eab8e2d6fe6) )
	ROM_LOAD( "scroller.5f",  0x1000, 0x1000, CRC(47e8e97e) SHA1(51904ab4d5c92870996e6f575f67778cab206c3d) )

	PACMAN_PROMS
ROM_END

ROM_START( seq1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "seq1.6e",      0x0000, 0x1000, CRC(7063B724) SHA1(3A291D26BDFBF5C895D5F6AA70FC164299E8D9F1) )
	ROM_LOAD( "seq1.6f",      0x1000, 0x1000, CRC(95DB4723) SHA1(A0C47EA05E14CEBC6493705CD2D46D1E3D12B23A) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( seq2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "seq2.6e",      0x0000, 0x1000, CRC(0EC135BE) SHA1(F2BA6F8EE4256420E6149853A34A2B11A3545EEA) )
	ROM_LOAD( "seq2.6f",      0x1000, 0x1000, CRC(1B95F321) SHA1(F9D76544CCE43CE779A1BF59A01AC00297BAA82C) )
	ROM_LOAD( "seq2.6h",      0x2000, 0x1000, CRC(5892650A) SHA1(7C63F42EA3F75FAA15C6F5864569CB6A8F8C7C79) )
	ROM_LOAD( "seq2.6j",      0x3000, 0x1000, CRC(8612016A) SHA1(4E144069F194D240E503B24DF9B10BCDE68B2C6C) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "at.5e",        0x0000, 0x1000, CRC(EEB359BF) SHA1(36F153B5CE53475FD144AF50E4BD67B0E0B3A01F) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958FEDF9) SHA1(4A937AC02216EA8C96477D4A15522070507FB599) )

	PACMAN_PROMS
ROM_END

ROM_START( snakes ) // No text, no scores. Need 2 joysticks (2nd stick is RDFG keys).
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "snakes2.6e",   0x0000, 0x1000, CRC(6737be9c) SHA1(69a04dc9e9f08d78a6ad0a206b2af9126bfbe40c) )
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "snakes2.5e",   0x0000, 0x1000, CRC(5fa96d40) SHA1(fea209c43bea8db56426c7c74d7c16954d195fea) )
	ROM_LOAD( "puckman.5f",   0x1000, 0x0800, CRC(9e39323a) SHA1(be933e691df4dbe7d12123913c3b7b7b585b7a35) )
	ROM_LOAD( "puckman.5j",   0x1800, 0x0800, CRC(1b1d9096) SHA1(53771c573051db43e7185b1d188533056290a620) )

	PACMAN_PROMS
ROM_END

/* No sound */
ROM_START( tinyworld )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tinyworld.1",  0x0000, 0x1000, CRC(b485503b) SHA1(c868587529f73b4e52a9a20bc04fd8a9a9543a1e) )
	ROM_LOAD( "tinyworld.2",  0x1000, 0x1000, CRC(37862299) SHA1(bfaec22fcb6c59b0e26980b68936ae2bf2312b40) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "tinyworld.5e", 0x0000, 0x1000, CRC(38db47a4) SHA1(ae85b7d39d36b92d519653d65132ff213f6caecc) )
	ROM_LOAD( "absurd.5f",    0x1000, 0x1000, CRC(ae9805ee) SHA1(fd90b8bb2492a39f27216e0cda09f9594a9dada7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "glob.7f",      0x0000, 0x0020, CRC(1f617527) SHA1(448845cab63800a05fcb106897503d994377f78f) )
	ROM_LOAD( "absurd.4a",    0x0020, 0x0100, CRC(9379a246) SHA1(88c84668f67afc4a5ff12e794ac37a0636bd7153) )

	PACMAN_SOUND_PROMS
ROM_END

/* note: the original rom was only 3154 (0xC52) bytes with a CRC of 0E67D062 - the one here is padded out to normal size */
ROM_START( pacmtest1 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "test.6e",      0x0000, 0x1000, CRC(fb645998) SHA1(ff57155533e1ec0fdc5c407dce8d270e86d33882) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( pacmtest2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pacmtest2.1",  0x0000, 0x0530, CRC(3c33cbeb) SHA1(b26f4fcad7042f93e6df29af73c3ee91a7333837) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pacman.5e",    0x0000, 0x1000, CRC(0c944964) SHA1(06ef227747a440831c9a3a613b76693d52a2f0a9) )
	ROM_LOAD( "pacman.5f",    0x1000, 0x1000, CRC(958fedf9) SHA1(4a937ac02216ea8c96477d4a15522070507fb599) )

	PACMAN_PROMS
ROM_END

ROM_START( wavybug )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "lazybug.1",    0x0000, 0x1000, CRC(8cee62ee) SHA1(f33b114f3c133cb62f962ca1b2586314b73c8270) )
	ROM_LOAD( "lazybug.2",    0x1000, 0x1000, CRC(c17a5571) SHA1(24bae96cf95f009825fd06916eeae8a06e2e35ae) )
	ROM_LOAD( "lazybug.3",    0x2000, 0x1000, CRC(ac53ee82) SHA1(a69a71ee936ae73d5a27025528ff2c666893866d) )
	ROM_LOAD( "wavybug.4",    0x3000, 0x1000, CRC(830c47fc) SHA1(bc802de8c89417c4df91c26c2f365e471ea5efaf) )
	ROM_LOAD( "lazybug.5",    0x8000, 0x1000, CRC(ba11a997) SHA1(8dde84e9071bfa3e60d522a02ae5bf4464e730cf) )
	ROM_LOAD( "lazybug.6",    0x9000, 0x1000, CRC(c8b79a5b) SHA1(abaa9c585da1ee62d4b79968bedb429a5bbb739b) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "lazybug.5e",   0x0000, 0x1000, CRC(4e72e4f5) SHA1(05188aa854034e5cd9779d4b7034ec0658ccc805) )
	ROM_LOAD( "lazybug.5f",   0x1000, 0x1000, CRC(35eaf3a5) SHA1(6a0e5a5289003b0d8ccb9dd585a1e12a4486977c) )

	PACMAN_PROMS
ROM_END

/* No sound */
ROM_START( zap )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zap.1",        0x0000, 0x1000, CRC(74e6e3a0) SHA1(3d953bfd57aa56f8fe61d1abd01db84d027df3bb) )
	ROM_LOAD( "zap.2",        0x1000, 0x1000, CRC(40e1884b) SHA1(ce303b6a6c9e765c256997c19f64e9ebfd8f435e) )
	ROM_LOAD( "zap.4",        0x3000, 0x1000, CRC(17c357e6) SHA1(6486f58592b784d53a5c14f59b25c8cd513b2a0d) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "zap.5e",       0x0000, 0x1000, CRC(a0578404) SHA1(329f3e869eecff96191642e54dbfc7b6b268f026) )
	ROM_LOAD( "zap.5f",       0x1000, 0x1000, CRC(039b5c78) SHA1(36a96c996f20d1b5fb4f674393a03d10cdf4a091) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "crush.4a",     0x0020, 0x0100, CRC(2bc5d339) SHA1(446e234df94d9ef34c3191877bb33dd775acfdf5) )

	PACMAN_SOUND_PROMS
ROM_END


/*************************************
 *
 *  Game drivers
 *
 *************************************/

/* Official MAME */

/*          rom        parent    machine   inp       init */
GAME( 1981, abscam,    puckman,  piranha,  mspacman, puckman_state, init_eyes,       ROT90,  "GL (US Billiards License)", "Abscam", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyes,      0,        pacman,   eyes,     puckman_state, init_eyes,       ROT90,  "Digitrex Techstar (Rock-ola license)", "Eyes (Digitrex Techstar)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, piranha,   puckman,  piranha,  mspacman, puckman_state, init_eyes,       ROT90,  "GL (US Billiards License)", "Piranha", MACHINE_SUPPORTS_SAVE )
GAME( 1981, piranhah,  puckman,  pacman,   mspacman, puckman_state, empty_init,      ROT90,  "hack", "Piranha (hack)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, piranhao,  puckman,  piranha,  mspacman, puckman_state, init_eyes,       ROT90,  "GL (US Billiards License)", "Piranha (older)", MACHINE_SUPPORTS_SAVE )


/* Dave Widel's Games - http://www.widel.com */

GAME( 2003, aa,        0,        widel,    mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Alien Armada", MACHINE_SUPPORTS_SAVE )
GAME( 2003, bace,      0,        widel,    mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Balloon Ace", MACHINE_SUPPORTS_SAVE )
GAME( 2003, dderby,    0,        widel,    mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Death Derby", MACHINE_SUPPORTS_SAVE )
GAME( 2003, kangaroh,  0,        woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Kagaroo (Qbertish) (incomplete)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2003, ladybugh,  lazybug,  woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Ladybug on Pacman Hardware", MACHINE_SUPPORTS_SAVE )
GAME( 1981, lazybug,   0,        woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Lazy Bug", MACHINE_SUPPORTS_SAVE )
GAME( 1981, lazybug1,  lazybug,  woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Lazy Bug (Slower)", MACHINE_SUPPORTS_SAVE )
GAME( 19??, pacmtest,  puckman,  woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Pacman Test Rom (set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, ppong,     0,        woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Ping Pong demo (Pacman Hardware)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, ppong2,    ppong,    woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Ping Pong demo (Pacman Hardware) v2", MACHINE_SUPPORTS_SAVE )
GAME( 2003, rainboh,   0,        pacman,   mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Rainbow (Incomplete)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 1981, wavybug,   lazybug,  woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Wavy Bug", MACHINE_SUPPORTS_SAVE )
GAME( 2003, zap,       0,        woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "David Widel", "Space Zap Tribute", MACHINE_SUPPORTS_SAVE )

/* Jerronimo's Progs - www.umlautllama.com */

GAME( 2006, absurd,    0,        pacman,   mspacman, puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Absurd!/QuadBlok (non-working alpha ver 3)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )
GAME( 2003, alpaca7,   alpaca8,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Alpaca v0.7 (Pacman Hardware)", MACHINE_SUPPORTS_SAVE )
GAME( 2003, alpaca8,   0,        pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Alpaca v0.8 (Pacman Hardware)", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pachello,  0,        pachack,  pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Hello, World!", MACHINE_SUPPORTS_SAVE )
GAME( 2001, pacmatri,  0,        pachack,  pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Matrix Effect", MACHINE_SUPPORTS_SAVE )
GAME( 2003, seq1,      0,        pachack,  pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "16 Step Simple Sequencer", MACHINE_SUPPORTS_SAVE )
GAME( 2003, seq2,      seq1,     pachack,  pacman0,  puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Sequencer and Music Player", MACHINE_SUPPORTS_SAVE )
GAME( 2012, tinyworld, 0,        pacman,   mspacman, puckman_state, empty_init,          ROT90,  "Scott Lawrence", "Tiny World 82 (demo)", MACHINE_IS_INCOMPLETE | MACHINE_SUPPORTS_SAVE )

/* Other Misc Hacks */

GAME( 2002, alienres,  0,        pacman,   pacman,   puckman_state, empty_init,          ROT90,  "MonstersGoBoom", "Alien Rescue", MACHINE_SUPPORTS_SAVE )
GAME( 2002, alienresp, alienres, pacman,   pacman,   puckman_state, empty_init,          ROT90,  "MonstersGoBoom", "Alien Rescue (non-working prototype)", MACHINE_SUPPORTS_SAVE )
GAME( 2002, crashh,    0,        woodpek,  mspacpls, puckman_state, empty_init,          ROT90,  "hack", "Crash (Pac-Man)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, mtturbo,   0,        maketrax, maketrax, puckman_state, init_maketrax,       ROT270, "Tim Arcadecollecting", "Make Trax (Turbo Hack)", MACHINE_SUPPORTS_SAVE ) // http://www.arcadecollecting.com/hacks/maketrax
GAME( 1999, pacmtest1, puckman,  pacman,   mspacpls, puckman_state, empty_init,          ROT90,  "David Caldwell", "Pacman Test Rom (set 2)", MACHINE_SUPPORTS_SAVE ) // http://www.porkrind.org/arcade/
GAME( 2020, pacmtest2, puckman,  pacman,   mspacpls, puckman_state, empty_init,          ROT90,  "Phil Murray", "Pacman Test Rom (set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyesb,     eyes,     pacman,   eyes,     puckman_state, init_eyes,           ROT90,  "bootleg", "Eyes (unknown bootleg)", MACHINE_SUPPORTS_SAVE )
GAME( 2016, ghohunt,   puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Hurray Banana", "Ghost Hunt", MACHINE_SUPPORTS_SAVE )
GAME( 2012, pactetris, puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Ben Leperchey", "Tetris on Pacman hardware (incomplete)", MACHINE_SUPPORTS_SAVE )
GAME( 2017, deathstar, puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Stefano Bodrato", "Death Star", MACHINE_SUPPORTS_SAVE )
GAME( 2019, deathstar2,puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Stefano Bodrato", "Death Star v2", MACHINE_SUPPORTS_SAVE )
GAME( 2017, scroller,  puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Hurray Banana", "Scroller", MACHINE_SUPPORTS_SAVE )
GAME( 2019, snakes,    puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Stefano Bodrato", "Snakes", MACHINE_SUPPORTS_SAVE )
GAME( 2021, 100doors,  puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Hurray Banana", "100 doors", MACHINE_SUPPORTS_SAVE )
GAME( 2023, pollen,    puckman,  pacman,   pacman0,  puckman_state, empty_init,          ROT90,  "Arlasoft", "Pollen Angel", MACHINE_SUPPORTS_SAVE )


/*************************************************************************************************************************/

// This is a homebrew mod of sprglbpg. The German text was changed to English, the roms split to 2k blocks, and the pacman sound prom was used.
// Eventually the cabinet was sold, bought by the DU and added to MAME. Only then did its past come to light with a letter to Mamedev from
// chaneman on 2020-12-07. And, so it was moved to HBMAME.
// Bugs: High scores aren't saved to high score table; it sets credits to 4 after game is over.
// Ctrl = start, also calls the elevator. Alt = cling to ceiling

static INPUT_PORTS_START( theglobp )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_COCKTAIL

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_COCKTAIL
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_BUTTON1 ) // and start1
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_BUTTON2 ) // and start2
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )

	PORT_START("DSW1")
	PORT_DIPNAME( 0x03, 0x03, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x03, "3" )
	PORT_DIPSETTING(    0x02, "4" )
	PORT_DIPSETTING(    0x01, "5" )
	PORT_DIPSETTING(    0x00, "6" )
	PORT_DIPNAME( 0x1c, 0x1c, DEF_STR( Difficulty ) )
	PORT_DIPSETTING(    0x1c, DEF_STR( Easiest ) )
	PORT_DIPSETTING(    0x18, DEF_STR( Very_Easy) )
	PORT_DIPSETTING(    0x14, DEF_STR( Easy ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Normal ) )
	PORT_DIPSETTING(    0x0c, DEF_STR( Difficult ) )
	PORT_DIPSETTING(    0x08, DEF_STR( Very_Difficult ) )
	PORT_DIPSETTING(    0x04, DEF_STR( Very_Hard ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Hardest ) )
	PORT_DIPNAME( 0x20, 0x00, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

ROM_START( theglobpb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "theglobpb.8",  0x0000, 0x0800, CRC(3fb1ab3d) SHA1(29b8600c86a5161c90e1797cfec86875c948cf5d) )
	ROM_LOAD( "theglobpb.4",  0x0800, 0x0800, CRC(554a0461) SHA1(149a0e317d91465c09fb3406073c331cc4e4aa95) )
	ROM_LOAD( "theglobpb.7",  0x1000, 0x0800, CRC(07a2faf7) SHA1(7f1e95ae94fc404b65ef4465e7f147dbd3093ffb) )
	ROM_LOAD( "theglobpb.3",  0x1800, 0x0800, CRC(b097cb29) SHA1(bfe750dffebcf6bc1b0acf5a4147fb445559b926) )
	ROM_LOAD( "theglobpb.6",  0x2000, 0x0800, CRC(b459ba66) SHA1(563259523a4e525eeb01c733fb3c192897725a45) )
	ROM_LOAD( "theglobpb.2",  0x2800, 0x0800, CRC(d8ef9f98) SHA1(caaefdda74d7415be28abacc192b57a87a72baf7) )
	ROM_LOAD( "theglobpb.5",  0x3000, 0x0800, CRC(7204e11d) SHA1(ca680a835edad78859b0b3bf54360b1963795850) )
	ROM_LOAD( "theglobpb.1",  0x3800, 0x0800, CRC(edac5b91) SHA1(8a6f29442370cca8114e7941a36747aa96e4f1bc) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "theglobpb.9",  0x0000, 0x0800, CRC(36408c76) SHA1(f5bb18e38de57adc2aed6211048d9f0ee0e58df7) )
	ROM_LOAD( "theglobpb.11", 0x0800, 0x0800, CRC(b8ba069c) SHA1(f8d8e40afd8214a6d951af8de2761703b0651f79) )
	ROM_LOAD( "theglobpb.10", 0x1000, 0x0800, CRC(e0478b4e) SHA1(9697c7fd92752d052aea4c46292b1b7cae28f606) )
	ROM_LOAD( "theglobpb.12", 0x1800, 0x0800, CRC(7c4456a4) SHA1(74f55ae921cdf8f1f7a866d75a63244187426f17) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "ic78.prm",     0x0000, 0x0020, CRC(1f617527) SHA1(448845cab63800a05fcb106897503d994377f78f) )
	ROM_LOAD( "ic88.prm",     0x0020, 0x0100, CRC(28faa769) SHA1(7588889f3102d4e0ca7918f536556209b2490ea1) )

	PACMAN_SOUND_PROMS
ROM_END

GAME( 1999, theglobpb, 0, pacman, theglobp, puckman_state,  empty_init,    ROT90,  "chaneman", "Super Glob (English conversion of sprglbpg)", MACHINE_SUPPORTS_SAVE )

static INPUT_PORTS_START( ponpoko )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	/* The 2nd player controls are used even in upright mode */
	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("DSW1")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Bonus_Life ) )
	PORT_DIPSETTING(    0x01, "10000" )
	PORT_DIPSETTING(    0x02, "30000" )
	PORT_DIPSETTING(    0x03, "50000" )
	PORT_DIPSETTING(    0x00, DEF_STR( None ) )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x00, "0" )
	PORT_DIPSETTING(    0x04, "1" )
	PORT_DIPSETTING(    0x08, "2" )
	PORT_DIPSETTING(    0x0c, "3" )
	PORT_DIPNAME( 0x30, 0x20, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "2" )
	PORT_DIPSETTING(    0x10, "3" )
	PORT_DIPSETTING(    0x20, "4" )
	PORT_DIPSETTING(    0x30, "5" )
	PORT_DIPNAME( 0x40, 0x40, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )

	PORT_START("DSW2")
	PORT_DIPNAME( 0x0f, 0x01, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x04, "A 3/1 B 3/1" )
	PORT_DIPSETTING(    0x0e, "A 3/1 B 1/2" )
	PORT_DIPSETTING(    0x0f, "A 3/1 B 1/4" )
	PORT_DIPSETTING(    0x02, "A 2/1 B 2/1" )
	PORT_DIPSETTING(    0x0d, "A 2/1 B 1/1" )
	PORT_DIPSETTING(    0x07, "A 2/1 B 1/3" )
	PORT_DIPSETTING(    0x0b, "A 2/1 B 1/5" )
	PORT_DIPSETTING(    0x0c, "A 2/1 B 1/6" )
	PORT_DIPSETTING(    0x01, "A 1/1 B 1/1" )
	PORT_DIPSETTING(    0x06, "A 1/1 B 4/5" )
	PORT_DIPSETTING(    0x05, "A 1/1 B 2/3" )
	PORT_DIPSETTING(    0x0a, "A 1/1 B 1/3" )
	PORT_DIPSETTING(    0x08, "A 1/1 B 1/5" )
	PORT_DIPSETTING(    0x09, "A 1/1 B 1/6" )
	PORT_DIPSETTING(    0x03, "A 1/2 B 1/2" )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )  /* Most likely unused */
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )  /* Most likely unused */
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x40, 0x00, DEF_STR( Demo_Sounds ) )
	PORT_DIPSETTING(    0x40, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x80, DEF_STR( Unknown ) )  /* Most likely unused */
	PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
INPUT_PORTS_END

void puckman_state::init_ponpoko()
{
	/* The gfx data is swapped wrt the other Pac-Man hardware games. */
	/* Here we revert it to the usual format. */

	int i, j;
	uint8_t *RAM, temp;
	int length = memregion("gfx1")->bytes()/2;

	/* Characters */
	RAM = memregion("gfx1")->base();
	for (i = 0;i < length;i += 0x10)
	{
		for (j = 0; j < 8; j++)
		{
			temp          = RAM[i+j+0x08];
			RAM[i+j+0x08] = RAM[i+j+0x00];
			RAM[i+j+0x00] = temp;
		}
	}

	/* Sprites */
	RAM = memregion("gfx1")->base()+length;
	for (i = 0;i < length;i += 0x20)
	{
		for (j = 0; j < 8; j++)
		{
			temp          = RAM[i+j+0x18];
			RAM[i+j+0x18] = RAM[i+j+0x10];
			RAM[i+j+0x10] = RAM[i+j+0x08];
			RAM[i+j+0x08] = RAM[i+j+0x00];
			RAM[i+j+0x00] = temp;
		}
	}
}

ROM_START( chuckie )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "chuckie.1",    0x0000, 0x1000, CRC(f2ba04fd) SHA1(30fd7256ac1706db94dfd0f7e7518b36197f4a32) )
	ROM_LOAD( "chuckie.2",    0x1000, 0x1000, CRC(161510d3) SHA1(ad77509495a03466554f2388a9651a6183c16066) )
	ROM_LOAD( "chuckie.3",    0x2000, 0x1000, CRC(5c594671) SHA1(598aa268b45a04e4dafbf7271278b06271b31e50) )
	ROM_LOAD( "chuckie.4",    0x3000, 0x1000, CRC(25d4fc4f) SHA1(42f8424a8059248bb57e054e3bd129e995b94622) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "chuckie.9",    0x0000, 0x1000, CRC(7394bd6d) SHA1(0a4973636d1366872b1ce1e77772d6fef604922f) )
	ROM_LOAD( "chuckie.10",   0x1000, 0x1000, CRC(0024afe1) SHA1(bd6ad919a9e116d08b030cd55597900bae6fd7e3) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "chuckie.7f",   0x0000, 0x0020, CRC(f2f2cbfb) SHA1(b00207e379809168c0420d4d2ce5ee49a46e1bd7) )
	ROM_LOAD( "82s126.4a",    0x0020, 0x0100, CRC(3eb3a8e4) SHA1(19097b5f60d1030f8b82d9f1d3a241f93e5c75d6) )

	PACMAN_SOUND_PROMS
ROM_END

GAME( 2023, chuckie, 0, woodpek, ponpoko, puckman_state, init_ponpoko, ROT0, "Arlasoft", "Chuckie Egg", MACHINE_SUPPORTS_SAVE )

static INPUT_PORTS_START( paintrlr )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY
	PORT_DIPNAME( 0x10, 0x00, DEF_STR( Cabinet ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Cocktail ) )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_COIN1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_COIN2 )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE1 )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_4WAY PORT_COCKTAIL
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection in Make Trax */
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection in Make Trax */

	PORT_START("DSW1")
	PORT_DIPNAME( 0x03, 0x01, DEF_STR( Coinage ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0c, 0x00, DEF_STR( Lives ) )
	PORT_DIPSETTING(    0x00, "3" )
	PORT_DIPSETTING(    0x04, "4" )
	PORT_DIPSETTING(    0x08, "5" )
	PORT_DIPSETTING(    0x0c, "6" )
	PORT_DIPNAME( 0x10, 0x10, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x10, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x20, 0x20, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_BIT( 0xc0, IP_ACTIVE_HIGH, IPT_UNUSED )  /* Protection in Make Trax */

	PORT_START("DSW2")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

ROM_START( crush )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "crushkrl.6e",  0x0000, 0x1000, CRC(a8dd8f54) SHA1(4e3a973ea74a9e145c6997513b98fc80aa478442) )
	ROM_LOAD( "crushkrl.6f",  0x1000, 0x1000, CRC(91387299) SHA1(3ad8c28e02c45667e32860953b157832445a82c8) )
	ROM_LOAD( "crushkrl.6h",  0x2000, 0x1000, CRC(d4455f27) SHA1(53f8ffc28be664fa8a2d756b4c70045a3f041bea) )
	ROM_LOAD( "crushkrl.6j",  0x3000, 0x1000, CRC(d59fc251) SHA1(024605e4485b0ac826217256e5356ed9a6c8ef34) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( crush2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "tp1",          0x0000, 0x0800, CRC(f276592e) SHA1(68ebb7d9f70af868d99ec42c26bc55a54ba1f22c) )
	ROM_LOAD( "tp5a",         0x0800, 0x0800, CRC(3d302abe) SHA1(8ca5cd82d099b55e20f785489158231a1d99a430) )
	ROM_LOAD( "tp2",          0x1000, 0x0800, CRC(25f42e70) SHA1(66de8203c364fd90e8a2b5749c2e40665b2f5830) )
	ROM_LOAD( "tp6",          0x1800, 0x0800, CRC(98279cbe) SHA1(84b5e64bdbc25afab9b6f53e1719640e21a6feba) )
	ROM_LOAD( "tp3",          0x2000, 0x0800, CRC(8377b4cb) SHA1(f828a177f22db9093a00c31e39e16214ce0dc6de) )
	ROM_LOAD( "tp7",          0x2800, 0x0800, CRC(d8e76c8c) SHA1(7c3d7eb07b9256130141f71eba722f7823fd4c32) )
	ROM_LOAD( "tp4",          0x3000, 0x0800, CRC(90b28fa3) SHA1(ff58d2dfb016397daabe2996bc3a7b63d28a4cca) )
	ROM_LOAD( "tp8",          0x3800, 0x0800, CRC(10854e1b) SHA1(b3b9066d9a43796185c00ae12f7bb2bbf42e3a07) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "tpa",          0x0000, 0x0800, CRC(c7617198) SHA1(95b204af0345163f93811cc770ee0ca2851a39c1) )
	ROM_LOAD( "tpc",          0x0800, 0x0800, CRC(e129d76a) SHA1(c9256795c6d0929ade1f24b372dadc2a2b88d897) )
	ROM_LOAD( "tpb",          0x1000, 0x0800, CRC(d1899f05) SHA1(dce755511b6262b984a2bca329f454892e486a09) )
	ROM_LOAD( "tpd",          0x1800, 0x0800, CRC(d35d1caf) SHA1(65dd7861e05651485626465dc97215fed58db551) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( crush3 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1.6e",  0x0000, 0x1000, CRC(50a1a776) SHA1(bdd5c2b0ce76744dedc3ff527a1f4fcfa38d193a) )
	ROM_LOAD( "2.6f",  0x1000, 0x1000, CRC(5b03c1f8) SHA1(11fffe37817739bfe4524278a6ec4b3555b088a0) )
	ROM_LOAD( "3.6h",  0x2000, 0x1000, CRC(ae5b39fb) SHA1(bf144f14baa3db5fc407488750183749d2b1ca8d) )
	ROM_LOAD( "4.6j",  0x3000, 0x1000, CRC(ddf63743) SHA1(34c8338bc7b14b200453febc7a90af8cb9416527) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5.5e",  0x0000, 0x1000, CRC(b1c86cd7) SHA1(4e9fdd37e99426bfb9da19ca41f1001af06b9ea2) )
	ROM_LOAD( "6.5f",  0x1000, 0x1000, CRC(b5c14376) SHA1(2c8c57f96c51f12f73daf65dc2a73e8185aaacea) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(ff344446) SHA1(45eb37533da8912645a089b014f3b3384702114a) ) // sldh w/crush4
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s129.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s129.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( crush4 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "unkmol.4e",    0x0000, 0x0800, CRC(49150ddf) SHA1(5a20464a40d1d48606664779c85a7679073d7954) )
	ROM_LOAD( "unkmol.6e",    0x0800, 0x0800, CRC(21f47e17) SHA1(1194b5e8b0cce1f480acda3cb6c1fc65988bdc80) )
	ROM_LOAD( "unkmol.4f",    0x1000, 0x0800, CRC(9b6dd592) SHA1(6bb1b7ed95a7a8682a6ab58fa9f02c34beea8cd4) )
	ROM_LOAD( "unkmol.6f",    0x1800, 0x0800, CRC(755c1452) SHA1(a2da17ed0e526dad4d53d332467a3dfd3b2a8cab) )
	ROM_LOAD( "unkmol.4h",    0x2000, 0x0800, CRC(ed30a312) SHA1(15855904422eb603e5c5465bd038a3e8c666c10d) )
	ROM_LOAD( "unkmol.6h",    0x2800, 0x0800, CRC(fe4bb0eb) SHA1(70e480a75421ee0832456f1d30bf45a702192625) )
	ROM_LOAD( "unkmol.4j",    0x3000, 0x0800, CRC(072b91c9) SHA1(808df98c0cfd2367a39e06f30f920fd14887d922) )
	ROM_LOAD( "unkmol.6j",    0x3800, 0x0800, CRC(66fba07d) SHA1(4944d69a38fd823dad38b70433848017ae7027d7) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "unkmol.5e",    0x0000, 0x0800, CRC(338880a0) SHA1(beba1c71291394442b04fa5f4e1b833d7cf0fa8a) )
	ROM_LOAD( "unkmol.5h",    0x0800, 0x0800, CRC(4ce9c81f) SHA1(90a695ce4a45bde62bdbf09724a3ec6b45674660) )
	ROM_LOAD( "unkmol.5f",    0x1000, 0x0800, CRC(752e3780) SHA1(5730ebd8091eba5ac32ddd9db2f42d718b088753) )
	ROM_LOAD( "unkmol.5j",    0x1800, 0x0800, CRC(6e00d2ac) SHA1(aa3f1f3a3b6899bea717d97e4817b13159e552e5) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) ) // sldh w/crush3
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( crushbl )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "cr1.bin",  0x0000, 0x1000, CRC(e2e84cd1) SHA1(3fc5a9aa3ee219b386a1d0622547c77aca27533d) )
	ROM_LOAD( "cr2.bin",  0x1000, 0x1000, CRC(ec020e6f) SHA1(eef9008c38a68ed20c1e3596016d97d4e72de9f2) )
	ROM_LOAD( "cr3.bin",  0x2000, 0x1000, CRC(d4455f27) SHA1(53f8ffc28be664fa8a2d756b4c70045a3f041bea) ) // matches original
	ROM_LOAD( "cr4.bin",  0x3000, 0x1000, CRC(9936ae06) SHA1(80aff9a12dab97e9d5818f7a7fac54dc52b579d4) )

	/* no other roms in this set */
	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( crushrlf )
	ROM_REGION( 0x10000, "maincpu", 0 )

	ROM_LOAD( "pin1cc_6e.bin",          0x0400, 0x0400, CRC(65e469cf) SHA1(baeb5ba0ca0d78bca07f7830269f9c079f36d425) )
	ROM_CONTINUE(0x0000,0x0400)
	ROM_LOAD( "pin5cc_6k.bin",          0x0c00, 0x0400, CRC(15f0415b) SHA1(90c663387a81ad206874a531d9fe631ac0175975) )
	ROM_CONTINUE(0x0800,0x0400)
	ROM_LOAD( "pin2cc_6f.bin",          0x1400, 0x0400, CRC(653f726d) SHA1(3121315cf3e8be86d29687f29fc514e29dc64a02) )
	ROM_CONTINUE(0x1000,0x400)
	ROM_LOAD( "pin6cc_6m.bin",          0x1c00, 0x0400, CRC(4536ea5b) SHA1(6e0b22dd05a76644b13f1c71f771d686cd411eea) )
	ROM_CONTINUE(0x1800,0x400)
	ROM_LOAD( "pin3cc_6h.bin",          0x2400, 0x0400, CRC(55e15863) SHA1(bcbf4e5a268739c906e5c400e639e0e055799d47) )
	ROM_CONTINUE(0x2000,0x400)
	ROM_LOAD( "pin7cc_6n.bin",          0x2c00, 0x0400, CRC(409111ec) SHA1(ba98cfc1cce8627d11fda4954c3776d0b90cb584) )
	ROM_CONTINUE(0x2800,0x400)
	ROM_LOAD( "pin4cc_6j.bin",          0x3400, 0x0400, CRC(4fc4b582) SHA1(cb73b5f9171ba493afdfced0baeef9bb6bdb428d) )
	ROM_CONTINUE(0x3000,0x400)
	ROM_LOAD( "pin8cc_6p.bin",          0x3c00, 0x0400, CRC(0d97a047) SHA1(d0024a87a7530246bfbef7d1603b599e2f168973) )
	ROM_CONTINUE(0x3800,0x400)

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pin9cc_5e.bin",        0x0000, 0x0800, CRC(b6551507) SHA1(a544e6afda0dd1bea526cb94b9c456d923054698))
	ROM_LOAD( "pin11cc_5h.bin",       0x0800, 0x0800, CRC(e129d76a) SHA1(c9256795c6d0929ade1f24b372dadc2a2b88d897))
	ROM_LOAD( "pin10cc_5f.bin",       0x1000, 0x0800, CRC(d106da36) SHA1(a086e4874edf2f1a8bc945bed0e51424d92beaf2))
	ROM_LOAD( "pin12cc_5j.bin",       0x1800, 0x0800, CRC(d35d1caf) SHA1(65dd7861e05651485626465dc97215fed58db551) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( eyes2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "g38201.7d",    0x0000, 0x1000, CRC(2cda7185) SHA1(7ec3ee9bb90e6a1d83ad3aa12fd62184e07b1399) )
	ROM_LOAD( "g38202.7e",    0x1000, 0x1000, CRC(b9fe4f59) SHA1(2d97dc1a0458b406ca0c50d6b8bd0dbe58d21464) )
	ROM_LOAD( "g38203.7f",    0x2000, 0x1000, CRC(d618ba66) SHA1(76d93d8bc09bafac464ebfd002869e21535a365b) )
	ROM_LOAD( "g38204.7h",    0x3000, 0x1000, CRC(cf038276) SHA1(bcf4e129a151e2245e630cf865ce6cb009b405a5) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "g38205.5d",    0x0000, 0x1000, CRC(03b1b4c7) SHA1(a90b2fbaee2888ee4f0bcdf80a069c8594ef5ea1) )  /* this one has a (c) sign */
	ROM_LOAD( "g38206.5e",    0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( eyesb2 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1.bin",           0x0000, 0x0800, CRC(339d279a) SHA1(bc2a7801b9f94782f260346255f84a0e84729f01) )
	ROM_LOAD( "5.bin",           0x0800, 0x0800, CRC(1b68a61d) SHA1(348ef7b19934b000b03c74bec4796f1089dfc5eb) )
	ROM_LOAD( "2.bin",           0x1000, 0x0800, CRC(d4f9aaf8) SHA1(1e450a7ecf42f9bc2d58823907930dd7aa454215) )
	ROM_LOAD( "6.bin",           0x1800, 0x0800, CRC(6b41bb80) SHA1(aa3555833a2e8e596e126a749fe12853e4aa05b1) )
	ROM_LOAD( "3.bin",           0x2000, 0x0800, CRC(748e0e48) SHA1(857de6f7fe79a2613a93fb3efd981edb64c0190e) )
	ROM_LOAD( "7.bin",           0x2800, 0x0800, CRC(7b7f4a74) SHA1(373229de4ed25d577fe15deb3b6bc36786988284) )
	ROM_LOAD( "4.bin",           0x3000, 0x0800, CRC(367a3884) SHA1(d55d7eca56412661adac1849a398298670e86d15) )
	ROM_LOAD( "8.bin",           0x3800, 0x0800, CRC(2baaadae) SHA1(7b20ed5935e9a4f793f690bab2a6bc0db44d12af) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "9.bin",           0x0000, 0x0800, CRC(342c0653) SHA1(d07e3d4528b72e54a1b5dbed009cce765a5a086f) )
	ROM_LOAD( "12.bin",          0x0800, 0x0800, CRC(99af4b30) SHA1(6a0939ff2fa7ae39a960dd4d9f9b7c01f57647c5) )
	ROM_LOAD( "10.bin",          0x1000, 0x0800, CRC(b247b82c) SHA1(8c10a8ef5e79b0b5fefad6eb77bfa68a0ca18035) )
	ROM_LOAD( "11.bin",          0x1800, 0x0800, CRC(aaa7a537) SHA1(571d981ed2aad62d7c7f2798e9084228d45523d4) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "7051.bin",        0x0000, 0x0020, CRC(2c3cc909) SHA1(32d68d4cfdf9f3e7351353428d268c763e809c63) ) // fixed 3x bytes with inverse second half
	ROM_LOAD( "7051-3.bin",      0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",       0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) ) // not dumped, taken from parent
	ROM_LOAD( "7051-2.bin",      0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) ) // two of these?
ROM_END

ROM_START( eyeszac ) /* All ROMs / PROMs dumped and verified from actual PCB */
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "1.7e",         0x0000, 0x1000, CRC(e555b265) SHA1(ef3138d3d52b678bf26e8c2299719cca08eef4bf) ) // 2532
	ROM_LOAD( "2.7f",         0x1000, 0x1000, CRC(d6d73eb5) SHA1(b0c51afc09dd62bdda70710d57ae5b90a5e981ac) ) // 2532
	ROM_LOAD( "3.7h",         0x2000, 0x1000, CRC(604c940c) SHA1(a611c30e42492fc35d2a215dfc8c3ebda82909f7) ) // 2532
	ROM_LOAD( "4.7i",         0x3000, 0x1000, CRC(acc9cd8b) SHA1(d7fcf1b4b3466ee2187f82080634346a5427385e) ) // 2532

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "5.5d",         0x0000, 0x1000, CRC(d6af0030) SHA1(652b779533e3f00e81cc102b78d367d503b06f33) ) // 2532
	ROM_LOAD( "6.5f",         0x1000, 0x1000, CRC(a42b5201) SHA1(2e5cede3b6039c7bd5230de27d02aaa3f35a7b64) ) // 2532

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )
ROM_END

/* It's just a decrypted version of Eyes with the copyright changes...
 roms marked with a comment were in the set but we're not using them */
ROM_START( eyeszacb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "zacb_11.bin",  0x0000, 0x0800, CRC(69c1602a) SHA1(47b0935406b7ee2f414de58da1d4e81c6277a0c2) ) // "no diagnostics, bad custom??" (unused)
	ROM_LOAD( "zacb_1.bin",   0x0000, 0x0800, CRC(a4a9d7a0) SHA1(f0b807d2fa347e50df52971aa7539a88f342bad6) )
	ROM_LOAD( "zacb_5.bin",   0x0800, 0x0800, CRC(c32b3f73) SHA1(80d2e987f0318b984e5c7c4d0b5faa262eebeca4) )
	ROM_LOAD( "zacb_2.bin",   0x1000, 0x0800, CRC(195b9473) SHA1(62eb16af38cc9004787dc55433ed3db11af44a4b) )
	ROM_LOAD( "zacb_6.bin",   0x1800, 0x0800, CRC(292886cb) SHA1(e77c3724c7cd8cd95014194ba4bb2f7e04afb0dd) )
//  ROM_LOAD( "33.bin",          0x2000, 0x0800, CRC(df983e1d) SHA1(7c06fc69b7d0424f7b9348649d5587ff4d6dfc2d) ) // alt rom with copyright removed (unused)
	ROM_LOAD( "zacb_3.bin",   0x2000, 0x0800, CRC(ff94b015) SHA1(6d8f43db3c98cadb35f70e3bff788e653dc132cd) )
	ROM_LOAD( "zacb_7.bin",   0x2800, 0x0800, CRC(9271c58c) SHA1(e6b8f1807c5852ae4e822d80719a4e8f8b036c31) )
	ROM_LOAD( "zacb_4.bin",   0x3000, 0x0800, CRC(965cf32b) SHA1(68cc573a24c74f2ab417d0330fc9523e77fda961) )
	ROM_LOAD( "zacb_8.bin",   0x3800, 0x0800, CRC(c254e92e) SHA1(023b45403ebc69c29516d77950dc69f05a1a130c) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "x.bin",           0x0000, 0x0800, CRC(59dce22e) SHA1(81eaef3e4d8299b5133b62d04460abfa519696f5) )
	ROM_LOAD( "c.bin",           0x0800, 0x0800, CRC(aaa7a537) SHA1(571d981ed2aad62d7c7f2798e9084228d45523d4) )
	ROM_LOAD( "b.bin",           0x1000, 0x0800, CRC(1969792b) SHA1(7c3e2ace75402ad227e6437785b7cfec4db88db8) )
	ROM_LOAD( "p.bin",           0x1800, 0x0800, CRC(99af4b30) SHA1(6a0939ff2fa7ae39a960dd4d9f9b7c01f57647c5) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "82s129.4a",    0x0020, 0x0100, CRC(d8d78829) SHA1(19820d1651423210083a087fb70ebea73ad34951) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( maketrax )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "maketrax.6e",  0x0000, 0x1000, CRC(0150fb4a) SHA1(ba41582d5432670654479b4bf6d938d2168858af) )
	ROM_LOAD( "maketrax.6f",  0x1000, 0x1000, CRC(77531691) SHA1(68a450bcc8d832368d0f1cb2815cb5c03451796e) )
	ROM_LOAD( "maketrax.6h",  0x2000, 0x1000, CRC(a2cdc51e) SHA1(80d80235cda3ce19c1dbafacf3d47b1325ad4728) )
	ROM_LOAD( "maketrax.6j",  0x3000, 0x1000, CRC(0b4b5e0a) SHA1(621aece612df612065f776696956ef3671421fac) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( maketrxb )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "maketrax.6e",  0x0000, 0x1000, CRC(0150fb4a) SHA1(ba41582d5432670654479b4bf6d938d2168858af) )
	ROM_LOAD( "maketrax.6f",  0x1000, 0x1000, CRC(77531691) SHA1(68a450bcc8d832368d0f1cb2815cb5c03451796e) )
	ROM_LOAD( "maketrxb.6h",  0x2000, 0x1000, CRC(6ad342c9) SHA1(5469f3952adc682725a71602b4a00a7751e48a99) )
	ROM_LOAD( "maketrxb.6j",  0x3000, 0x1000, CRC(be27f729) SHA1(0f7b873d33f751fa2fc54f9eede0598cb7d7f3c8) )

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "maketrax.5e",  0x0000, 0x1000, CRC(91bad2da) SHA1(096197d0cb6d55bf72b5be045224f4bd6a9cfa1b) )
	ROM_LOAD( "maketrax.5f",  0x1000, 0x1000, CRC(aea79f55) SHA1(279021e6771dfa5bd0b7c557aae44434286d91b7) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "82s123.7f",    0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "2s140.4a",     0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )    /* sound PROMs */
	ROM_LOAD( "82s126.1m",    0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "82s126.3m",    0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    /* timing - not used */
ROM_END

ROM_START( painter )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "pain1.6e",           0x0000, 0x1000, CRC(fb2eb6dc) SHA1(377075cb64c7ccc0bd2b0185848d4798428bceb2) )
	ROM_LOAD( "pain2.6f",           0x1000, 0x1000, CRC(39f92fb0) SHA1(2b58429664eadb8bb5100289182b274cc7e4688c) )
	ROM_LOAD( "pain3.6h",           0x2000, 0x1000, CRC(f80435b1) SHA1(7f41ecc2a91f8bdf0969f4f3fe96b48adb1010ba) )
	ROM_LOAD( "pain4-pennello2.6j", 0x3000, 0x1000, CRC(0cb678dc) SHA1(b1b9eadf22dc7985b39d414438092a7fc6c58955) ) // has strange strings in the second half, but seems to work fine

	ROM_REGION( 0x2000, "gfx1", 0 )
	ROM_LOAD( "pain5.5e", 0x0000, 0x1000, CRC(bd819afc) SHA1(2e8762c3c480aa669f7e87651ddfdbb965ea4211) )
	ROM_LOAD( "pain6.5f", 0x1000, 0x1000, BAD_DUMP CRC(014e5ed3) SHA1(8e01c640457515da89723215b19684ceb4556997) ) // BADADDR            xx-xxxxxxxxx, dumped with 3 different programmers with same result, but probably damaged ROM

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "mb7051.7f",   0x0000, 0x0020, CRC(ff344446) SHA1(45eb37533da8912645a089b014f3b3384702114a) )
	ROM_LOAD( "n82s129n.4a", 0x0020, 0x0100, CRC(63efb927) SHA1(5c144a613fc4960a1dfd7ead89e7fee258a63171) )

	ROM_REGION( 0x0200, "namco", 0 )
	ROM_LOAD( "mb7052.1m", 0x0000, 0x0100, CRC(a9cc86bf) SHA1(bbcec0570aeceb582ff8238a4bc8546a23430081) )
	ROM_LOAD( "mb7052.3m", 0x0100, 0x0100, CRC(77245b66) SHA1(0c4d0bee858b97632411c440bea6948a74759746) )    // Timing - not used
ROM_END


ROM_START( mspacmab3 ) // main PCB with GDP-02 auxiliary card
	ROM_REGION( 0x10000, "maincpu",0 )
	ROM_LOAD( "p1.6ef",  0x0000, 0x1000, CRC(50b38941) SHA1(c798fe6efe7ec6d15add74986d72d9ea8676d14a) ) // could also be d instead of p
	ROM_LOAD( "p2.7ef",  0x1000, 0x1000, CRC(195883b8) SHA1(3395fd2f9963b1809fc55a4f1c998673de75793b) ) // could also be d instead of p
	ROM_LOAD( "p3.bin",  0x2000, 0x1000, CRC(124a4507) SHA1(a7c82970ac53129c7a642322214adee4206298ff) ) // could also be d instead of p
	ROM_LOAD( "p4.bin",  0x3000, 0x1000, CRC(08ac65da) SHA1(745d9d054c33df96a7f27a1f4575f8770d92ac10) ) // could also be d instead of p
	ROM_LOAD( "d5.6fh",  0x8000, 0x1000, CRC(50b29f09) SHA1(27ca2cdf57b96d628b1811210d254b107e2f324e) )
	ROM_LOAD( "d6.6j",   0x9000, 0x0800, CRC(33b09ed9) SHA1(a2a3e069cce442c3b179315e90476cd431c604c2) )

	ROM_REGION( 0x2000, "gfx1" , 0 )
	ROM_LOAD( "d7.5de", 0x0000, 0x0800, CRC(b5d8c872) SHA1(d137b0cff8635a6a02f9334b7fa72dc1a623fe9b) )
	ROM_LOAD( "d9.5fh", 0x0800, 0x0800, CRC(9b2b936c) SHA1(d6f57d0ce6fba37d4838cdcace7e2e02c94b1ba1) )
	ROM_LOAD( "d8.5ef", 0x1000, 0x0800, CRC(a70a6ac4) SHA1(81b0c56697bb671cc43928133446da74e74b4982) )
	ROM_LOAD( "d10.5j", 0x1800, 0x0800, CRC(53368498) SHA1(0409288fe59c2bbf2730c31e4c7817392a544165) )

	ROM_REGION( 0x0120, "proms", 0 )
	ROM_LOAD( "6331.8h", 0x0000, 0x0020, CRC(2fc650bd) SHA1(8d0268dee78e47c712202b0ec4f1f51109b1f2a5) )
	ROM_LOAD( "6301.4a", 0x0020, 0x0100, CRC(720528b4) SHA1(1f8fee1b8dec02cb19e706ca807438cec512479c) ) // 11xxxxxxx = 0x00
	ROM_IGNORE(                  0x0100 )

	ROM_REGION( 0x0200, "namco", 0 ) // sound PROMs
	ROM_LOAD( "63s141.1k", 0x0000, 0x0100, CRC(459d2618) SHA1(14ba61caada575909b4dbd57e7342dc84722325d) )
	ROM_IGNORE(                    0x0100 ) // 1xxxxxxxx = 0x00
	ROM_LOAD( "63s141.3k", 0x0100, 0x0100, CRC(fcc24d5d) SHA1(7ae2523f92cccdbd8db8bda80c613a2f90220807) )  // timing - not used
	ROM_IGNORE(                    0x0100 ) // 11xxxxxxx = 0x00

	ROM_REGION( 0x0200, "gdp02_prom", 0 ) // currently not used by the emulation
	ROM_LOAD( "82s141.i14", 0x0000, 0x0200, CRC(8d43d0a6) SHA1(be4e0d86ce4c6f1833cc0efc18277b4e04dc458f) ) // 1ST AND 2ND HALF IDENTICAL
ROM_END

/*    YEAR  NAME            PARENT    MACHINE        INPUT       INIT             MONITOR COMPANY                 FULLNAME FLAGS */
GAME( 1981, crush,    0,        maketrax, maketrax, puckman_state,  init_maketrax, ROT90,  "Alpha Denshi Co. / Kural Samno Electric, Ltd.", "Crush Roller (set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, crush2,   crush,    pacman,   maketrax, puckman_state,  empty_init,    ROT90,  "Alpha Denshi Co. / Kural Esco Electric, Ltd.", "Crush Roller (set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, crush3,   crush,    maketrax, maketrax, puckman_state,  init_maketrax, ROT90,  "Alpha Denshi Co. / Kural Electric, Ltd.", "Crush Roller (set 3)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, crush4,   crush,    pacman,   maketrax, puckman_state,  init_eyes,     ROT90,  "Alpha Denshi Co. / Kural Electric, Ltd.", "Crush Roller (set 4)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, crushbl,  crush,    pacman,   maketrax, puckman_state,  empty_init,    ROT90,  "bootleg", "Crush Roller (bootleg set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, crushrlf, crush,    pacman,   maketrax, puckman_state,  empty_init,    ROT90,  "bootleg", "Crush Roller (Famare SA PCB)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyes2,    eyes,     pacman,   eyes,     puckman_state,  init_eyes,     ROT90,  "Techstar (Rock-Ola license)", "Eyes (US set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyeszac,  eyes,     pacman,   eyes,     puckman_state,  init_eyes,     ROT90,  "Techstar (Zaccaria license)", "Eyes (Italy)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyeszacb, eyes,     pacman,   eyes,     puckman_state,  empty_init,    ROT90,  "bootleg", "Eyes (bootleg set 2, decrypted)", MACHINE_SUPPORTS_SAVE )
GAME( 1982, eyesb2,   eyes,     pacman,   eyes,     puckman_state,  init_eyes,     ROT90,  "bootleg", "Eyes (bootleg set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, maketrax, crush,    maketrax, maketrax, puckman_state,  init_maketrax, ROT270, "Alpha Denshi Co. / Kural (Williams license)", "Make Trax (US set 1)", MACHINE_SUPPORTS_SAVE )
GAME( 1981, maketrxb, crush,    maketrax, maketrax, puckman_state,  init_maketrax, ROT270, "Alpha Denshi Co. / Kural (Williams license)", "Make Trax (US set 2)", MACHINE_SUPPORTS_SAVE )
GAME( 1984, painter,  crush,    pacman,   paintrlr, puckman_state,  empty_init,    ROT90,  "hack (Monshine Ent. Co.)", "Painter (hack of Crush Roller)", MACHINE_IMPERFECT_GRAPHICS | MACHINE_SUPPORTS_SAVE )
GAME( 1981, mspacmab3,puckman,  piranha,  mspacman, puckman_state,  init_eyes,     ROT90,  "bootleg", "Ms. Pac-Man (bootleg, set 3)",                             MACHINE_SUPPORTS_SAVE )
