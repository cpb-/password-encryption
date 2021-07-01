DESCRIPTION = "Password hasher tool"
HOMEPAGE = "https://github.com/cpb-/password-encryption"
AUTHOR = "Christophe BLAESS"
LICENSE="GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=fed54355545ffd980b814dab4a3b312c"

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
	install -m 0755 ${S}/hasher ${D}${bindir}/
}
