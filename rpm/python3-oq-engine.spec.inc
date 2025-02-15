# -*- coding: utf-8 -*-
# vim: syntax=spec
#
# Copyright (C) 2015-2022 GEM Foundation
#
# OpenQuake is free software: you can redistribute it and/or modify it
# under the terms of the GNU Affero General Public License as published
# by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# OpenQuake is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with OpenQuake. If not, see <http://www.gnu.org/licenses/>.

# Provide a custom prefix for oq
%define _oqprefix /opt/openquake
%define _oqvenv %{_oqprefix}/venv
%define _oqpython %{_oqprefix}/bin

# Make sure that the proper installation of python is used by macros
%define __python3 %{_oqpython}/python3
%define __python %{__python3}
%define installscript install.py

%define oqstable ##_stable_##
%define oqrepo ##_repo_##
%define oqversion ##_version_##
%define oqrelease ##_release_##
%define oqname python3-%{oqrepo}
%define oqtimestamp ##_timestamp_##

%if %{oqstable} == 1
%define oqformat %{oqrepo}-%{oqversion}
Release: %{oqrelease}
%else
%define oqformat %{oqrepo}-%{oqversion}-%{oqrelease}
Release: %{oqtimestamp}_%{oqrelease}
%endif

# ==========================================
# Descriptions, and metadata for subpackages
# ==========================================

Summary: Computes earthquake hazard and risk
Name: %{oqname}
Version: %{oqversion}
License: AGPLv3
Group: Applications/Engineering
BuildRoot: %{_tmppath}/python-%{oqformat}-buildroot
Prefix: %{_prefix}
BuildArch: noarch
Vendor: The GEM OpenQuake team <devops@openquake.org>
Url: http://github.com/gem/oq-engine


Requires: oq-python3 >= 3.8.0

Obsoletes: python-oq-hazardlib python-oq-engine
Provides: python-oq-hazardlib python-oq-engine

# =======================
# Source code and patches
# =======================

Source0: %{oqformat}.tar.gz

%description
OpenQuake is an open source application that allows users to
compute seismic hazard and seismic risk of earthquakes on a global scale.
Copyright (C) 2010-2022 GEM Foundation

%prep
%setup -n %{oqformat}

%build


# ======================================================
# Installing the built code:
# ======================================================

%install
#mkdir -p %{buildroot}%{_bindir}
mkdir -p %{buildroot}%{_oqprefix}
cp -a %{installscript} %{buildroot}%{_oqprefix}/

%files
%{_oqprefix}/%{installscript}

%post
echo STARTING: INSTALL OF OPENQUAKE ENGINE
%if %{oqstable} == 1
%{__python3} %{_oqprefix}/%{installscript} server
%else
%{__python3} %{_oqprefix}/%{installscript} server --version=master
%endif

%clean
rm -rf %{buildroot}

%preun
%if %{oqstable} == 1
%{__python3} %{_oqprefix}/%{installscript} server --remove
%else
%{__python3} %{_oqprefix}/%{installscript} server --version=master --remove
%endif

