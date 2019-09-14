Name: lte-gtp-simulator
Version: master
Release: 1%{?dist}
Summary: A 3gpp GTP signaling simulator and load generator.

Group: Applications/Internet
License: ?
URL: https://github.com/nithinn/lte-gtp-simulator
Source0: https://github.com/nithinn/lte-gtp-simulator/archive/master.zip

BuildRequires: ncurses-devel

%description
LTE GTP Simulator is a 3gpp LTE GTPv2-C signaling simulator and gtp load generator. Any LTE GTP signaling scenario can easily be represented in XML and the simulator plays out the scenario. The simulator provides command line options to control the scenario execution. A scenario file, written in xml is a sequence of GTPC messages that is exchanged between between two LTE core network nodes. Each message, its informational elements etc are represented in XML. The scenario is then simulated by the GTP Simulator for a single UE or for multiple UEs in the form of load generator.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT/usr/bin
install -m 0755 gsim $RPM_BUILD_ROOT/usr/bin/gsim
mkdir -p $RPM_BUILD_ROOT/usr/share/lte-gtp-simulator/scenario
install -m 0644 scenario/mme.xml $RPM_BUILD_ROOT/usr/share/lte-gtp-simulator/scenario/mme.xml
install -m 0644 scenario/pgw.xml $RPM_BUILD_ROOT/usr/share/lte-gtp-simulator/scenario/pgw.xml
install -m 0644 scenario/sgw.xml $RPM_BUILD_ROOT/usr/share/lte-gtp-simulator/scenario/sgw.xml

%files
/usr/bin/gsim
/usr/share/lte-gtp-simulator/scenario/mme.xml
/usr/share/lte-gtp-simulator/scenario/pgw.xml
/usr/share/lte-gtp-simulator/scenario/sgw.xml
%doc

%changelog
* Sat Jun  9 2018 Oleg Gashev <gashev@gmail.com> - master
- Initial package.
