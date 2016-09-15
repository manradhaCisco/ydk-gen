#  ----------------------------------------------------------------
# Copyright 2016 Cisco Systems
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ------------------------------------------------------------------

import os
import shutil
from subprocess import call

class LibDownloader(object):

    def __init__(self, home_dir):
        self.home_dir = home_dir

        self.download_dir =     '%s/.temp'      % self.home_dir
        self.objects_dir =      '%s/.libs'  % self.home_dir

    def download(self):
        download_dir = self.download_dir
        objects_dir = self.objects_dir

        self._init_archive_dirs(download_dir, objects_dir)
        self._clone_libs(download_dir)
        self._make_archives(download_dir)
        self._unpack_archives(download_dir, objects_dir)

    def _init_archive_dirs(self, src_dir, dest_dir):
        if os.path.exists(src_dir):
            shutil.rmtree(src_dir)
        os.makedirs(src_dir)

        if os.path.exists(dest_dir):    # ydgen_home/gen-api/cpp/ydk/.libs
            shutil.rmtree(dest_dir)
        os.makedirs(dest_dir)
        os.makedirs('%s/libnetconf' % dest_dir)
        os.makedirs('%s/libyang' % dest_dir)

    def _clone_libs(self, dest_dir):
        os.chdir(dest_dir)
        call(["git", "clone", "https://github.com/abhikeshav/libnetconf"])
        call(["git", "clone", "-b", "ydk_core", "https://github.com/manradhaCisco/libyang"])

    def _make_archives(self, dest_dir):
        libnetconf      = '%s/libnetconf' % dest_dir    # ydkgen_home/.temp/libnetconf
        libyang         = '%s/libyang' % dest_dir       # ydkgen_home/.temp/libyang

        os.chdir(libnetconf)
        call(["./configure"])
        call(["make &> /dev/null"], shell=True)

        os.chdir(libyang)
        os.makedirs("build")
        os.chdir("build")
        call(["cmake", ".."])
        call(["make &> /dev/null"], shell=True)

    def _unpack_archives(self, src_dir, dest_dir):
        libnetconf_archive  = '%s/libnetconf/.libs/libnetconf.a' % src_dir  # ydkgen_home/.temp/libnetconf/.libs/libnetconf.a
        libyang_archive     = '%s/libyang/build/libyang.a' % src_dir        # ydkgen_home/.temp/libyang/build/libyang.a
        
        os.chdir(dest_dir)
        call(["tar", "-xf", libnetconf_archive, "-C", "libnetconf" ])
        call(["tar", "-xf", libyang_archive, "-C", "libyang" ])

if __name__ == '__main__':
    home_dir = os.getcwd()
    dl = LibDownloader(home_dir)
    dl.download()
