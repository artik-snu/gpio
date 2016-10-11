Name: tizen-example
Version: 0
Release: 0
Summary: example
Source: %{name}-%{version}.tar.gz
License: TODO
BuildRequires: make
%description

A basic hello world example for TIZEN.
%prep

%setup -q -n %{name}-%{version}

%build

%install

%make_install

%clean

%files

%{_bindir}/*
