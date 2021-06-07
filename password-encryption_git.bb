DESCRIPTION = "Password hash tool"
HOMEPAGE = "https://github.com/cpb-/password-encryption"
AUTHOR = "Christophe BLAESS"
LICENSE="GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

S = "${WORKDIR}/git"

SRC_URI = "git://github.com/cpb-/password-encryption.git;protocol=git"
SRCREV = "master"
DEPENDS += "libxcrypt"
RDEPENDS_${PN} += "libxcrypt"

do_compile() {
	oe_runmake
}

do_install() {
	install -m 0755 -d ${D}${bindir}
	install -m 0755 ${S}/hash ${D}${bindir}/
}


