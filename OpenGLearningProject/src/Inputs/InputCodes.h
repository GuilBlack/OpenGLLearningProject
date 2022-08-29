#pragma once
#include "pch.h"

using KeyCode =		uint16_t;
using MouseCode =	uint16_t;

enum : KeyCode
{
	//signs
	KSpace =		32,
	KApostrophe =	39, /* ' */
	KComma =		44, /* , */
	KMinus =		45, /* - */
	KPeriod =		46, /* . */
	KSlash =		47, /* / */
	KSemicolon =	59, /* ; */
	KEqual =		61, /* = */
	KLeftBracket =	91, /* [ */
	KBackslash =	92, /* \ */
	KRightBracket =	93, /* ] */
	KGraveAccent =	96, /* ` */
	
	// Numbers
	K0 =			48,
	K1 =			49,
	K2 =			50,
	K3 =			51,
	K4 =			52,
	K5 =			53,
	K6 =			54,
	K7 =			55,
	K8 =			56,
	K9 =			57,

	//Alphabetic character
	KA =			65,
	KB =			66,
	KC =			67,
	KD =			68,
	KE =			69,
	KF =			70,
	KG =			71,
	KH =			72,
	KI =			73,
	KJ =			74,
	KK =			75,
	KL =			76,
	KM =			77,
	KN =			78,
	KO =			79,
	KP =			80,
	KQ =			81,
	KR =			82,
	KS =			83,
	KT =			84,
	KU =			85,
	KV =			86,
	KW =			87,
	KX =			88,
	KY =			89,
	KZ =			90,

	KWorld1 =		161, /* non-US #1 */
	KWorld2 =		162, /* non-US #2 */

	// Function keys
	KEscape =		256,
	KEnter =		257,
	KTab =			258,
	KBackspace =	259,
	KInsert =		260,
	KDelete =		261,
	KRight =		262,
	KLeft =			263,
	KDown =			264,
	KUp =			265,
	KPageUp =		266,
	KPageDown =		267,
	KHome =			268,
	KEnd =			269,
	KCapsLock =		280,
	KScrollLock =	281,
	KNumLock =		282,
	KPrintScreen =	283,
	KPause =		284,
	KF1 =			290,
	KF2 =			291,
	KF3 =			292,
	KF4 =			293,
	KF5 =			294,
	KF6 =			295,
	KF7 =			296,
	KF8 =			297,
	KF9 =			298,
	KF10 =			299,
	KF11 =			300,
	KF12 =			301,
	KF13 =			302,
	KF14 =			303,
	KF15 =			304,
	KF16 =			305,
	KF17 =			306,
	KF18 =			307,
	KF19 =			308,
	KF20 =			309,
	KF21 =			310,
	KF22 =			311,
	KF23 =			312,
	KF24 =			313,
	KF25 =			314,

	// Keypad
	KP0 =			320,
	KP1 =			321,
	KP2 =			322,
	KP3 =			323,
	KP4 =			324,
	KP5 =			325,
	KP6 =			326,
	KP7 =			327,
	KP8 =			328,
	KP9 =			329,
	KPDecimal =		330,
	KPDivide =		331,
	KPMultiply =	332,
	KPSubtract =	333,
	KPAdd =			334,
	KPEnter =		335,
	KPEqual =		336,

	// Modifiers
	KLeftShift =	340,
	KLeftControl =	341,
	KLeftAlt =		342,
	KLeftSuper =	343,
	KRightShift =	344,
	KRightControl =	345,
	KRightAlt =		346,
	KRightSuper =	347,
	KMenu =			348
};

enum : MouseCode
{
	MButton0 =		0,
	MButton1 =		1,
	MButton2 =		2,
	MButton3 =		3,
	MButton4 =		4,
	MButton5 =		5,
	MButton6 =		6,
	MButton7 =		7,

	MButtonLast =	MButton7,
	MButtonLeft =	MButton0,
	MButtonRight =	MButton1,
	MButtonMiddle =	MButton2
};