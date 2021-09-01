#!/user/bin/env python3

"""Concatenates csv files provided via stdin."""

from iterdub import iterdub as ib
from keyname import keyname as kn
import pandas as pd
import sys
from tqdm import tqdm

# adapted from https://github.com/mmore500/dishtiny/blob/2f60ec6423b9fcd7933a6e0c2576d65cf0f9a664/script/concat_csvs.py#L21
def make_output_filename(filenames):

    df = pd.DataFrame.from_records(
        [
            kn.unpack(source_filename)
            for source_filename in filenames
        ],
    )

    out_attrs = {
        column : ib.dub( df[column] )
        for column in df.columns
        if column not in { '_', 'ext', }
        if not column.startswith('_')
    }

    out_filename = kn.pack({
        **out_attrs,
        **{
            'concat' : str( len( filenames ) ),
            'ext' : '.csv',
        },
    })

    return out_filename


def load_df_with_patches(filename):
    res = pd.read_csv(filename)

    res['Source File'] = filename

    # manual patches for data collection bugs
    if '_revision=3a4253c2-clean' in filename:
        res['SLURM_NNODES'] = 64
        if 'subject=outlet' in filename:
            res.drop(columns=['Num Round Trip Touches.1'], inplace=True)
            res.rename(columns={
                'Round Trip Touches Per Attempted Pull.1'
                    : 'Mean Round Trip Touches Per Attempted Pull'
            }, inplace=True)
    elif '_revision=f13788fa-clean' in filename:
        res['SLURM_NNODES'] = 256
        if 'subject=outlet' in filename:
            res.drop(columns=['Num Round Trip Touches.1'], inplace=True)
            res.rename(columns={
                'Round Trip Touches Per Attempted Pull.1'
                    : 'Mean Round Trip Touches Per Attempted Pull'
            }, inplace=True)

    return res

filenames = [filename.rstrip() for filename in sys.stdin]

dataframes = (load_df_with_patches(f) for f in tqdm(filenames))
concatenated_df = pd.concat(dataframes, ignore_index=True)

concatenated_df.to_csv(
    make_output_filename(filenames),
    index=False,
)
