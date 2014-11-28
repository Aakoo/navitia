# Copyright (c) 2001-2014, Canal TP and/or its affiliates. All rights reserved.
#
# This file is part of Navitia,
#     the software to build cool stuff with public transport.
#
# Hope you'll enjoy and contribute to this project,
#     powered by Canal TP (www.canaltp.fr).
# Help us simplify mobility and open public transport:
#     a non ending quest to the responsive locomotion way of traveling!
#
# LICENCE: This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Stay tuned using
# twitter @navitia
# IRC #navitia on freenode
# https://groups.google.com/d/forum/navitia
# www.navitia.io

from navitiacommon import models
from tyr import tasks
import logging
from tyr import manager
from tyr.helper import get_instance_logger

@manager.command
def import_last_dataset(instance_name, background=False, reload_kraken=False, custom_output=None):
    """
    reimport the last dataset of a instance
    By default the kraken is not reloaded, the '-r' switch can activate it

    the output parameter is the name of the nav file create. If not given, the instance default one is taken
    Note that the output file will be created in the same directory as the instance default output file
    """
    instance = models.Instance.query.filter_by(name=instance_name).first()

    if not instance:
        raise Exception("cannot find instance {}".format(instance_name))

    files = [d.name for d in instance.last_datasets(1)]
    logger = get_instance_logger(instance)
    logger.info('we reimport the last dataset of %s, composed of: %s', instance.name, files)
    tasks.import_data(files, instance, backup_file=False,
                      async=background, reload=reload_kraken, custom_output=custom_output)


